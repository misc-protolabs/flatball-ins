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

#include <stdint.h>
#include <ArduinoJson.hpp>
#include <catch.hpp>

using namespace ArduinoJson::detail;

template <typename T>
void checkInteger(const char* input, T expected) {
  CAPTURE(input);
  T actual = parseNumber<T>(input);
  REQUIRE(expected == actual);
}

TEST_CASE("parseNumber<int8_t>()") {
  checkInteger<int8_t>("-128", -128);
  checkInteger<int8_t>("127", 127);
  checkInteger<int8_t>("+127", 127);
  checkInteger<int8_t>("3.14", 3);
  checkInteger<int8_t>("x42", 0);
  checkInteger<int8_t>("128", 0);   // overflow
  checkInteger<int8_t>("-129", 0);  // overflow
}

TEST_CASE("parseNumber<int16_t>()") {
  checkInteger<int16_t>("-32768", -32768);
  checkInteger<int16_t>("32767", 32767);
  checkInteger<int16_t>("+32767", 32767);
  checkInteger<int16_t>("3.14", 3);
  checkInteger<int16_t>("x42", 0);
  checkInteger<int16_t>("-32769", 0);  // overflow
  checkInteger<int16_t>("32768", 0);   // overflow
}

TEST_CASE("parseNumber<int32_t>()") {
  checkInteger<int32_t>("-2147483648", (-2147483647 - 1));
  checkInteger<int32_t>("2147483647", 2147483647);
  checkInteger<int32_t>("+2147483647", 2147483647);
  checkInteger<int32_t>("3.14", 3);
  checkInteger<int32_t>("x42", 0);
  checkInteger<int32_t>("-2147483649", 0);  // overflow
  checkInteger<int32_t>("2147483648", 0);   // overflow
}

TEST_CASE("parseNumber<uint8_t>()") {
  checkInteger<uint8_t>("0", 0);
  checkInteger<uint8_t>("-0", 0);
  checkInteger<uint8_t>("255", 255);
  checkInteger<uint8_t>("+255", 255);
  checkInteger<uint8_t>("3.14", 3);
  checkInteger<uint8_t>("x42", 0);
  checkInteger<uint8_t>("-1", 0);
  checkInteger<uint8_t>("256", 0);
}

TEST_CASE("parseNumber<uint16_t>()") {
  checkInteger<uint16_t>("0", 0);
  checkInteger<uint16_t>("65535", 65535);
  checkInteger<uint16_t>("+65535", 65535);
  checkInteger<uint16_t>("3.14", 3);
  // checkInteger<uint16_t>(" 42", 0);
  checkInteger<uint16_t>("x42", 0);
  checkInteger<uint16_t>("-1", 0);
  checkInteger<uint16_t>("65536", 0);
}
