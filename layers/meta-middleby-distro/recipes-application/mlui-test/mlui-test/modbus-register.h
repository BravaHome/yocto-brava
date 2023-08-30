/* Modbus Register

   Register definitions shared between modbuse-server and modbus-client.

   2022, ibeeto, Rudolf J Streif <rudolf.streif@ibeeto.com>

*/

#define SERVER_ADDRESS        0x3    // Modbus server address

#define REGISTER_COUNT         10    // Number of registers
#define REGISTER_MESSAGE      0x0    // Messages sent from client
#define REGISTER_DATA_1       0x1    // Data register
#define REGISTER_DATA_2       0x2    // Data register
#define REGISTER_DATA_3       0x3    // Data register
#define REGISTER_DATA_4       0x4    // Data register

#define MESSAGE_NONE          0x0
#define MESSAGE_EXIT       0xffff

#define DATA_DEFAULT_1     0xdead
#define DATA_DEFAULT_2     0xbeef