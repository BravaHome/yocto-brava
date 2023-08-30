#ifndef __r485__defined
#define __r485__defined
#include <stdint.h>

#define R485D_SUCCESS (0)
#define R485D_CODE_WRITE (1)
#define R485D_CODE_READ (2)
#define R485D_CODE_LOG (3)
#define R485D_CODE_SHUT_DOWN (4)
#define R485D_CODE_TRANSACT (5)
#define R485D_CODE_ADD_DEVICE (6)
#define R485D_CODE_GET_STATUS (7)
#define R485D_CODE_SET_SERIAL_NUMBER (8)
#define R485D_CODE_SET_CONTROL_FLAGS (9)

#define R485D_WRITE_COMPLETE (64)
#define R485D_READ_COMPLETE (65)
#define R485D_LOG_COMPLETE (66)
#define R485D_TRANSACT_COMPLETE (67)
#define R485D_ADD_DEVICE_COMPLETE (68)
#define R485D_STATUS_COMPLETE (69)
#define R485D_CODE_PACKET_MALFORMED (128)
#define R485D_COULD_NOT_WRITE_TO_SERIAL_PORT (129)
#define R485D_CODE_COULD_NOT_READ_FROM_SERIAL_PORT (130)
#define R485D_CODE_READ_TIMEOUT (131)
#define R485D_CRC_ERROR (132)
#define R485D_VALUE_MISMATCH (133)
#define R485D_ELAN_INTERACTION_FAILURE (134)

#define REMOTE_TIMEOUT (10000)
#define BUFFER_SIZE (1024)

#define R485D_VERSION (1.0)
#define R485D_MESSAGE_VERSION "R485D version 1.0 started"
#define R485D_MESSAGE_CONNECTING "R485D Using port %s for MODBUS activity"
#define R485D_MESSAGE_DEVICES_CONNECTED "R485D Physical devices connected"
#define R485D_MESSAGE_SOCKET_CONNECTED "R485D Socket connected"
#define R485D_DEVICE_UNAVAILABLE "R485D Physical device %d failed to respond and has been deactivated"

#define R485D_CONNECTION_FLAG_CONNECTED_ONCE (1)
#define R485D_INITIAL_CONNECTION_COUNT_MASK (0xFE)
#define R485D_RESET_INITIAL_CONNECTION_COUNT (0xFFFFFF01)
#define R485D_INITIAL_CONNECTION_INCREMENT (0x2)
#define R485D_INITIAL_CONNECTION_ATTEMPTS (10)
#define R485D_INITIAL_CONNECTION_LIMIT (R485D_INITIAL_CONNECTION_INCREMENT*R485D_INITIAL_CONNECTION_ATTEMPTS)


struct elan_device_status_record
{
uint32_t initialization_flags;
int32_t connection_count;           // Number of times the controller connected with the device, designed for detecting intervening disconnects
int elan_initialized;               // Nonzero indicates the initialization sequence has occurred and status messages are being exchanged
uint32_t outbound_status_bits;      // Status bits output during the initialization sequence
uint32_t inbound_status_bits;       // Status bits read during the last status message
uint8_t model_number;
uint8_t model_flags;
uint32_t oven_temp;
uint32_t raw_temp;
uint32_t probe_temp;
uint32_t input_digital_mask;
uint32_t current;
uint8_t s_bit;
uint16_t soft_reset_count;
uint16_t hard_reset_count;
uint32_t fan_speed;
uint8_t serial_number_data[16];
uint32_t serial_number_is_valid;
uint32_t spare1;
uint32_t spare2;
uint32_t spare3;
uint32_t spare4;
uint32_t spare5;
uint32_t spare6;
uint32_t spare7;
};

struct rs485d_device_status_record
{
uint16_t payload_size;
uint8_t payload_command;
uint8_t device_class;
union
    {
    struct elan_device_status_record eoc6;
    };
};


struct eoc6_device_setting_record
{
uint32_t set_point;
uint32_t cycle;
uint32_t power_state;
uint32_t digital_mask;
};

struct rs485d_transaction_record
{
uint16_t payload_size;
uint8_t payload_command;
uint8_t logical_device;
uint8_t command;
uint32_t value;
uint32_t flags;
uint8_t verifying_read_command;
};

#define R485D_DEVICE_CLASS_UNKNOWN (0)
#define R485D_DEVICE_CLASS_EOC6 (1)
#define R485D_DEVICE_CLASS_PMC (2)

struct rs485d_device_initialization_record
{
uint16_t payload_size;
uint8_t payload_command;
uint8_t logical_device;
uint8_t physical_device;
uint8_t device_class;
uint32_t flags;
};


#define ELAN_INTER_MESSAGE_DELAY (5000)
#define ELAN_READ_TIMEOUT (1)
#define ELAN_RETRIES (4)

#define EOC6_COMMAND_PROTOCOL_ID (1)
#define EOC6_COMMAND_SOFTWARE_VERSION (2)
#define EOC6_COMMAND_INITIALIZATION_COMPLETE (3)
#define EOC6_COMMAND_STATUS (6)
#define EOC6_COMMAND_GET_MODEL (65)
#define EOC6_COMMAND_SET_POINT (66)
#define EOC6_COMMAND_CYCLE (67)
#define EOC6_COMMAND_TEMPERATURE_ADJUST (68)
#define EOC6_COMMAND_OVEN_TEMP (69)
#define EOC6_COMMAND_PROBE_TEMP (70)
#define EOC6_COMMAND_DIGITAL_OUTPUT (71)
#define EOC6_COMMAND_CURRENT (72)
#define EOC6_COMMAND_FAN_SPEED (73)
#define EOC6_COMMAND_ACTIVE_LOADS (74)

#define EOC6_COMMAND_START_STOP (4)
#define EOC6_SUBCOMMAND_STOP (1)
#define EOC6_SUBCOMMAND_START_WITH_PREHEAT (2)
#define EOC6_SUBCOMMAND_START_WITHOUT_PREHEAT (6)
#define EOC6_PROTOCOL_ID (1)
#define EOC6_SOFTWARE_VERSION (2)
#define EOC6_NOSHOWROOM 0x01
#define EOC6_NOSABBATH 0x02
#define EOC6_UISABBATHANDTEMPAD 0x08
#define EOC6_ERROR_MASK (0xff0000)
#define EOC6_ERROR_COMMUNICATION_LOST (0x080000)

#define EOC6_SESSION_OPTIONS (EOC6_NOSHOWROOM | EOC6_NOSABBATH | EOC6_UISABBATHANDTEMPAD)

#define LOGICAL_DEVICE_FIRST_OVEN (0)
#define LOGICAL_DEVICE_SECOND_OVEN (1)
#define LOGICAL_DEVICE_PMC (2)

#define ELAN_PHYSICAL_DEVICE_FIRST_OVEN (2)
#define ELAN_PHYSICAL_DEVICE_SECOND_OVEN (3)
#define ELAN_PHYSICAL_DEVICE_PMC (5)

#define CLIENT_SOCKET_NAME "/tmp/rs485c.socket"
#define SERVER_SOCKET_NAME "/tmp/rs485.socket"
#define SIMULATION_SOCKET_NAME "/tmp/rs485sim.socket"


#define STATUS_DELAY (1250000000)

#define MAX_CONTROLLED_DEVICES (16)

#define MAX_SETTINGS_COUNT (32)
#define RS485D_SETTINGS_STATUS_PERIOD (0)
#define RS485D_SETTINGS_OVEN_TEMP_PERIOD (1)
#define RS485D_SETTINGS_PROBE_TEMP_PERIOD (2)
#define RS485D_SETTINGS_CURRENT_PERIOD (3)
#define RS485D_SETTINGS_FAN_SPEED_PERIOD (4)
#define RS485D_SETTINGS_ACTIVE_MASK_PERIOD (5)
#define RS485D_SETTINGS_CONNECTION_PERIOD (6)
#define RS485D_SETTINGS_LONG_CONNECTION_PERIOD (7)

#define PMC_PROTOCOL_VERSION (2)
#define PMC_SOFTWARE_VERSION (4)
#define PMC_COMMAND_STATUS (6)
#define PMC_SET_LONG_PRESS_TIME (103)
#define PMC_SET_LONG_PRESS_ACK (104)
#define PMC_SET_RESET_WARNING_ACK (105)
#define PMC_SET_POWER_OFF_TIME (106)
#define PMC_ACK_RESET_WARNING_SIGNAL (107)
#define PMC_ACK_MASTER_RESET (108)
#define PMC_CLEAR_S_BIT (109)
#define PMC_LOW_POWER_MODE (110)
#define PMC_GET_RESET_COUNTS (111)
#define PMC_SERIAL_NUMBER_WRITE (112)
#define PMC_SERIAL_NUMBER_READ (113)
#define PMC_LONG_PRESS_TIME_VALUE (10000)



/* For the SSR IO Molex connector
When looking at the board with the lock tab down the pins are:
12345
67890

Pins
1,2     RS485 Termination
3       RS485 A
4       RS485 B
5       VCC
6       GND
7       GPIO 1
8       GPIO 2
9       GPIO 3
10      NC

GPIOs show up as two devices
GPIO    1   IN      gpiochip2   2
GPIO    2   IN      gpiochip2   3
GPIO    3   IN      gpiochip2   4
GPIO    1   OUT     gpiochip2   5
GPIO    2   OUT     gpiochip2   6
GPIO    3   OUT     gpiochip2   7


The SSRIO is connected to the PMC
P10 on the PMC has the following pin assignments:
When looking at the board with the lock tab down the pins are:
12345
67890



1,2     RS485 Termination
3       RS485+
4       RS485-
5       VCC
6       GND
7       GPIO output reset_warning
8       GPIO input wake from sleep
9       Spare
10      NC
*/

#define GPIO_CHIP_NAME "/dev/gpiochip2"
#define GPIO_LINE_RESET_WARNING (2)
#define GPIO_LINE_RESET_WARNING_OUTPUT (5)
#define GPIO_LINE_FACTORY_RESET (4)
#define GPIO_LINE_FACTORY_RESET_OUTPUT (7)
#define GPIO_LINE_WAKE_FROM_SLEEP (6)
#define GPIO_CONSUMER_NAME "R485D"



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


int rs485d_write_to_rs485(int new_socket, int size,char *buffer,void* server_addr,int server_length);

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



int rs485d_read_from_rs485(int new_socket, int size,char *buffer,void* server_addr,int server_length,int timeout, int *bytes_read);

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


int rs485d_transact_with_rs485(int new_socket, uint8_t logical_device, uint8_t command, uint32_t value, uint32_t flags, uint8_t verifying_read_command,void* server_addr,int server_length, uint8_t *result_buffer);

/*
Routine to print a log of recent RS485 activity with timing.

Parameters:
server_addr:    A pointer to a struct sockaddr_un with the name of the RS-485 deamon
server_length:  The length of server_addr structure
*/

void rs485_print_rs485_log(int new_socket,void* server_addr,int server_length);

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

int rs485d_add_device(int new_socket, uint8_t logical_device, uint8_t physical_device, uint8_t device_class, uint32_t flags,void* server_addr,int server_length);

/* 
Routine to get the status of a rs485 connected logical device

Parameters:
new_socket:         The handle to the linux domain datagram socket
logical_device:     The logical device number to assign to the new device
server_addr:        A pointer to a struct sockaddr_un with the name of the RS-485 deamon
server_length:      The length of server_addr structure
*/

int rs485d_get_device_status(int new_socket, uint8_t logical_device, void* server_addr,int server_length, struct rs485d_device_status_record *result);

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

int rs485d_set_serial_number(int new_socket, int size,char *buffer,void* server_addr,int server_length);

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

int rs485d_set_control_flags(int new_socket, int flags, void* server_addr,int server_length);




#endif
