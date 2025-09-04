// Copyright 2025 Michael V. Schaefer
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <Arduino.h>
#include <freertos/task.h>
#include "vfb.h"
#include "profiler.h"
#include "app.h"

// Task handles
TaskHandle_t core0_100Hz_hndl;
TaskHandle_t core0_10Hz_hndl;
TaskHandle_t core0_1Hz_hndl;
TaskHandle_t core0_idle_hndl;
TaskHandle_t core1_100Hz_hndl;
TaskHandle_t core1_10Hz_hndl;
TaskHandle_t core1_1Hz_hndl;
TaskHandle_t core1_idle_hndl;

// Task functions
void core0_100Hz_task(void *pvParameters)
{
  for (;;)
  {
    unsigned long t_entry = esp_timer_get_time();
    vfb_step_100Hz();
    unsigned long t_exit = esp_timer_get_time();

    Profiler_markEntry(0, 0, t_entry);
    Profiler_markExit(0, 0, t_exit);

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void core0_10Hz_task(void *pvParameters)
{
  for (;;)
  {
    unsigned long t_entry = esp_timer_get_time();
    vfb_step_10Hz();
    unsigned long t_exit = esp_timer_get_time();

    Profiler_markEntry(0, 1, t_entry);
    Profiler_markExit(0, 1, t_exit);
    Profiler_computeStats();

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void core0_1Hz_task(void *pvParameters)
{
  for (;;)
  {
    unsigned long t_entry = esp_timer_get_time();
    vfb_step_1Hz();
    unsigned long t_exit = esp_timer_get_time();

    Profiler_markEntry(0, 2, t_entry);
    Profiler_markExit(0, 2, t_exit);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void core1_100Hz_task(void *pvParameters)
{
  for (;;)
  {
    unsigned long t_entry = esp_timer_get_time();
    vfb_step_100Hz_core1();
    unsigned long t_exit = esp_timer_get_time();

    Profiler_markEntry(1, 0, t_entry);
    Profiler_markExit(1, 0, t_exit);

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void core1_10Hz_task(void *pvParameters)
{
  for (;;)
  {
    unsigned long t_entry = esp_timer_get_time();
    vfb_step_10Hz_core1();
    unsigned long t_exit = esp_timer_get_time();

    Profiler_markEntry(1, 1, t_entry);
    Profiler_markExit(1, 1, t_exit);

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void core1_1Hz_task(void *pvParameters)
{
  for (;;)
  {
    unsigned long t_entry = esp_timer_get_time();
    vfb_step_1Hz_core1();
    unsigned long t_exit = esp_timer_get_time();

    Profiler_markEntry(1, 2, t_entry);
    Profiler_markExit(1, 2, t_exit);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup()
{
  vfb_init();
  app_init();
  Profiler_init();

  Serial.print("misc::sched::configuration...\n");
  // Create tasks
  xTaskCreatePinnedToCore(core0_100Hz_task, "core0_100Hz", 1024 * 16, NULL, 3, &core0_100Hz_hndl, 0);
  xTaskCreatePinnedToCore(core0_10Hz_task, "core0_10Hz", 1024 * 16, NULL, 2, &core0_10Hz_hndl, 0);
  xTaskCreatePinnedToCore(core0_1Hz_task, "core0_1Hz", 1024 * 16, NULL, 1, &core0_1Hz_hndl, 0);
  // xTaskCreatePinnedToCore(core0_idle_task, "idle_monitor_core0", 2048, NULL, tskIDLE_PRIORITY + 1, &core0_idle_hndl, 0);
  Serial.println("misc::sched::core0 [100|10|1]hz.");

  xTaskCreatePinnedToCore(core1_100Hz_task, "core1_100Hz", 1024 * 16, NULL, 3, &core1_100Hz_hndl, 1);
  xTaskCreatePinnedToCore(core1_10Hz_task, "core1_10Hz", 1024 * 16, NULL, 2, &core1_10Hz_hndl, 1);
  xTaskCreatePinnedToCore(core1_1Hz_task, "core1_1Hz", 1024 * 16, NULL, 1, &core1_1Hz_hndl, 1);
  // xTaskCreatePinnedToCore(core1_idle_task, "idle_monitor_core1", 2048, NULL, tskIDLE_PRIORITY + 1, &core1_idle_hndl, 1);
  Serial.println("misc::sched::core1 [100|10|1]hz.");

  /*
  vTaskStartScheduler();
  */

  Serial.print("misc::sched::configured.\n");

  Serial.println();
  Serial.println("...flatball-imu...\n");
  Serial.println();
}

void loop()
{
}
