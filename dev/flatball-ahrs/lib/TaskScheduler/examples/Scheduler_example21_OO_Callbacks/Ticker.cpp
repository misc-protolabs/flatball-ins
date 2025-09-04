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

#include "Ticker.h"

Ticker::Ticker(Scheduler* aS, Task* aCalc, StatusRequest* aM) : Task(10000, TASK_FOREVER, aS, false) {
  iCalc = aCalc;
  iMeasure = aM;
}

bool Ticker::Callback() {
  Serial.println(); Serial.println(); Serial.println();
  Serial.print(millis()); Serial.print(":\t");
  Serial.println("CycleCallback: Initiating measurement cycle every 10 seconds");

  int numberSensors = random(1, 11); // 1 to 10 sensors, randomly
  Serial.print("Number of sensors=");
  Serial.println(numberSensors);

  iMeasure->setWaiting(numberSensors); // Set the StatusRequest to wait for 1 to 10 signals.
  iCalc->waitFor(iMeasure);
}

