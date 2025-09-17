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

template <typename TOut, typename TIn>
void shouldBeOk(TIn value) {
  JsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();
  var.set(value);
  REQUIRE(var.as<TOut>() == TOut(value));
}

template <typename TOut, typename TIn>
void shouldOverflow(TIn value) {
  JsonDocument doc;
  JsonVariant var = doc.to<JsonVariant>();
  var.set(value);
  REQUIRE(var.as<TOut>() == 0);
  REQUIRE(var.is<TOut>() == false);
}

TEST_CASE("Handle integer overflow in stored integer") {
  SECTION("int8_t") {
    // ok
    shouldBeOk<int8_t>(-128);
    shouldBeOk<int8_t>(42.0);
    shouldBeOk<int8_t>(127);

    // too low
    shouldOverflow<int8_t>(-128.1);
    shouldOverflow<int8_t>(-129);

    // too high
    shouldOverflow<int8_t>(128);
    shouldOverflow<int8_t>(127.1);
  }

  SECTION("int16_t") {
    // ok
    shouldBeOk<int16_t>(-32768);
    shouldBeOk<int16_t>(-32767.9);
    shouldBeOk<int16_t>(32766.9);
    shouldBeOk<int16_t>(32767);

    // too low
    shouldOverflow<int16_t>(-32768.1);
    shouldOverflow<int16_t>(-32769);

    // too high
    shouldOverflow<int16_t>(32767.1);
    shouldOverflow<int16_t>(32768);
  }

  SECTION("uint8_t") {
    // ok
    shouldBeOk<uint8_t>(1);
    shouldBeOk<uint8_t>(42.0);
    shouldBeOk<uint8_t>(255);

    // too low
    shouldOverflow<uint8_t>(-1);
    shouldOverflow<uint8_t>(-0.1);

    // to high
    shouldOverflow<uint8_t>(255.1);
    shouldOverflow<uint8_t>(256);
    shouldOverflow<uint8_t>(257);
  }
}
