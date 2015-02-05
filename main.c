/* 
    USB-HID Gamepad for ChibiOS/RT
    Copyright (C) 2014, +inf Wenzheng Xu.
    
    EMAIL: wx330@nyu.edu
    
    This piece of code is FREE SOFTWARE and is released
    under the Apache License, Version 2.0 (the "License");
*/

/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
        http://www.apache.org/licenses/LICENSE-2.0
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/


#include "ch.h"
#include "hal.h"

#include "usb_hid.h"
#include "usbcfg.h"
#include "adccfg.h"


// Possible Threads

/*
 * Red LED blinker thread, times are in milliseconds.
 */
/*static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {
  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palClearPad(GPIOD, 15);
    chThdSleepMilliseconds(500);
    palSetPad(GPIOD, 15);
    chThdSleepMilliseconds(500);
  }
  return (msg_t)0;
}
*/

// Main

int main(void) {
    int16_t tempdata;
    uint16_t temp;
    bool JoyCal;
    uint16_t initX;
    uint16_t initY;
	halInit();
	chSysInit();
	usbInitState=0;
	//usbStart(&USBD1, &usbcfg);
	usbDisconnectBus(&USBD1);
    chThdSleepMilliseconds(1000);
    usbStart(&USBD1, &usbcfg);
    usbConnectBus(&USBD1);
    JoyCal=0;
    initX=0;
    initY=0;
	/*
	 * Creates the blinker thread.
	 */
	/* 
	chThdSleepMilliseconds(3000);
    usbDisconnectBus(&USBD1);
    usbStop(&USBD1);
    chThdSleepMilliseconds(1500);
    usbStart(&USBD1, &usbcfg);
    usbConnectBus(&USBD1);
    */
    
    /*
	chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
    */
	/*
	 * Normal main() thread activity, in this demo it does nothing except
	 * sleeping in a loop and check the button state.
	 */
      /*
   * Setting up analog inputs used by the demo.
   */
   myADCinit();

   palSetGroupMode(GPIOA, PAL_PORT_BIT(1) | PAL_PORT_BIT(2)| PAL_PORT_BIT(3)| PAL_PORT_BIT(4)| PAL_PORT_BIT(5)| PAL_PORT_BIT(6)| PAL_PORT_BIT(7),0, PAL_MODE_INPUT);

	//hid_transmit(&USBD1);
	while (TRUE) {
		chThdSleepMilliseconds(50);
		if(usbInitState==1)
		{
		    if(JoyCal==0)
		    {
		        initX=dataX;
		        initY=dataY;
		        JoyCal=1;
		    }
		    else if(JoyCal==1)
		    {
		        tempdata=((int16_t)dataX-(int16_t)initX)/15;
		        if (tempdata > 128)hid_in_data.x=127;
		        if (tempdata < -128)hid_in_data.x=-127;
		        if (tempdata > -128 && tempdata < 128) hid_in_data.x=(int8_t)tempdata;
	            tempdata=((int16_t)dataY-(int16_t)initY)/15;
                if (tempdata > 128)hid_in_data.y=-127;
		        if (tempdata < -128)hid_in_data.y=127;
		        if (tempdata > -128 && tempdata < 128) hid_in_data.y=-((int8_t)tempdata);
                temp=palReadPort(GPIOA);	        
                hid_in_data.button=(uint8_t)(((~temp) >> 1) & 0xFF);
		        hid_transmit(&USBD1);
		    }
		    
		}

	}

}


