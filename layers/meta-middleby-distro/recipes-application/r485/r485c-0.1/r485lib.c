#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <errno.h>
#include <sys/un.h>
#include <poll.h>
#include <sched.h>
#include "r485lib.h"
#include "r485.h"


int high_precision_baseline_defined=0;
int64_t high_precision_baseline=0;

int64_t get_high_precision_tick()
{
struct timespec ts;
int64_t result;
clock_gettime(CLOCK_REALTIME, &ts);
result =  ((int64_t)(ts.tv_sec) * (int64_t)1000000000UL+ (int64_t)(ts.tv_nsec));
if (high_precision_baseline_defined==0)
    {
    high_precision_baseline = result;
    high_precision_baseline_defined = 1;
    }
return (result-high_precision_baseline);
}



int log_socket_errors=0;


// Routine to change the setting of logging on socket errors
// value = 1 means log errors 0 means don't log errors
void set_socket_error_display(int value)
{
log_socket_errors = value;
}

// Routine to read from a datagram socket while being resilient to signals and interruptions
//
// Paramaters:
// handle is the socket handle
// buffer is the location to store the data
// size is the size of the buffer
// actual_bytes_read is the number of bytes read
// server_address is the address of the source of the datagram
// server_length is the lenght of the address (This must be initialized to the size of the memory buffer source_address)
//
// Returns:
// 0 on success 1 on error
int read_full(int handle,char *buffer,int size,int *actual_bytes_read, void *server_addr, int *server_length)
{
int bytes_read;
while (1)
    {
    *server_length = sizeof(struct sockaddr_un);
    bytes_read = recvfrom(handle,buffer,size,0,(struct sockaddr *)server_addr, (unsigned int*)server_length);
    if (bytes_read<=0)
        {
        if (bytes_read<0)
            {
            if (errno==EINTR)
                continue;
            if (log_socket_errors)
                fprintf(stderr,"r485lib,read_full socket error %d\n",errno);
            }
        return 1;
        }
    break;
    }
*actual_bytes_read = bytes_read;
return 0;
}

// Routine to read from a datagram socket while being resilient to signals and interruptions
// Paramaters:
// handle is the socket handle
// buffer is the location to store the data
// size is the size of the buffer
// actual_bytes_written is the number of bytes written
// client_address is the address of the source of the datagram
// client_length is the lenght of the address 
// wait_mode = 1 means wait for transmission to be allowed 0 means dont wait
//
// Returns:
// 0 on success
int send_full(int handle,char *buffer,int size,int *actual_bytes_written, void *client_addr, int client_length, int wait_mode)
{
int bytes_written;
if (wait_mode)
    wait_mode = 0;
else
    wait_mode = MSG_DONTWAIT;
while (1)
    {
//    printf("Writing %d %d \n",size,buffer[2]);
    bytes_written = sendto(handle,buffer,size,wait_mode,(struct sockaddr *)client_addr, client_length);
    if (bytes_written!=size)
        {
        if (bytes_written<0)
            {
            if (errno==EINTR)
                continue;
            else
                {
                if (log_socket_errors)
                    fprintf(stderr,"r485lib,send_full socket error %d\n",errno);
                }
            }
        return 1;
        }
    else
        break;
    }
*actual_bytes_written = size;
return 0;
}

// Routine to poll a socket for the availability of data
// This version does not schedule any hidden timers but is CPU intensive
//
// Parameters:
// handle is the socket handle
// timeout in the amount of time in milliseconds to wait for data
//
// Returns:
// 0 on data available
// 1 on timeout
// 2 on socket error
int poll_for_read_without_select(int handle,int timeout)
{
char scratch_buffer[1];
struct sockaddr_un client_addr;
int client_length;
client_length = sizeof(struct sockaddr_un);
int64_t st;
int64_t et;
int bytes_read;
st = get_high_precision_tick();
while (1)
    {
    bytes_read = recvfrom(handle,scratch_buffer,1,MSG_DONTWAIT|MSG_PEEK,(struct sockaddr *)&client_addr, (unsigned int*)&client_length);
    if (bytes_read>0)
        return 0;
    if (bytes_read<0)
        {
        if (errno==EINTR || errno==EAGAIN || errno == EWOULDBLOCK)
            {
            et = get_high_precision_tick();
            if (et<st || (int)((et-st)/1000000)>timeout)
                break;
            sched_yield();
            continue;
            }
        if (log_socket_errors)
            fprintf(stderr,"r485lib,poll_for_read_without_select socket error %d\n",errno);
        return 2;
        }
    et = get_high_precision_tick();
    if (et<st || (int)((et-st)/1000000)>timeout)
        break;
    sched_yield();
    }
return 1;
}

// Routine to poll a socket for the availability of data
// This version uses select
//
// Parameters:
// handle is the socket handle
// timeout in the amount of time in milliseconds to wait for data
//
// Returns:
// 0 on data available
// 1 on timeout
// 2 on socket error
int poll_for_read(int handle,int timeout)
{
int j;
struct pollfd pfd;
pfd.fd = handle;
pfd.events = POLLIN;
pfd.revents = 0;
j = poll(&pfd,1,timeout);
if (j==0 || pfd.revents==0)
    {
    return 1;
    }
if (pfd.revents&POLLIN)
    return 0;
return 2;
}


// Fluches the input socket stream
// This version does not use select
// !!Warning!! This routine will fail if datagrams are larger than 64K
#define LARGE_SCRATCH_BUFFER_SIZE (65536)
void flush_input_without_select(int handle)
{
char scratch[LARGE_SCRATCH_BUFFER_SIZE];
int k;
struct sockaddr_un ret_addr;
int ret_length;

while (poll_for_read_without_select(handle,0)==0)
    {
    read_full(handle,scratch,LARGE_SCRATCH_BUFFER_SIZE,&k,&ret_addr,&ret_length);
    }
}

// Fluches the input socket stream
// !!Warning!! This routine will fail if datagrams are larger than 64K
#define LARGE_SCRATCH_BUFFER_SIZE (65536)
void flush_input(int handle)
{
char scratch[LARGE_SCRATCH_BUFFER_SIZE];
int k;
struct sockaddr_un ret_addr;
int ret_length;

while (poll_for_read(handle,0)==0)
    {
    read_full(handle,scratch,LARGE_SCRATCH_BUFFER_SIZE,&k,&ret_addr,&ret_length);
    }
}

uint16_t crc16_update(uint16_t crc, uint8_t a) 
{
crc ^= a;
for (uint32_t i = 0; i < 8; ++i) 
    {
    if (crc & 1)
        crc = (crc >> 1) ^ 0xA001;
    else
        crc = (crc >> 1);
    }
return crc;
}

uint16_t crc16(uint8_t* data, uint8_t data_size) 
{
uint16_t crc = 0xffff;
for (uint8_t i = 0; i < data_size; i++)
    crc = crc16_update(crc, data[i]);
return crc;
}


void prepare_request(uint8_t addr, uint8_t type, uint32_t msgData, uint8_t *buffer) 
{
buffer[0] = addr;
buffer[1] = type;
buffer[2] = msgData >>  0;
buffer[3] = msgData >>  8;
buffer[4] = msgData >> 16;
buffer[5] = msgData >> 24;
uint16_t crc = crc16(buffer, 8 - 2);
buffer[6] = crc >> 0;
buffer[7] = crc >> 8;
}

uint32_t fetch_reply_data(uint8_t *buffer)
{
return 0 | buffer[2] << 0 | buffer[3] << 8 | buffer[4] << 16 | buffer[5] << 24;
}

/*
Routine to request that data be written to the RS-485 bus.

Parameters:
new_socket:     The handle to the linux domain datagram socket
size:           The number of bytes to write
buffer:         The source data to write
server_addr:    A pointer to a struct sockaddr_un with the name of the RS-485 deamon
server_length:  The length of server_addr structure

Returns:
64              Success
-2              For an internal socket error or perhaps if the remote socket has closed
-3              If there was a timeout from the RS485 deamon acknowledging the write
-4              If there was a socket error getting the acknowledgement or the response was malformed
128             If the transmission from our client was malformed
129             If the RS485 port was not working
All others      The sockets are out of sync
*/


int rs485d_write_to_rs485(int new_socket, int size,char *buffer,void* server_addr,int server_length)
{
char scratch[BUFFER_SIZE];
int j;
int k;
struct sockaddr_un ret_addr;
int ret_length;
if (size>BUFFER_SIZE+3)
    return -1;
flush_input(new_socket);
*(uint16_t *)scratch = size+1;
scratch[2]=R485D_CODE_WRITE;
memcpy(scratch+3,buffer,size);
j = send_full(new_socket,scratch,size+3,&k,server_addr,server_length,1);
if (j!=0)
    return -2;
j = poll_for_read(new_socket,REMOTE_TIMEOUT);
if (j==1)
    return -3;
if (j!=0)
    return -2;   
j = read_full(new_socket,scratch,BUFFER_SIZE,&k,&ret_addr,&ret_length);
if (j!=0 || k<3)
    return -4;
return scratch[2];
}


/*
Routine to request that data be read from the RS-485 bus.

Parameters:
new_socket:     The handle to the linux domain datagram socket
size:           The number of bytes to read
buffer:         The destination location where data should be written
server_addr:    A pointer to a struct sockaddr_un with the name of the RS-485 deamon
server_length:  The length of server_addr structure
timeout:        Number of tenths of a second to wait for the input data
bytes_read:     Location where the number of bytes read should be stored. 

Returns:
65              Success
-1              Insufficient buffer space
-2              For an internal socket error or perhaps if the remote socket has closed
-3              If there was a timeout from the RS485 deamon acknowledging the read
-4              If there was a socket error getting the acknowledgement or the response was malformed
128             If the transmission from our client was malformed
130             If the RS485 port was not working
131             The was a timeout waiting for the RS485 data
All others      The sockets are out of sync


NOTE NOTE only on success will the bytes_read and buffer be filled. If the buffer is in a static location, confusing prior data could be stored in the buffer.
Always verify success before using the data.
*/



int rs485d_read_from_rs485(int new_socket, int size,char *buffer,void* server_addr,int server_length,int timeout, int *bytes_read)
{
char scratch[BUFFER_SIZE];
int j;
int k;
struct sockaddr_un ret_addr;
int ret_length;
if (size>BUFFER_SIZE+3)
    return -1;
flush_input(new_socket);
*(uint16_t *)scratch = 5;
scratch[2]=R485D_CODE_READ;
*(uint16_t *)(scratch+3) = size;
*(uint16_t *)(scratch+5) = timeout;
j = send_full(new_socket,scratch,7,&k,server_addr,server_length,1);
if (j!=0)
    return -2;
j = poll_for_read(new_socket,REMOTE_TIMEOUT);
if (j==1)
    return -3;
if (j!=0)
    return -2;   
j = read_full(new_socket,scratch,BUFFER_SIZE,&k,&ret_addr,(int *)&ret_length);
//printf("Read j,k %d %d\n",j,k);
if (j!=0 || k<3)
    return -4;
if (scratch[2]==R485D_READ_COMPLETE)
    {
    k-=3;
    if (size<k)
        k = size;
    *bytes_read = k;
    memcpy(buffer,scratch+3,k);    
    }
return scratch[2];
}


/*
Routine to do an ELAN interations

Parameters:
new_socket:     The handle to the linux domain datagram socket
logical_device: Is the logical device to interact with (see RS485_LOGDEV_ constants)
command         The modbus command to issue
value           The value associated with the modbus command
flags           Transaction flags (see RS485_TRANSACT_ constants)
verifying_read_command
                If the RS485_TRANSACT_VERIFY bit is set then this is the read command to issue on the bus to verify the data
result          The result data for the command
*/


int rs485d_transact_with_rs485(int new_socket, uint8_t logical_device, uint8_t command, uint32_t value, uint32_t flags, uint8_t verifying_read_command,void* server_addr,int server_length, uint8_t *result_buffer)

{
struct rs485d_transaction_record transaction;

char scratch[BUFFER_SIZE];
int j;
int k;
struct sockaddr_un ret_addr;
int ret_length;
transaction.payload_size = sizeof(struct rs485d_transaction_record)-2;
transaction.payload_command = R485D_CODE_TRANSACT;
transaction.logical_device = logical_device;
transaction.command = command;
transaction.value = value;
transaction.flags = flags;
transaction.verifying_read_command = verifying_read_command;
flush_input(new_socket);
j = send_full(new_socket,(char *)&transaction,sizeof(struct rs485d_transaction_record),&k,server_addr,server_length,1);
if (j!=0)
    return -2;
j = poll_for_read(new_socket,REMOTE_TIMEOUT);
if (j==1)
    return -3;
if (j!=0)
    return -2;   
j = read_full(new_socket,scratch,BUFFER_SIZE,&k,&ret_addr,(int *)&ret_length);
//printf("Read j,k %d %d\n",j,k);
if (j!=0 || k!=11)
    return -4;
if (scratch[2]==R485D_TRANSACT_COMPLETE)
    memcpy(result_buffer,scratch+3,8);
return scratch[2];
}

struct logentryt
{
char event_type;
long long ts;
char buffer[8];
};



/*
Routine to print a log of recent RS485 activity with timing.

Parameters:
server_addr:    A pointer to a struct sockaddr_un with the name of the RS-485 deamon
server_length:  The length of server_addr structure
*/

void rs485_print_rs485_log(int new_socket,void* server_addr,int server_length)
{
char scratch[BUFFER_SIZE];
int j;
int k;
struct sockaddr_un ret_addr;
int ret_length;


flush_input(new_socket);
*(uint16_t *)scratch = 1;
scratch[2]=R485D_CODE_LOG;
j = send_full(new_socket,scratch,3,&k,server_addr,server_length,1);
if (j!=0)
    return;
j = poll_for_read(new_socket,REMOTE_TIMEOUT);
if (j==1)
    return;
if (j!=0)
    return;   
j = read_full(new_socket,scratch,BUFFER_SIZE,&k,&ret_addr,&ret_length);
if (j!=0 || k<3)
    return;
if (scratch[2]==R485D_LOG_COMPLETE)
    {
    j = *(uint16_t *)(scratch+3);
    if (j==0)
        return;
    char *p = scratch+5;
    struct logentryt *e;
    while (j>0)
        {
        e = (struct logentryt *)p;
        printf("%d %014lld 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", e->event_type,e->ts,e->buffer[0],e->buffer[1],e->buffer[2],e->buffer[3],e->buffer[4],e->buffer[5],e->buffer[6],e->buffer[7]);
        p += sizeof(struct logentryt);
        j--;
        }
    }
return;

}


/* 
Routine to connect a device to the RS485 network

Parameters:
new_socket:         The handle to the linux domain datagram socket
logical_device:     The logical device number to assign to the new device
physical_device:    The physical device address on the RS485 network
device_class:       The type of device being added right now only RS485_DEVICE_CLASS_EOC6 is defined
flags:              A set of device class dependent flags
server_addr:        A pointer to a struct sockaddr_un with the name of the RS-485 deamon
server_length:      The length of server_addr structure
*/

int rs485d_add_device(int new_socket, uint8_t logical_device, uint8_t physical_device, uint8_t device_class, uint32_t flags,void* server_addr,int server_length)
{
struct rs485d_device_initialization_record initialization;

char scratch[BUFFER_SIZE];
int j;
int k;
struct sockaddr_un ret_addr;
int ret_length;
initialization.payload_size = sizeof(struct rs485d_device_initialization_record)-2;
initialization.payload_command = R485D_CODE_ADD_DEVICE;
initialization.logical_device = logical_device;
initialization.physical_device = physical_device;
initialization.device_class = device_class;
initialization.flags = flags;
flush_input(new_socket);
j = send_full(new_socket,(char *)&initialization,sizeof(struct rs485d_device_initialization_record),&k,server_addr,server_length,1);
if (j!=0)
    return -2;
j = poll_for_read(new_socket,REMOTE_TIMEOUT);
if (j==1)
    return -3;
if (j!=0)
    return -2;   
j = read_full(new_socket,scratch,BUFFER_SIZE,&k,&ret_addr,(int *)&ret_length);
//printf("Read j,k %d %d\n",j,k);
if (j!=0 || k<3)
    return -4;
return scratch[2];
}

/* 
Routine to get the status of a rs485 connected logical device

Parameters:
new_socket:         The handle to the linux domain datagram socket
logical_device:     The logical device number to assign to the new device
server_addr:        A pointer to a struct sockaddr_un with the name of the RS-485 deamon
server_length:      The length of server_addr structure
*/

int rs485d_get_device_status(int new_socket, uint8_t logical_device, void* server_addr,int server_length, struct rs485d_device_status_record *result)
{

char scratch[BUFFER_SIZE];
int j;
int k;
struct sockaddr_un ret_addr;
int ret_length;
scratch[2]=R485D_CODE_GET_STATUS;
scratch[3]=logical_device;
*(uint16_t *)scratch = 2;
flush_input(new_socket);
j = send_full(new_socket,(char *)scratch,4,&k,server_addr,server_length,1);
if (j!=0)
    return -2;
j = poll_for_read(new_socket,REMOTE_TIMEOUT);
if (j==1)
    return -3;
if (j!=0)
    return -2;   
j = read_full(new_socket,scratch,BUFFER_SIZE,&k,&ret_addr,(int *)&ret_length);
if (j!=0 || k!=sizeof(struct rs485d_device_status_record))
    return -4;
memcpy((void *)result,scratch,sizeof(struct rs485d_device_status_record));
return scratch[2];
}


/*
Routine to set the serial number on the pmc.
The process of writing the serial number is slow. Wait 2 seconds and then check a status request for a new serial number
Parameters:
new_socket:     The handle to the linux domain datagram socket
size:           The number of characters in the serial number, must be between 1 and 13
buffer:         The source data to write
server_addr:    A pointer to a struct sockaddr_un with the name of the RS-485 deamon
server_length:  The length of server_addr structure

Returns:
64              Success
-2              For an internal socket error or perhaps if the remote socket has closed
-3              If there was a timeout from the RS485 deamon acknowledging the write
-4              If there was a socket error getting the acknowledgement or the response was malformed
128             If the transmission from our client was malformed
All others      The sockets are out of sync
*/

int rs485d_set_serial_number(int new_socket, int size,char *buffer,void* server_addr,int server_length)
{
char scratch[BUFFER_SIZE];
int j;
int k;
struct sockaddr_un ret_addr;
int ret_length;
if (size>BUFFER_SIZE+3)
    return -1;
flush_input(new_socket);
*(uint16_t *)scratch = size+1;
scratch[2]=R485D_CODE_SET_SERIAL_NUMBER;
memcpy(scratch+3,buffer,size);
j = send_full(new_socket,scratch,size+3,&k,server_addr,server_length,1);
if (j!=0)
    return -2;
j = poll_for_read(new_socket,REMOTE_TIMEOUT);
if (j==1)
    return -3;
if (j!=0)
    return -2;   
j = read_full(new_socket,scratch,BUFFER_SIZE,&k,&ret_addr,&ret_length);
if (j!=0 || k<3)
    return -4;
return scratch[2];
}

/*
Set core r485d flags
The process of writing the serial number is slow. Wait 2 seconds and then check a status request for a new serial number
Parameters:
new_socket:     The handle to the linux domain datagram socket
flags:          bits 0..1 0 = Normal mode, 1 = US, 2 = European, 3 = Display only
                bit  2=1 report good ui health
                bits 8..15 syslog verbosity
                0 Everything
                4 State changes and errors
                8 Only errors
server_addr:    A pointer to a struct sockaddr_un with the name of the RS-485 deamon
server_length:  The length of server_addr structure

Returns:
64              Success
-2              For an internal socket error or perhaps if the remote socket has closed
-3              If there was a timeout from the RS485 deamon acknowledging the write
-4              If there was a socket error getting the acknowledgement or the response was malformed
128             If the transmission from our client was malformed
All others      The sockets are out of sync
*/

int rs485d_set_control_flags(int new_socket, int flags, void* server_addr,int server_length)
{
char scratch[BUFFER_SIZE];
int j;
int k;
struct sockaddr_un ret_addr;
int ret_length;
flush_input(new_socket);
*(uint16_t *)scratch = 4+1;
scratch[2]=R485D_CODE_SET_CONTROL_FLAGS;
*((int *)(scratch+3)) = flags;
j = send_full(new_socket,scratch,4+3,&k,server_addr,server_length,1);
if (j!=0)
    return -2;
j = poll_for_read(new_socket,REMOTE_TIMEOUT);
if (j==1)
    return -3;
if (j!=0)
    return -2;   
j = read_full(new_socket,scratch,BUFFER_SIZE,&k,&ret_addr,&ret_length);
if (j!=0 || k<3)
    return -4;
return scratch[2];
}

