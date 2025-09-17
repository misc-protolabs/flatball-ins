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
 *  TaskScheduler Test
 *  Illustration of use of Time Critical Information
 *
 *  Task1 runs every 1 second indefinitely
 *  On each run it reports how delayed the invokation of the callback method was,
 *  and what was the scheduling overun.
 *  Each run task 1 is dealyed randomly for up to 2 seconds, thus simulating scheduling overrun
 */
 
 #define _TASK_TIMECRITICAL
 #define _TASK_SLEEP_ON_IDLE_RUN
#include <TaskScheduler.h>

// Callback methods prototypes
void t1Callback();


//Tasks
Task t1(1000, -1, &t1Callback);

Scheduler runner;


void t1Callback() {
    Serial.print(millis());
    Serial.print(": overrun = ");
    Serial.print(t1.getOverrun());
    Serial.print(", start delayed by ");
    Serial.println(t1.getStartDelay());
    
    int i = random(2000); 
    Serial.print("Delaying for "); Serial.println(i);
    delay(i);
}

void setup () {
  Serial.begin(115200);
  Serial.println("Scheduler TimeCritical TEST");
  
  runner.init();
  Serial.println("Initialized scheduler");
  
  runner.addTask(t1);
  Serial.println("added t1. Waiting for 5 seconds.");
  
  delay(5000);
  
  t1.enable();

  Serial.println("Enabled t1");
}


void loop () {
  runner.execute();
}
