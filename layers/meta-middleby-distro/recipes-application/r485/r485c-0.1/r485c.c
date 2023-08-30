#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <poll.h>
#include <getopt.h>
#include <time.h>
#include <errno.h>
#include <sys/un.h>
#include <poll.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "r485.h"
#include "r485lib.h"


uint8_t elan_output_buffer[8];
uint8_t elan_input_buffer[8];
int elan_device_count = 2;
volatile struct rs485d_device_status_record device_status[MAX_CONTROLLED_DEVICES];
volatile struct eoc6_device_setting_record device_setting[MAX_CONTROLLED_DEVICES];
int new_socket;
struct sockaddr_un server_addr;
struct sockaddr_un client_addr;

// Will be depricated once Jon gets a programmed EOC6 v92
int do_verified_command(int device,uint8_t command,uint32_t value)
{
int j;
int k;
int retries;
retries = ELAN_RETRIES;
while (retries--)
    {
    j = rs485d_transact_with_rs485(new_socket,device,command | 0x80,value, 0, 0, &server_addr,sizeof(server_addr),elan_input_buffer);
    if (j!=R485D_TRANSACT_COMPLETE)
        continue;
    k = fetch_reply_data(elan_input_buffer);
//    j = rs485d_transact_with_rs485(new_socket,device,command | 0x80,value, 0, 0, &server_addr,sizeof(server_addr),elan_input_buffer);
//    prepare_request(device,command,0,elan_output_buffer);
//    j = interact_with_elan(ELAN_RETRIES);
//    if (j!=R485D_SUCCESS)
//        continue;
    if (k==value)
        return R485D_SUCCESS;
    printf("Data mismatch\n");
    j = R485D_VALUE_MISMATCH;
    }
return j;

}

int do_setpoint(int device)
{
return do_verified_command(device,EOC6_COMMAND_SET_POINT,device_setting[device].set_point);
}

int do_cycle(int device)
{
return do_verified_command(device,EOC6_COMMAND_CYCLE,device_setting[device].cycle);
}

int do_startstop(int device,uint32_t sub_command)
{
int j;
if (sub_command!=EOC6_SUBCOMMAND_STOP)
    {
    j = do_setpoint(device);
    if (j!=R485D_SUCCESS)
        return j;
    j = do_cycle(device);
    if (j!=R485D_SUCCESS)
        return j;
    }
return rs485d_transact_with_rs485(new_socket,device,EOC6_COMMAND_START_STOP ,sub_command, 0, 0, &server_addr,sizeof(server_addr),elan_input_buffer);
}

int do_digital(int device)
{
return rs485d_transact_with_rs485(new_socket,device,EOC6_COMMAND_DIGITAL_OUTPUT|0x80 ,device_setting[device].digital_mask, 0, 0, &server_addr,sizeof(server_addr),elan_input_buffer);
}

int do_set_serial(int size,char *serial_number)
{
return rs485d_set_serial_number(new_socket,size,serial_number, &server_addr,sizeof(server_addr));
}

int current_sleep_mode = 0;
int current_verbosity = 0;

int do_sleep_mode(int mode)
{
current_sleep_mode = mode;
return rs485d_set_control_flags(new_socket,(current_verbosity<<8)+mode, &server_addr,sizeof(server_addr));
}




char *power_state_messages[] = {"Power state uninitialized.\n","Power state stopped.\n","Power state started with preheat\n","Power state started without preheat\n"};

void print_device_status_info()
{
int j;
int k;
volatile struct rs485d_device_status_record *n;
volatile struct elan_device_status_record *p;
volatile struct eoc6_device_setting_record *q;
char scratch[16];
for (j=0;j<elan_device_count;j++)
    {
    k =  rs485d_get_device_status(new_socket, (uint8_t) j, &server_addr,sizeof(server_addr), (struct rs485d_device_status_record *)&(device_status[j]));
    if (k!=R485D_STATUS_COMPLETE)
        continue;
    n = &(device_status[j]);
    q = &(device_setting[j]);
    printf("\n\n\nStatus for device %d:\n",j);
    switch (n->device_class)
        {
        case R485D_DEVICE_CLASS_EOC6:
            {
            p = &(n->eoc6);
            if (p->elan_initialized)
                {
                printf("Device is an EOC6\n");
                printf("Status:\n");
                printf("Connection count: %d\n", p->connection_count);
                printf("Oven temp %d\n",p->oven_temp);
                printf("Raw temp %d\n",p->raw_temp);
                printf("Probe temp %d\n",p->probe_temp);
                printf("Digital mask %X\n",p->input_digital_mask);
                printf("Current %d\n",p->current);
                printf("Fan speed %d\n",p->fan_speed);
                uint32_t b;
                b = p->inbound_status_bits;
                if ((b&3)==1)
                    printf("Unlatched  ");
                if ((b&3)==2)
                    printf("Latched  ");
                if (b&4)
                    printf("Sabbath  ");
                if (b&8)
                    printf("Light on  ");
                if (b&0x10)
                    printf("Door closed  ");
                else
                    printf("Door open  ");
                if (b&0x100)
                    printf("Cooldown mode  ");
                if (b&0x400)
                    printf("Searing mode  ");
                if (b&0x800)
                    printf("Text mode");
                printf("\n");
                if (b&0xff0000)
                    printf("Error code %d\n",b>>16);
                if (b&0x1000000)
                    printf("Is preheated\n");
                if (b&0x2000000)
                    printf("Heat request\n");
                printf("\nSettings:\n");
                printf("Current set point: %d\n",q->set_point);
                printf("Selected cycle: %d\n",q->cycle);
                printf(power_state_messages[q->power_state]);
                }
            else
                printf("Communication is not initialized for this device.\n");
            break;    
            }
        case R485D_DEVICE_CLASS_PMC:
            {
            p = &(n->eoc6);
            if (p->elan_initialized)
                {
                printf("Device is an PMC\n");
                printf("Status:\n");
                printf("Connection count: %d\n", p->connection_count);
                if (p->serial_number_is_valid)
                    {
                    memcpy(scratch,(void *)&(p->serial_number_data[1]),p->serial_number_data[0]);
                    scratch[p->serial_number_data[0]]=0;
                    printf("Serial number: %s",scratch);
                    }
                else
                    printf("Serial number is not valid");
                }
            else
                printf("Communication is not initialized for this device.\n");
            break;    
            }
        }
    }
}

int open_communication_socket()
{
// Make sure the local side isn't lingering from a prior crash 
unlink(CLIENT_SOCKET_NAME);
//Create the local socket.
new_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
if (new_socket<0)
    {
    printf("Could not create socket\n");
    return 1;
    }
// Bind the name of the local socket
memset(&client_addr, 0, sizeof(struct sockaddr_un));
client_addr.sun_family = AF_UNIX;
strcpy(client_addr.sun_path, CLIENT_SOCKET_NAME);

if (bind(new_socket, (const struct sockaddr *) &client_addr,sizeof(client_addr)))
    {
    printf("Could not bind socket\n");
    return 1;
    }
//
// Construct server address.
//
memset(&server_addr, 0, sizeof(struct sockaddr_un));
server_addr.sun_family = AF_UNIX;
strcpy(server_addr.sun_path, SERVER_SOCKET_NAME);
return 0;
}

void initialize_device_structures()
{
int j;
for (j=0;j<elan_device_count;j++)
    {
    memset((void *)&(device_status[j]),0,sizeof(struct rs485d_device_status_record));
    memset((void *)&(device_setting[j]),0,sizeof(struct eoc6_device_setting_record));
    device_setting[j].power_state = 1;
    }
}


int main(int argc, char **argv)
{
int j;
char buffer[BUFFER_SIZE];

int interacting = 0;
volatile struct eoc6_device_setting_record *q;
int subcommand;
int stop_request = 0;

j = open_communication_socket();
if (j)
    return j;
initialize_device_structures();

printf("Starting interactive mode:\n");
//printf("How many ovens are present? ");
//fgets(buffer,BUFFER_SIZE,stdin);
//j = atol(buffer);
//if (j>2 || j<1)
//    j = 1;
j = 3;
if (!rs485d_add_device(new_socket,LOGICAL_DEVICE_FIRST_OVEN,ELAN_PHYSICAL_DEVICE_FIRST_OVEN,R485D_DEVICE_CLASS_EOC6,EOC6_SESSION_OPTIONS, &server_addr,sizeof(server_addr)))
    printf("Could not add first logical device.");
//if (j==2)
if (!rs485d_add_device(new_socket,LOGICAL_DEVICE_SECOND_OVEN,ELAN_PHYSICAL_DEVICE_SECOND_OVEN,R485D_DEVICE_CLASS_EOC6,EOC6_SESSION_OPTIONS, &server_addr,sizeof(server_addr)))
    printf("Could not add second logical device.");
if (!rs485d_add_device(new_socket,LOGICAL_DEVICE_PMC,ELAN_PHYSICAL_DEVICE_PMC,R485D_DEVICE_CLASS_PMC,0, &server_addr,sizeof(server_addr)))
    printf("Could not add second logical device.");
elan_device_count = 3;
while (stop_request==0)
    {
    print_device_status_info();
    printf("\n\n Controlling device %d\n",interacting);
    printf("Select command: \n");
    printf("0) Refresh stats\n");
    printf("1) Exit program\n");
    printf("2) Swap oven under control\n");
    printf("3) Set setpoint\n");
    printf("4) Set cycle\n");
    printf("5) Stop\n");
    printf("6) Start with preheat\n");
    printf("7) Start without preheat\n");
    printf("8) Set element mask\n");
    printf("9) Set serial number\n");
    printf("A) Set low power mode\n");
    printf("Command: ");
    fgets(buffer,BUFFER_SIZE,stdin);
    q = &(device_setting[interacting]);
    switch (buffer[0])
        {
        case '0':
            {
            break;
            }    
        case '1':
            {
            stop_request = 1;
            break;
            }
        case '2':
            {
            interacting ^= 1;
            break;
            }
        case '3':
            {
            printf("New set point: ");
            fgets(buffer,BUFFER_SIZE,stdin);
            q->set_point = atol(buffer);
            j = do_setpoint(interacting);
            if (j!=R485D_SUCCESS)
                printf("!!!Failed to perform set point!!!\n");
            break;
            }
        case '4':
            {
            printf("New cycle: ");
            fgets(buffer,BUFFER_SIZE,stdin);
            q->cycle = atol(buffer);
            j = do_cycle(interacting);
            if (j!=R485D_SUCCESS)
                printf("!!!Failed to set cycle!!!\n");
            break;
            }
        case '5':
            {
            printf("Stopping... ");
            subcommand = EOC6_SUBCOMMAND_STOP;
            j=do_startstop(interacting,subcommand);
            if (j!=R485D_SUCCESS)
                printf("!!!Failed to perform stop!!!\n");
            else
                q->power_state = 1;
            break;        
            }
        case '6':
            {
            printf("Starting with preheat... ");
            subcommand = EOC6_SUBCOMMAND_START_WITH_PREHEAT;
            j=do_startstop(interacting,subcommand);
            if (j!=R485D_SUCCESS)
                printf("!!!Failed to perform start with preheat!!!\n");
            else
                q->power_state = 2;
            break;
            }
        case '7':
            {
            printf("Starting without preheat... ");
            subcommand = EOC6_SUBCOMMAND_START_WITHOUT_PREHEAT;
            j=do_startstop(interacting,subcommand);
            if (j!=R485D_SUCCESS)
                printf("!!!Failed to perform start with preheat!!!\n");
            else
                q->power_state = 3;
            break;
            }
        case '8':
            {
            printf("New digital mask: ");
            fgets(buffer,BUFFER_SIZE,stdin);
            q->digital_mask = atol(buffer);
            j=do_digital(interacting);
            if (j!=R485D_SUCCESS)
                printf("!!!Failed to perform digital mask!!!\n");
            break;        
            }
        case '9':
            {
            printf("New serial number: ");
            fgets(buffer,BUFFER_SIZE,stdin);
            j = strlen(buffer);
            if (j==0)
                break;
            if (j>13)
                j=13;
            j=do_set_serial(j,buffer);
            if (j!=R485D_WRITE_COMPLETE)
                printf("!!!Failed to set serial number!!!\n");
            break;        
            }
        case 'A':
            {
            printf("Low power mode (0=Normal, 1 = US, 2 = European, 3 = Display Only):  ");
            fgets(buffer,BUFFER_SIZE,stdin);
            j = atol(buffer);
            if (j<0 || j>3)
                break;
            j=do_sleep_mode(j);
            if (j!=R485D_WRITE_COMPLETE)
                printf("!!!Failed to perform sleep mode!!! %d\n",j);
            break;        
            }
        }
    }
return 0;
}
