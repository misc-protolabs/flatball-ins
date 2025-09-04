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

#ifndef _SUPER_SENSOR_H
#define _SUPER_SENSOR_H


#include "Arduino.h"
#include <TaskSchedulerDeclarations.h>

class SuperSensor {
  public:
    SuperSensor(int aPin);
    ~SuperSensor();
    void begin();
    void stop();
    long trigger();
    bool measurementReady();
    long value();

  private: 
    long  iDelay;
    long  iValue;
    int   iPin;
    unsigned long iStart;
};

#endif // _SUPER_SENSOR_H
