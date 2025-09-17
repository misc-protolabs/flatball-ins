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

// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

// Include any header that might use the conflicting macros
#include <cmath>
#include <iostream>
#include <string>

// All cores
#define bit()
#define constrain()
#define DEFAULT
#define DISABLED
#define HIGH
#define INPUT
#define LOW
#define max()
#define min()
#define OUTPUT
#define round()
#define sq()
#define word()
#define bitRead()
#define bitSet()
#define bitClear()
#define bitWrite()
#define interrupts()
#define lowByte()
#define highByte()
#define DEC
#define HEX
#define OCT
#define BIN
#define cbi()
#define sbi()

// ESP8266
#define _max()
#define _min()

// Realtek Ameba
#define isdigit(c) (((c) >= '0') && ((c) <= '9'))
#define isprint(c)
#define isxdigit(c)
#define isspace(c)
#define isupper(c)
#define islower(c)
#define isalpha(c)

// issue #839
#define BLOCKSIZE
#define CAPACITY

// issue #1905
#define _current

// issue #1914
#define V7 7

// STM32, Mbed, Particle
#define A0 16
#define A1 17
#define A2 18

// catch.hpp mutes several warnings, this file also allows to detect them
#include "ArduinoJson.h"
