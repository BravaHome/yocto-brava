#ifndef __r485lib_defined
#define __r485lib_defined



// Routine to get a high precision clock time
// Returns nanoseconds since the first call to get_high_precision_tick
int64_t get_high_precision_tick();


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
int read_full(int handle,char *buffer,int size,int *actual_bytes_read, void *server_addr, int *server_length);

// Routine to write to a datagram socket while being resilient to signals and interruptions
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
int send_full(int handle,char *buffer,int size,int *actual_bytes_written, void *client_addr, int client_length, int wait_mode);

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
int poll_for_read_without_select(int handle,int timeout);

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
int poll_for_read(int handle,int timeout);

// Fluches the input socket stream
// This version does not use select
// !!Warning!! This routine will fail if datagrams are larger than 64K
//
// Parameters:
// handle is the socket handle
void flush_input_without_select(int handle);

// Fluches the input socket stream
// !!Warning!! This routine will fail if datagrams are larger than 64K
//
// Parameters:
// handle is the socket handle
void flush_input(int handle);

uint16_t crc16_update(uint16_t crc, uint8_t a);

uint16_t crc16(uint8_t* data, uint8_t data_size);

void prepare_request(uint8_t addr, uint8_t type, uint32_t msgData, uint8_t *buffer);

uint32_t fetch_reply_data(uint8_t *buffer);


#endif
