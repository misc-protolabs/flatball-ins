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

#include "SuperSensor.h"

SuperSensor::SuperSensor(int aPin) {
  iPin = aPin;
}

SuperSensor::~SuperSensor() {
  iValue = -1;
}

void SuperSensor::begin() {
  iDelay = random(300, 1500);
  iValue = -1;
}

void SuperSensor::stop() {
  //nothing to do
}

long SuperSensor::trigger() {
  iStart = millis();
  return iDelay;
}

bool SuperSensor::measurementReady() {
  if ( millis() - iStart > iDelay ) {
    iValue = random(501);
    return true;
  }
  return false;
}

long SuperSensor::value() {
  return iValue;
}

