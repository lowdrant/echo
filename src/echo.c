/** echo.c
 * Echos strings sent over serial on Raspberry Pi
 * Options:
 *     -h --help : help dialog
 *     -b        : specify baud rate   <int>
 *     -p        : specify serial port <char *>
 *
 * MIT License
 * Author: Marion Anderson 2017-09-07
 */
#include <wiringPi.h>      // pin control
#include <wiringSerial.h>  // serial
#include <stdio.h>
#include <string.h>        // portname string
#include <errno.h>         // error handling
#include <unistd.h>        // file checking
#include <stdlib.h>
#include <linux/limits.h>

struct serdata {
    int baud;
    char *port;
    int fd;
};

void input_parser(int, char**, struct serdata*);
void help_msg(void);   // -h and --help handler
void die(int);         // error handler


int main(int argc, char *argv[])
{
    // Parsing Input Args:
    struct serdata ser = {.baud=9600, .port=malloc(PATH_MAX)};
    strncpy(ser.port, "/dev/ttyAMA0", 13);
    input_parser(argc, argv, &ser);

    // User Error Handling:
    // check if baud rate is valid
    switch (ser.baud) {
        case 110:
        case 300:
        case 600:
        case 1200:
        case 2400:
        case 4800:
        case 9600:
        case 14400:
        case 19200:
        case 28800:
        case 38400:
        case 56000:
        case 57600:
        case 115200:
            break;
        default:
            die(2);
    }
    // check if serial port exists
    if (access(ser.port, F_OK) == -1) {
        die(3);
    }

    // Hardware Setup:
    wiringPiSetup();
    ser.fd = serialOpen(ser.port, ser.baud);

    // Execution:
    int datind = 0;   // index of buffer to write char
    //char serbuf;       // buffer of incoming serial byte
    char serdat[100];  // current string of chars sent over UART
    while (1) {
        // Reading data
        datind = 0;  // reset data index
        while (serialDataAvail(ser.fd)) {
            serdat[datind] = serialGetchar(ser.fd);
            datind++;
        }
        serdat[datind] = '\0';  // null terminate string

        delay(1);  // wait 1ms

        // echo data
        printf("%s\n", serdat);
        serialPrintf(ser.fd, "%s\n", serdat);
    }

    // End Program:
    serialClose(ser.fd);
    return 0;
}


/** input_parser
 * Parses input args specified in top-level comment.
 * Updates input serial data struct
 *
 * argc  <int>              number of input args to program
 * argv  <char**>           str array of input args
 * ser   <struct serdata*> struct of serial data to be updated
 * 
 * Void Function
 */
void input_parser(int argc, char *argv[], struct serdata *ser)
{
    switch (argc) {
        case 1:  // no input args
            break;
        case 2:  // help dialog
            if (strncmp(argv[1], "--help", 7) == 0
                || strncmp(argv[1], "-h", 3) == 0) {
                help_msg();
            } else {
                die(1);
            }
            break;
        case 3:  // -b or -p args
            // baudrate
            if (strncmp(argv[1], "-b", 3) == 0) {
                ser->baud = atoi(argv[2]);

            // port
            } else if (strncmp(argv[1], "-p", 3) == 0) {
                if (!strncpy(ser->port, argv[2], PATH_MAX)) die(4);

            // unknown arg
            } else {
                die(1);
            }
            break;
        case 5:  // -b & -p args
            // identifying first arg
            if (strncmp(argv[1], "-b", 3) == 0) {
                ser->baud = atoi(argv[2]);
            } else if (strncmp(argv[1], "-p", 3) == 0) {
                if (!strncpy(ser->port, argv[2], PATH_MAX)) die(4);
            } else {
                die(1);
            }

            // identifying second arg
            if (strncmp(argv[3], "-b", 3) == 0) {
                ser->baud = atoi(argv[4]);
            } else if (strncmp(argv[3], "-p", 3) == 0) {
                if (!strncpy(ser->port, argv[4], PATH_MAX)) die(4);
            } else {
                die(1);
            }
            break;
        default:
            die(1);
    }

    // informing user of settings
    printf("Baudrate set to %d.\n", ser->baud);
    printf("Serial port set to %s.\n", ser->port);
}


/** help_msg
 * Displays help dialog for this program
 */
void help_msg()
{
    printf("This function echos input sent over a serial connection.\n");
    printf("Usage: echo [-b baudrate] [-p port]\n");
    printf("Options\n");
    printf("\t--help\t\tDisplay this information.\n");
    printf("\t-h\t\tSame as --help.\n\n");
    printf("\t-b baudrate\tThe baudrate of the serial interface.\n");
    printf("\t-p port\t\tThe port for serial communication.\n\n");
}


/** die
 * Internal error handling
 * Prints out appropriate error message
 *
 * errnum  <int>  number identifying the type of internal error
 *          1 - unrecognized input arg
 *          2 - baudrate
 *          3 - serial port
 *          4 - mem error
 *
 * Void Function
 */
void die(int errnum)
{
    char msg[40];
    switch (errnum) {
        case 1:
            strncpy(msg, "unrecognized input arg(s)", 40);
            break;
        case 2:
            strncpy(msg, "unrecognized baudrate", 40);
            break;
        case 3:
            strncpy(msg, "unable to open serial port", 40);
            break;
        case 4:
            strncpy(msg, "memory error", 40);
            break;
        default:
            printf("die: default\n");
            strncpy(msg, "unrecognized internal error value", 40);
    }

    if (errno) {
        perror(msg);
    } else {
        printf("ERROR: %s\n", msg);
    }

    exit(-1);
}
