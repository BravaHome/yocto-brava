/* Modbus Server

   Simple Modbus RTU server to test a Modbus link. To be used in conjunction with
   a Modbus RTU client.

   2022, ibeeto, Rudolf J Streif <rudolf.streif@ibeeto.com>

   compile with: cc `pkg-config --cflags --libs libmodbus` -o modbus-server modbus-server.c
*/

#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <stdlib.h>
#include "modbus-register.h"


void usage (char *name) {
    fprintf(stderr, "Usage: %s -c <device> [-b <baud>] [-p <parity>] [-d <data bits>] [-s <stop bits>]\n", name);
}


int main (int argc, char *argv[])
{
    char *device = NULL;
    int baud = 38400;
    char parity = 'N';
    int data = 8;
    int stop = 1;
    int opt;
    int ret = EXIT_SUCCESS;

    // parse command line
    while ((opt = getopt(argc, argv, "c:b:p:d:s:")) != -1) {
        switch (opt) {
        case 'c':
            device = optarg;
            break;
        case 'b':
            baud = atoi(optarg);
            break;
        case 'p':
            parity = atoi(optarg);
            break;
        case 'd':
            data = atoi(optarg);
            break;
        case 's':
            stop = atoi(optarg);
        default:
            usage(argv[0]);
            exit(EXIT_FAILURE);
            break;
        }
    }

    // we need at least the serial device
    if (device == NULL) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // setup Modbus mapping
    modbus_mapping_t *mb_map = modbus_mapping_new(0, 0, REGISTER_COUNT, 0);
    if (mb_map == NULL) {
        fprintf(stderr, "%s: Insufficient memory to allocate Modbus mapping.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // set data registers
    mb_map->tab_registers[REGISTER_DATA_1] = DATA_DEFAULT_1;
    mb_map->tab_registers[REGISTER_DATA_2] = DATA_DEFAULT_2;

    // establish Modbus context
    modbus_t *ctx = modbus_new_rtu(device, baud, parity, data, stop);
    if (!ctx) {
        ret = errno;
        fprintf(stderr, "%s: Failed to create Modbus context: %s.\n", argv[0], modbus_strerror(ret));
        exit(ret);
    }

    // enable debug
    //modbus_set_debug(ctx, TRUE);

    // set Modbus address of this server
    modbus_set_slave(ctx, SERVER_ADDRESS);

    // connect
    if (modbus_connect(ctx) == -1) {
        ret = errno;
        fprintf(stderr, "%s: Unable to connect: %s.\n", argv[0], modbus_strerror(ret));
        modbus_free(ctx);
        modbus_mapping_free(mb_map);
        exit(ret);
    }

    // set rs485 mode
    if (modbus_rtu_set_rts(ctx, MODBUS_RTU_RTS_UP) == -1) {
        ret = errno;
        fprintf(stderr, "%s: Unable to set serial mode: %s.\n", argv[0], modbus_strerror(ret));
        modbus_free(ctx);
        exit(ret);
    } 

    // request buffer
    uint8_t req[MODBUS_RTU_MAX_ADU_LENGTH];
    int len;
    uint16_t msg = 0;

    // receive loop
    int run = 1;
    while(run) {

        // listen for incoming request
        len = modbus_receive(ctx, req);
        if (len == -1) {
            ret = errno;
            fprintf(stderr, "%s: Receive error: %s.\n", argv[0], modbus_strerror(ret));
            continue;
        }

        // respond to request
        len = modbus_reply(ctx, req, len, mb_map);
        if (len == -1) {
            ret = errno;
            fprintf(stderr, "%s: Reply error: %s.\n", argv[0], modbus_strerror(ret));
            continue;
        }

        // check message register
        if ((msg = mb_map->tab_registers[REGISTER_MESSAGE]) != MESSAGE_NONE) {
            switch (msg) {
                case MESSAGE_EXIT:
                    fprintf(stdout, "%s: Received exit message.\n", argv[0]);
                    run = 0;
                    break;

                default:
                    fprintf(stdout, "%s: Received unknown message: %04x", argv[0], msg);
                    break;
            }
        }
    }

    // cleanup
    modbus_close(ctx);
    modbus_free(ctx);
    modbus_mapping_free(mb_map);
    exit(EXIT_SUCCESS);
}