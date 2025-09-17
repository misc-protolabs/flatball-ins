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

TEST_CASE("JsonArrayConst::begin()/end()") {
  SECTION("Non null JsonArrayConst") {
    JsonDocument doc;
    JsonArrayConst array = doc.to<JsonArray>();
    doc.add(12);
    doc.add(34);

    auto it = array.begin();
    auto end = array.end();

    REQUIRE(end != it);
    REQUIRE(12 == it->as<int>());
    REQUIRE(12 == static_cast<int>(*it));
    ++it;
    REQUIRE(end != it);
    REQUIRE(34 == it->as<int>());
    REQUIRE(34 == static_cast<int>(*it));
    ++it;
    REQUIRE(end == it);
  }

  SECTION("Null JsonArrayConst") {
    JsonArrayConst array;

    REQUIRE(array.begin() == array.end());
  }
}
