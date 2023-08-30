/* Modbus Client

   Simple Modbus RTU client to test a Modbus link. To be used in conjunction with
   a Modbus RTU server.

   2022, ibeeto, Rudolf J Streif <rudolf.streif@ibeeto.com>

   compile with: cc `pkg-config --cflags --libs libmodbus` -o modbus-client modbus-client.c
*/

#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <modbus/modbus.h>
#include <errno.h>
#include <stdlib.h>
#include "modbus-register.h"


void usage (char *name) {
    fprintf(stderr, "Usage: %s -c <device> [-b <baud>] [-p <parity>] [-d <data bits>] [-s <stop bits>] [-l <loops>\n", name);
}


int main (int argc, char *argv[])
{
    char *device = NULL;
    int baud = 38400;
    char parity = 'N';
    int data = 8;
    int stop = 1;
    int loop = 5;
    int opt;
    int ret = EXIT_SUCCESS;

    // parse command line
    while ((opt = getopt(argc, argv, "c:b:p:d:s:l:")) != -1) {
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
        case 'l':
            loop = atoi(optarg);
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

    // establish Modbus context
    modbus_t *ctx = modbus_new_rtu(device, baud, parity, data, stop);
    if (!ctx) {
        ret = errno;
        fprintf(stderr, "%s: Failed to create Modbus context: %s.\n", argv[0], modbus_strerror(ret));
        exit(ret);
    }

    // enable debug
    //modbus_set_debug(ctx, TRUE);

    // connect
    if (modbus_connect(ctx) == -1) {
        ret = errno;
        fprintf(stderr, "%s: Unable to connect: %s.\n", argv[0], modbus_strerror(ret));
        modbus_free(ctx);
        exit(ret);
    }

    // set rs485 mode
    if (modbus_rtu_set_rts(ctx, MODBUS_RTU_RTS_UP) == -1) {
        ret = errno;
        fprintf(stderr, "%s: Unable to set serial mode: <%s>.\n", argv[0], modbus_strerror(ret));
        modbus_free(ctx);
        exit(ret);
    } 

    // set Modbus address of the server this client is talking to
    modbus_set_slave(ctx, SERVER_ADDRESS);

    // read data registers
    uint16_t reg[2];
    int num;
    int l = 0;
    while (l < loop) {
        l++;
        fprintf(stderr, "%s: Read/write loop: %d of %d\n", argv[0], l, loop);
        num = modbus_read_registers(ctx, 0x1, 2, reg);
        if (num != 2) {
            ret = errno;
            fprintf(stderr, "%s: Failed to read registers: %s\n", argv[0], modbus_strerror(ret));
        }
        else {
            fprintf(stderr, "%s: Registers read: [%04x, %04x]\n", argv[0], reg[0], reg[1]);
        }
        num = modbus_write_register(ctx, 0x3, l);
        if (num != 1) {
            ret = errno;
            fprintf(stderr, "%s: Failed to write register: %s\n", argv[0], modbus_strerror(ret));
        }
        num = modbus_read_registers(ctx, 0x3, 1, reg);
        if ((num != 1) && (reg[0]) != l) {
            ret = errno;
            fprintf(stderr, "%s: Failed to read register: %s\n", argv[0], modbus_strerror(ret));
        }
        else {
            fprintf(stderr, "%s: Register read: [%04x]\n", argv[0], reg[0]);
        }
    }

    // send exit message to server
    num = modbus_write_register(ctx, 0x0, MESSAGE_EXIT);
    if (num != 1) {
        ret = errno;
        fprintf(stderr, "%s: Failed to write register: %s\n", argv[0], modbus_strerror(ret));
    }

    // cleanup
    modbus_close(ctx);
    modbus_free(ctx);
    exit(ret);
}