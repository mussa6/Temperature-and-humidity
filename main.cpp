/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "DFRobot_RGBLCD.h"
#include <HTS221Sensor.h>
#include <algorithm>
#include <HTS221_driver.h>
#include <cstdio>
#define WAIT_TIME_MS 1000 
DigitalOut led1(LED1);
DigitalIn button(A2, PullDown);

DevI2C i2c(PB_11, PB_10);
DFRobot_RGBLCD lcd(16, 2, D14, D15); 
HTS221Sensor hts221(&i2c);

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


int main()
{
    lcd.init(); 
    //lcd.printf("Number: %i", 42);
    button.mode(PullDown);
    //lcd.setRGB(0, 255, 0);

    //printf("This is the bare metal blinky example running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
// Init HTS221 with default params (NULL)
  if (hts221.init(NULL) != 0) {
    printf("Failed to initialize HTS221 device!\n");
  }

  // Power up the HTS221 device
  if (hts221.enable() != 0) {
    printf("Failed to power up HTS221 device!\n");
  }
    bool button_ = false;

    while (true) {
        float hum;
        float temp;
        int count=0;

        if (button.read() == true) {
            count++;
            button_ = !button_;
        }

        if (button_) {
            printf("Humidity: %.1f %% ", hum);
            lcd.printf("Humidity: %.1f %% ", hum);
            lcd.setCursor(0,0);
            hum=20;
            lcd.setRGB(255-(int)(map(hum, 0, 100, 0, 255)), 255-(int)(map(hum, 0, 1, 0, 255)), 255);
        }
        else {
            printf("Temperature: %.1f C\n", temp);
            lcd.printf("Temp: %.2f C\n", temp);
            lcd.setCursor(0,0);
            //temp = ;
            if( temp < 20)
            {
             lcd.setRGB(0, 72, 255);
            }
            else if ( temp >= 20 && temp < 24)
            {
             lcd.setRGB(255, 94, 0);
            }
            else if ( temp >= 24)
            {
                lcd.setRGB(255, 0, 0);
            }
          
        }
 
    

    hts221.get_humidity(&hum);
    hts221.get_temperature(&temp);

    
    

    led1 = !led1;

    thread_sleep_for(1000);
  }
}

