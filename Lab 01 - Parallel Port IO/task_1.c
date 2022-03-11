//E/17/219 : Nawarathna K.G.I.S.
//E/17/212 : MORAIS K.W.G.A.N.D.
//E/17/230 : NISHANKAR S.

#include <stdio.h>      //include the standard input output library
#include <stdlib.h>     //include the standard library
#include <unistd.h>     //POSIX OS API, this is a sys call wrapper function
#include <sys/io.h>     // for basic parallel port functions
#include <time.h>       //for implementation in delay function in C

#define DATA_PORT 0x378         //base address of the parallel port

unsigned char data;         //8 bits to store values of status reg and data reg

void main(){

    //set the permission to access 7 bits from data port
    if (ioperm(DATA_PORT, 1, 1)){
        fprintf(stderr, "Access denied to %x\n", DATA_PORT), exit(1);
    }

    //to control the counter in the 7-segment display
    int counter = 0;

    //go for the infinity loop
    while(1){

        //data variable contains the bits inorder to display the correct number
        //data is in the decimal form

        if (counter==0){
            data = 63;
        }else if(counter==1){
            data = 6;
        }else if(counter==2){
            data = 91;
        }else if(counter==3){
            data = 79;
        }else if(counter==4){
            data = 102;
        }else if(counter==5){
            data = 109;
        }else if(counter==6){
            data = 125;
        }else if(counter==7){
            data = 7;
        }else if(counter==8){
            data = 127;
        }else if(counter==9){
            data = 111;
        }else if(counter==10){
            counter = 0;
            continue;
        }

        //write data to the output port
        outb(data, DATA_PORT);

        //place the delay
        sleep(1);

        //increment the counter
        counter++;

    }   
}
