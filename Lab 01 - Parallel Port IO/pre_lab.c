#include <stdio.h>      //include the standard input output library
#include <stdlib.h>     //include the standard library
#include <unistd.h>     //POSIX OS API, this is a sys call wrapper function
#include <sys/io.h>     // What is this library???


#define DATA_PORT 0x378         //base address of the parallel port
#define STATUS_PORT DATA_PORT+1     //status port address
#define CONTROL_PORT DATA_PORT+2    //control port address

unsigned char status, data;         //8 bits to store values of status reg and data reg

void main(){

    //ioperm - set port input/output permissions
    //int ioperm(unsigned long from, unsigned long num, int turn_on)
    //ioperm() sets the port access permission bits for the calling thread for num bits starting from port address from
    //If turn_on is nonzero, then permission for the specified bits is enabled; otherwise it is disabled
    // On success, zero is returned.  On error, -1 is returned, and errno is set to indicate the error

    //set the permission to access 1 bit from data port
    if (ioperm(DATA_PORT, 1, 1)){
        fprintf(stderr, "Access denied to %x\n", DATA_PORT), exit(1);
    }

    //set the permission to access 1 bit from status port
    if (ioperm(STATUS_PORT, 1, 1)){
        fprintf(stderr, "Access denied to %x\n", STATUS_PORT), exit(1);
    }

    //read what is in the status port
    status = inb(STATUS_PORT);

    //get the data as the status
    data = status;

    //write data to the output port
    outb(data, DATA_PORT);
}
