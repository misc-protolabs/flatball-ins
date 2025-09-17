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

TEST_CASE("JsonArrayConst::size()") {
  JsonDocument doc;
  JsonArrayConst array = doc.to<JsonArray>();

  SECTION("returns 0 if unbound") {
    JsonArrayConst unbound;
    REQUIRE(0U == unbound.size());
  }

  SECTION("returns 0 is empty") {
    REQUIRE(0U == array.size());
  }

  SECTION("return number of elements") {
    doc.add("hello");
    doc.add("world");

    REQUIRE(2U == array.size());
  }
}
