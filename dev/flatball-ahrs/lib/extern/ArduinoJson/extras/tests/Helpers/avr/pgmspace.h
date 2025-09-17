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

#pragma once

#include <stdint.h>  // uint8_t

#define PROGMEM

class __FlashStringHelper;

inline const void* convertPtrToFlash(const void* s) {
  return reinterpret_cast<const char*>(s) + 42;
}

inline const void* convertFlashToPtr(const void* s) {
  return reinterpret_cast<const char*>(s) - 42;
}

#define PSTR(X) reinterpret_cast<const char*>(convertPtrToFlash(X))
#define F(X) reinterpret_cast<const __FlashStringHelper*>(PSTR(X))

inline uint8_t pgm_read_byte(const void* p) {
  return *reinterpret_cast<const uint8_t*>(convertFlashToPtr(p));
}

#define ARDUINOJSON_DEFINE_PROGMEM_ARRAY(type, name, ...)                \
  static type const ARDUINOJSON_CONCAT2(name, _progmem)[] = __VA_ARGS__; \
  static type const* name = reinterpret_cast<type const*>(               \
      convertPtrToFlash(ARDUINOJSON_CONCAT2(name, _progmem)));
