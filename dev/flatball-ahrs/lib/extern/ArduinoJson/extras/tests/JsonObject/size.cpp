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

TEST_CASE("JsonObject::size()") {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();

  SECTION("initial size is zero") {
    REQUIRE(0 == obj.size());
  }

  SECTION("increases when values are added") {
    obj["hello"] = 42;
    REQUIRE(1 == obj.size());
  }

  SECTION("decreases when values are removed") {
    obj["hello"] = 42;
    obj.remove("hello");
    REQUIRE(0 == obj.size());
  }

  SECTION("doesn't increase when the same key is added twice") {
    obj["hello"] = 1;
    obj["hello"] = 2;
    REQUIRE(1 == obj.size());
  }

  SECTION("doesn't decrease when another key is removed") {
    obj["hello"] = 1;
    obj.remove("world");
    REQUIRE(1 == obj.size());
  }
}
