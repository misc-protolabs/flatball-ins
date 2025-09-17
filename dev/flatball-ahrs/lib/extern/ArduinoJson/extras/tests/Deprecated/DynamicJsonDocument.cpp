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

using ArduinoJson::detail::is_base_of;

TEST_CASE("DynamicJsonDocument") {
  SECTION("is a JsonDocument") {
    REQUIRE(is_base_of<JsonDocument, DynamicJsonDocument>::value == true);
  }

  SECTION("deserialize / serialize") {
    DynamicJsonDocument doc(256);
    deserializeJson(doc, "{\"hello\":\"world\"}");
    REQUIRE(doc.as<std::string>() == "{\"hello\":\"world\"}");
  }

  SECTION("copy") {
    DynamicJsonDocument doc(256);
    doc["hello"] = "world";
    auto copy = doc;
    REQUIRE(copy.as<std::string>() == "{\"hello\":\"world\"}");
  }

  SECTION("capacity") {
    DynamicJsonDocument doc(256);
    REQUIRE(doc.capacity() == 256);
  }

  SECTION("garbageCollect()") {
    DynamicJsonDocument doc(256);
    doc.garbageCollect();
  }
}
