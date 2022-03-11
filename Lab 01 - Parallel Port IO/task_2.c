//E/17/219 : Nawarathna K.G.I.S.
//E/17/212 : MORAIS K.W.G.A.N.D.
//E/17/230 : NISHANKAR S.

#include <stdio.h>      //include the standard input output library
#include <stdlib.h>     //include the standard library
#include <unistd.h>     //POSIX OS API, this is a sys call wrapper function
#include <sys/io.h>     //for ioperm function

#define DATA_PORT 0x378             //base address of the parallel port
#define STATUS_PORT DATA_PORT+1     //status port address of the parallel port
#define CONTROL_PORT DATA_PORT+2    //control port address of the parallel port

//8 bits variables to store values of status reg and data reg
unsigned char status = 0;
unsigned char data;         

void main(){

    //set the permission to access 8 bits from data port
    if (ioperm(DATA_PORT, 1, 1)){
        fprintf(stderr, "Access denied to %x\n", DATA_PORT), exit(1);
    }

    //set the permission to access 8 bits from status port
    if (ioperm(STATUS_PORT, 1, 1)){
        fprintf(stderr, "Access denied to %x\n", STATUS_PORT), exit(1);
    }

    int counter = 0;                        //to manage the number in the 7 segment display
    unsigned char pre_S6 = 0;               //to manage the value in the S6 bit(previous value in the S6)
    unsigned char pre_S7 = 1;               //to manage the value in the S7 bit(previous value in the S7)

    while(1){

        //get the status port bits
        status = inb(STATUS_PORT);

        //if the 6th bit is changed
        if((status>>6) & 1){
            //get the rising edge
            if(pre_S6==0){
                //increment the counter
                counter++;
                //set the previous S6 value to high
                pre_S6 = 1;
            }
        }else{
            //if the button is not pressed
            //set the previous S6 value to low 
            pre_S6 = 0;
        }

        //if the 7th bit is changed
        if(~(status>>7) & 1){
            //get the falling edge
            if(pre_S7 ==1){
                //decrement the counter
                counter--;
                //set the previous S7 value to low
                pre_S7  = 0;
            }
        }else{
            //if the button is not pressed
            //set the previous S6 value to high 
            pre_S7  = 1;
        }

        //values related to the up-down counter
        if(counter==-1){
            counter = 9;
        }else if (counter==0){
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
        }

        //write counter value to the output port
        outb(data, DATA_PORT);

    }
}