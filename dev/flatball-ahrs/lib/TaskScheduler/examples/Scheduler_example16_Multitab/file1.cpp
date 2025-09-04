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
#include "header.hpp"


//Declare the functions we want to use before we are ready to define them
void t1Callback();


// Tasks
Task t1(2000, 10, &t1Callback, &runner, true);  //adding task to the chain on creation
Task t3(5000, TASK_FOREVER, &t3Callback);


void t1Callback() {
    Serial.print("t1: ");
    Serial.println(millis());

    if (t1.isFirstIteration()) {
      runner.addTask(t3);
      t3.enable();
      Serial.println("t1: enabled t3 and added to the chain");
    }

    if (t1.isLastIteration()) {
      t3.disable();
      runner.deleteTask(t3);
      t2.setInterval(500);
      Serial.println("t1: disable t3 and delete it from the chain. t2 interval set to 500");
    }
}

