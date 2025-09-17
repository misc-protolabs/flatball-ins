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

TEST_CASE("JsonObjectConst::nesting()") {
  JsonDocument doc;
  JsonObjectConst obj = doc.to<JsonObject>();

  SECTION("return 0 if unbound") {
    JsonObjectConst unbound;
    REQUIRE(unbound.nesting() == 0);
  }

  SECTION("returns 1 for empty object") {
    REQUIRE(obj.nesting() == 1);
  }

  SECTION("returns 1 for flat object") {
    doc["hello"] = "world";
    REQUIRE(obj.nesting() == 1);
  }

  SECTION("returns 2 with nested array") {
    doc["nested"].to<JsonArray>();
    REQUIRE(obj.nesting() == 2);
  }

  SECTION("returns 2 with nested object") {
    doc["nested"].to<JsonObject>();
    REQUIRE(obj.nesting() == 2);
  }
}
