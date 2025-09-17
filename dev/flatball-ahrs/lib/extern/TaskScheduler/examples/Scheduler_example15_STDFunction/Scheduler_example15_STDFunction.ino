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
 * TaskScheduler Test sketch - Showing how to use std::function
 * to get acces to variables from within the task callback function 
 * 
 * Support for std::function is only available for ESP8266 architecture
 */
#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STD_FUNCTION   // Compile with support for std::function 
#include <TaskScheduler.h>

Scheduler ts;
int counter = 0;

class Calculator {
public:
    int cumSum = 0; // cumulative sum
    Calculator(int b) {
        // Pass the this pointer, so that we get access to this->cumSum
        // Also pass a copy of b
        calculateTask.set(TASK_SECOND, TASK_FOREVER, [this, b]() {
            counter++;
            Serial.printf("%u. %u: cumSum = %u + %u\t", counter, millis(), cumSum, b);
            cumSum += b;
            Serial.printf("Resulting cumulative sum: %u\n", cumSum);
        });
        ts.addTask(calculateTask);
        calculateTask.enable();
    }

    Task calculateTask;
};

Calculator calc1(2);
Calculator calc2(4);
Calculator calc3(8);

// Disable tasks after 10 seconds
Task tWrapper(10*TASK_SECOND, TASK_ONCE, []() {
    ts.disableAll();
}, &ts); 

/**
 * Standard Arduino setup and loop methods
 */
void setup() {
  Serial.begin(74880);
  Serial.println("std::function test");
  tWrapper.enableDelayed();
}

void loop() {
  ts.execute();
}