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

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace ArduinoJson::detail;

template <typename T>
static void check(const char* input, T expected) {
  T actual;
  uint8_t* f = reinterpret_cast<uint8_t*>(&actual);
  const uint8_t* d = reinterpret_cast<const uint8_t*>(input);
  doubleToFloat(d, f);
  fixEndianness(actual);
  CHECK(actual == expected);
}

TEST_CASE("doubleToFloat()") {
  check("\x40\x09\x21\xCA\xC0\x83\x12\x6F", 3.1415f);
  check("\x00\x00\x00\x00\x00\x00\x00\x00", 0.0f);
  check("\x80\x00\x00\x00\x00\x00\x00\x00", -0.0f);
  check("\xC0\x5E\xDC\xCC\xCC\xCC\xCC\xCD", -123.45f);
}
