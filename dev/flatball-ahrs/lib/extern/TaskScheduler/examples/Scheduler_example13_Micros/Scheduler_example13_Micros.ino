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
 * TaskScheduler Test of microsecond scheduling resolution
 *
 * Task 1 runs starting with 211 microseconds intervals, doubling the interval on every iteration 
 * until it wraps when interval reaches about 72 minutes mark
 * 
 * Task 2 provides heartbeat at a steady 5 seconds intervals
 *
 */
 
#define _TASK_MICRO_RES
#include <TaskScheduler.h>

#define T1_INIT (211L)

Scheduler runner;
// Callback methods prototypes
void t1Callback();
void t1OnDisable();
void t2Callback();


unsigned long t1_interval = T1_INIT;

// Tasks
Task t1(t1_interval, 1, &t1Callback, &runner, true, NULL, &t1OnDisable);      //adding task to the chain on creation
Task t2(5 * TASK_SECOND, TASK_FOREVER, &t2Callback, &runner, true);  //adding task to the chain on creation



void t1Callback() {
  unsigned long t = micros();
  Serial.print("t1: ");
  Serial.println(t);
}

void t1OnDisable() {
  t1_interval += t1_interval; 
  if (t1_interval < T1_INIT) t1_interval = T1_INIT;
  t1.setInterval(t1_interval);
  t1.restartDelayed();
}

void t2Callback() {
  unsigned long t = micros();
  Serial.print("t2: ");
  Serial.print(t);
  Serial.println(" heartbeat");
}


void setup () {
  Serial.begin(115200);
  Serial.println("Scheduler TEST Microsecond Resolution");
  
  Serial.println("5 seconds delay");
  delay(5000);
   
  runner.startNow(); // This creates a new scheduling starting point for all ACTIVE tasks. 
                      // PLEASE NOTE - THIS METHOD DOES NOT ACTIVATE TASKS, JUST RESETS THE START TIME
  t1.delay();        // Tasks which need to start delayed, need to be delayed again after startNow();

// Alternatively, tasks should be just enabled at the bottom of setup() method
//  runner.enableAll();    
//  t1.delay();
}


void loop () {
  runner.execute();
}
