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

TEST_CASE("JsonDocument::isNull()") {
  JsonDocument doc;

  SECTION("returns true if uninitialized") {
    REQUIRE(doc.isNull() == true);
  }

  SECTION("returns false after to<JsonObject>()") {
    doc.to<JsonObject>();
    REQUIRE(doc.isNull() == false);
  }

  SECTION("returns false after to<JsonArray>()") {
    doc.to<JsonArray>();
    REQUIRE(doc.isNull() == false);
  }

  SECTION("returns true after to<JsonVariant>()") {
    REQUIRE(doc.isNull() == true);
  }

  SECTION("returns false after set()") {
    doc.to<JsonVariant>().set(42);
    REQUIRE(doc.isNull() == false);
  }

  SECTION("returns true after clear()") {
    doc.to<JsonObject>();
    doc.clear();
    REQUIRE(doc.isNull() == true);
  }
}
