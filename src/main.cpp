/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"

#include "GPIO.h"

#define LOG_TAG "MAIN"

extern "C" void app_main(void)
{
  
  gpio_num_t myPin{GPIO_NUM_2};
  GPIO::output myLed(myPin, false, GPIO::pullUpDownConfig::PULLUP);

  gpio_num_t mySensorPin{GPIO_NUM_27};
  GPIO::input mySensor{mySensorPin, true, GPIO::pullUpDownConfig::PULLUP};

  bool sensorValue = false;

  while(true) {
    sensorValue = mySensor.state();
    myLed.set(sensorValue);

    vTaskDelay(50); // delay for X ticks
  }

}
