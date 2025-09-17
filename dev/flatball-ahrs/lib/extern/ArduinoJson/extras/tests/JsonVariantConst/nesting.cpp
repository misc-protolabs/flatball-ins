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

TEST_CASE("JsonVariantConst::nesting()") {
  JsonDocument doc;
  JsonVariantConst var = doc.to<JsonVariant>();

  SECTION("return 0 if unbound") {
    JsonVariantConst unbound;
    REQUIRE(unbound.nesting() == 0);
  }

  SECTION("returns 0 for string") {
    doc.set("hello");
    REQUIRE(var.nesting() == 0);
  }

  SECTION("returns 1 for empty object") {
    doc.to<JsonObject>();
    REQUIRE(var.nesting() == 1);
  }

  SECTION("returns 1 for empty array") {
    doc.to<JsonArray>();
    REQUIRE(var.nesting() == 1);
  }
}
