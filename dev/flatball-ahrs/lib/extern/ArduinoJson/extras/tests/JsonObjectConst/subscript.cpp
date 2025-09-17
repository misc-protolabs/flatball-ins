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

#include "Allocators.hpp"
#include "Literals.hpp"

TEST_CASE("JsonObjectConst::operator[]") {
  JsonDocument doc;
  doc["hello"] = "world";
  doc["a\0b"_s] = "ABC";
  JsonObjectConst obj = doc.as<JsonObjectConst>();

  SECTION("supports const char*") {
    REQUIRE(obj["hello"] == "world");  // issue #2019
  }

  SECTION("supports std::string") {
    REQUIRE(obj["hello"_s] == "world");  // issue #2019
    REQUIRE(obj["a\0b"_s] == "ABC");
  }

#if defined(HAS_VARIABLE_LENGTH_ARRAY) && \
    !defined(SUBSCRIPT_CONFLICTS_WITH_BUILTIN_OPERATOR)
  SECTION("supports VLA") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "hello");

    REQUIRE(obj[vla] == "world"_s);
  }
#endif

  SECTION("supports JsonVariant") {
    doc["key1"] = "hello";
    doc["key2"] = "a\0b"_s;
    doc["key3"] = "foo";
    REQUIRE(obj[obj["key1"]] == "world");
    REQUIRE(obj[obj["key2"]] == "ABC");
    REQUIRE(obj[obj["key3"]] == nullptr);
    REQUIRE(obj[obj["key4"]] == nullptr);
  }
}
