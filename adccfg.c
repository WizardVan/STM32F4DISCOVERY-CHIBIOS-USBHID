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
#define ADC_GRP1_NUM_CHANNELS   6
#define ADC_GRP1_BUF_DEPTH      10
#include "ch.h"
#include "hal.h"
#include "adccfg.h"

static adcsample_t samples1[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];
uint32_t dataX;
uint32_t dataY;
/*
 * ADC streaming callback.
 */
static void adccallback(ADCDriver *adcp, adcsample_t *buffer, size_t n) {
   (void)adcp;
   (void)n;
   unsigned int i;
   uint32_t sum_x=0;
   uint32_t sum_y=0;
   //uint32_t vrefSum=0;
   //uint32_t tempSum=0;
   //if(n != ADC_GRP2_BUF_DEPTH/2) overflow++;
   for(i=0;i<ADC_GRP1_BUF_DEPTH/2;i++){
       sum_x=buffer[i*ADC_GRP1_NUM_CHANNELS+0];
       sum_y=buffer[i*ADC_GRP1_NUM_CHANNELS+1];
       sum_x=buffer[i*ADC_GRP1_NUM_CHANNELS+2];
       sum_y=buffer[i*ADC_GRP1_NUM_CHANNELS+3];
       sum_x=buffer[i*ADC_GRP1_NUM_CHANNELS+4];
       sum_y=buffer[i*ADC_GRP1_NUM_CHANNELS+5];       
   }
   dataX=sum_x/(ADC_GRP1_BUF_DEPTH/6);
   dataY=sum_y/(ADC_GRP1_BUF_DEPTH/6);   

}

static void adcerrorcallback(ADCDriver *adcp, adcerror_t err) {

  (void)adcp;
  (void)err;
}

/*
 * ADC conversion group.
 * Mode:        Continuous, 16 samples of 8 channels, SW triggered.
 * Channels:    IN11, IN12, IN11, IN12, IN11, IN12, Sensor, VRef.
 */
static const ADCConversionGroup adcgrpcfg1 = {
  TRUE,
  ADC_GRP1_NUM_CHANNELS,
  adccallback,
  adcerrorcallback,
  0,                        /* CR1 */
  ADC_CR2_SWSTART,          /* CR2 */
  ADC_SMPR1_SMP_AN12(ADC_SAMPLE_3) | ADC_SMPR1_SMP_AN11(ADC_SAMPLE_3),
  0,                        /* SMPR2 */
  ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS),
  0,
  ADC_SQR3_SQ6_N(ADC_CHANNEL_IN12)   | ADC_SQR3_SQ5_N(ADC_CHANNEL_IN11) |
  ADC_SQR3_SQ4_N(ADC_CHANNEL_IN12)   | ADC_SQR3_SQ3_N(ADC_CHANNEL_IN11) |
  ADC_SQR3_SQ2_N(ADC_CHANNEL_IN12)   | ADC_SQR3_SQ1_N(ADC_CHANNEL_IN11)
};

void myADCinit(void){

   palSetGroupMode(GPIOC, PAL_PORT_BIT(1) | PAL_PORT_BIT(2),0, PAL_MODE_INPUT_ANALOG);
  /*
   * Activates the ADC1 driver and the temperature sensor.
   */
   adcStart(&ADCD1, NULL);

  /*
   * Starts an ADC continuous conversion.
   */
   adcStartConversion(&ADCD1, &adcgrpcfg1, samples1, ADC_GRP1_BUF_DEPTH);
   chThdSleepMilliseconds(1000);

}
