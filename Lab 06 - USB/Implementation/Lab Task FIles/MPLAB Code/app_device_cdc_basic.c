#include "system.h"

#include <stdint.h>
#include <string.h>
#include <stddef.h>

#include "usb.h"

#include "app_led_usb_status.h"
#include "app_device_cdc_basic.h"
#include "usb_config.h"

static bool buttonPressed;
static char buttonMessage[] = "Button pressed.\r\n";
static uint8_t readBuffer[CDC_DATA_OUT_EP_SIZE];
static uint8_t writeBuffer[CDC_DATA_IN_EP_SIZE];
static bool ready = false;
static uint8_t bIndex = 2;

void APP_DeviceCDCBasicDemoInitialize()
{   
    line_coding.bCharFormat = 0;
    line_coding.bDataBits = 8;
    line_coding.bParityType = 0;
    line_coding.dwDTERate = 9600;

    buttonPressed = false;
}


void APP_DeviceCDCBasicDemoTasks()
{

    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        return;
    }

    if( USBIsDeviceSuspended()== true )
    {
        return;
    }
        
    if(BUTTON_IsPressed(BUTTON_DEVICE_CDC_BASIC_DEMO) == true)
    {

        if(buttonPressed == false)
        {

            if(mUSBUSARTIsTxTrfReady() == true)
            {
                putrsUSBUSART(buttonMessage);
                buttonPressed = true;
            }
        }
    }
    else
    {

        buttonPressed = false;
    }

    if( USBUSARTIsTxTrfReady() == true)
    {
        uint8_t i;
        uint8_t numBytesRead;

        if (bIndex < 3) {
            memset(writeBuffer, 0x00, CDC_DATA_IN_EP_SIZE);
            memcpy(writeBuffer, "\r\n", 2*sizeof(uint8_t));
        }

        numBytesRead = getsUSBUSART(readBuffer, sizeof(readBuffer));

        for(i=0; i<numBytesRead; i++)
        {   
            //if the current character is the new line character
            if (readBuffer[i] == 0x0D || readBuffer[i] == 0x0A)
            {   
                //data is ready to be written
                ready = true;
                break;
            }

            //for all the lowercase characters
            if(readBuffer[i] > 96 && readBuffer[i] < 123)
            {   
                //make the character lower
                writeBuffer[bIndex] = readBuffer[i] - 32;
            }
            else
            {   
                //for every other character, write the character
                writeBuffer[bIndex] = readBuffer[i];
            }

            //increment the index of the character index
            bIndex++;
            ready = false;
            
        }

        //sending back the data after converting them to uppercase
        if(ready == true && numBytesRead > 0)
        {

            memcpy(&writeBuffer[bIndex+1], "\r\n\n", 3*sizeof(uint8_t));
            putUSBUSART(writeBuffer, bIndex+4);
            bIndex = 2;
        }

        //if the ready signal is not set, the newline character is not received
        //in that case, put that to the writebuffer
        else if (ready == false && numBytesRead > 0)
        {
            putUSBUSART(writeBuffer, numBytesRead);
        }
        
    }

    CDCTxService();
}