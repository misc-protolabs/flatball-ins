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
#include <string>

TEST_CASE("JsonObjectConst::size()") {
  JsonDocument doc;
  JsonObjectConst obj = doc.to<JsonObject>();

  SECTION("returns 0 when empty") {
    REQUIRE(0 == obj.size());
  }

  SECTION("returns the number of members") {
    doc["hello"] = 1;
    doc["world"] = 2;
    REQUIRE(2 == obj.size());
  }
}
