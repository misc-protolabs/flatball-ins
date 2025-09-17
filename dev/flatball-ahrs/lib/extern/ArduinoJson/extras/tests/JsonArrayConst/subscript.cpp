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
#include <stdint.h>
#include <catch.hpp>

TEST_CASE("JsonArrayConst::operator[]") {
  JsonDocument doc;
  JsonArrayConst arr = doc.to<JsonArray>();
  doc.add(1);
  doc.add(2);
  doc.add(3);

  SECTION("int") {
    REQUIRE(1 == arr[0].as<int>());
    REQUIRE(2 == arr[1].as<int>());
    REQUIRE(3 == arr[2].as<int>());
    REQUIRE(0 == arr[3].as<int>());
  }

  SECTION("JsonVariant") {
    REQUIRE(2 == arr[arr[0]].as<int>());
    REQUIRE(0 == arr[arr[3]].as<int>());
  }
}
