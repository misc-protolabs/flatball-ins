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


/**
   This sketch collects scheduling overhead and CPU Idle Sleep information.
   A task is invoked every 10 milliseconds for 10 seconds.
   CPU statistics are collected

   Compile and run once with _TASK_SLEEP_ON_IDLE_RUN enabled, then with _TASK_SLEEP_ON_IDLE_RUN disabled.
   Compare the results.
*/

#define _TASK_ESP8266_DLY_THRESHOLD 50L
#define _TASK_ESP32_DLY_THRESHOLD 40L

#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_TIMECRITICAL
#include <TaskScheduler.h>

Scheduler ts;

// Callback methods prototypes
void Count();
bool tOn(); void tOff();

// Tasks
Task c(10, TASK_FOREVER, &Count, &ts);
Task t(10000, TASK_ONCE, NULL, &ts, true, &tOn, &tOff);


volatile unsigned long c1, c2;
bool tOn() {
  c1 = 0;
  c2 = 0;
  c.enable();

  return true;
}

void tOff() {
  c.disable();
  unsigned long cpuTot = ts.getCpuLoadTotal();
  unsigned long cpuCyc = ts.getCpuLoadCycle();
  unsigned long cpuIdl = ts.getCpuLoadIdle();

  Serial.print("Loop counts c1="); Serial.println(c1);
  Serial.print("Task counts c2="); Serial.println(c2);
  Serial.print("Total CPU time="); Serial.print(cpuTot); Serial.println(" micros");
  Serial.print("Scheduling Overhead CPU time="); Serial.print(cpuCyc); Serial.println(" micros");
  Serial.print("Idle Sleep CPU time="); Serial.print(cpuIdl); Serial.println(" micros");
  Serial.print("Productive work CPU time="); Serial.print(cpuTot - cpuIdl - cpuCyc); Serial.println(" micros");
  Serial.println();

  float idle = (float)cpuIdl / (float)cpuTot * 100;
  Serial.print("CPU Idle Sleep "); Serial.print(idle); Serial.println(" % of time.");

  float prod = (float)(cpuIdl + cpuCyc) / (float)cpuTot * 100;
  Serial.print("Productive work (not idle, not scheduling)"); Serial.print(100.00 - prod); Serial.println(" % of time.");

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println("CPU Time Measurement");
  Serial.println("Start");

  ts.startNow();
  t.delay();
  ts.cpuLoadReset();
}

void Count() {
  c2++;  // number of task callback invocations

  // Try different delay intervals to see CPU statistics change
  //  delay(1);
  //  delay(5);
  //  delay(10);
  //  delay(20);
}


void loop() {
  // put your main code here, to run repeatedly:
  ts.execute();
  c1++;  // number of loop() cycles
}
