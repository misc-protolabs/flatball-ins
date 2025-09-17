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

TEST_CASE("JsonObject::clear()") {
  SECTION("No-op on null JsonObject") {
    JsonObject obj;
    obj.clear();
    REQUIRE(obj.isNull() == true);
    REQUIRE(obj.size() == 0);
  }

  SECTION("Removes all elements") {
    JsonDocument doc;
    JsonObject obj = doc.to<JsonObject>();
    obj["hello"] = 1;
    obj["world"] = 2;
    obj.clear();
    REQUIRE(obj.size() == 0);
    REQUIRE(obj.isNull() == false);
  }
}
