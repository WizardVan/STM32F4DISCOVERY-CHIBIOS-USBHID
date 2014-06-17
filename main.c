/* 
    USB-HID Gamepad for ChibiOS/RT
    Copyright (C) 2014, +inf Wenzheng Xu.
    
    EMAIL: wx330@nyu.edu
    
    This piece of code is FREE SOFTWARE and is released under the terms
    of the GNU General Public License <http://www.gnu.org/licenses/>
*/

/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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


