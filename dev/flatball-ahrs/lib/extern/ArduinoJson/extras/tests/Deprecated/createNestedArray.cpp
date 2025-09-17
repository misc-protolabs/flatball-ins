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

#include "Literals.hpp"

TEST_CASE("JsonDocument::createNestedArray()") {
  JsonDocument doc;

  SECTION("createNestedArray()") {
    JsonArray array = doc.createNestedArray();
    array.add(42);
    REQUIRE(doc.as<std::string>() == "[[42]]");
  }

  SECTION("createNestedArray(const char*)") {
    JsonArray array = doc.createNestedArray("key");
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

  SECTION("createNestedArray(std::string)") {
    JsonArray array = doc.createNestedArray("key"_s);
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("createNestedArray(VLA)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "key");
    JsonArray array = doc.createNestedArray(vla);
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }
#endif
}

TEST_CASE("JsonArray::createNestedArray()") {
  JsonDocument doc;
  JsonArray array = doc.to<JsonArray>();
  JsonArray nestedArray = array.createNestedArray();
  nestedArray.add(42);
  REQUIRE(doc.as<std::string>() == "[[42]]");
}

TEST_CASE("JsonObject::createNestedArray()") {
  JsonDocument doc;
  JsonObject object = doc.to<JsonObject>();

  SECTION("createNestedArray(const char*)") {
    JsonArray array = object.createNestedArray("key");
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

  SECTION("createNestedArray(std::string)") {
    JsonArray array = object.createNestedArray("key"_s);
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("createNestedArray(VLA)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "key");
    JsonArray array = object.createNestedArray(vla);
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }
#endif
}

TEST_CASE("JsonVariant::createNestedArray()") {
  JsonDocument doc;
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("createNestedArray()") {
    JsonArray array = variant.createNestedArray();
    array.add(42);
    REQUIRE(doc.as<std::string>() == "[[42]]");
  }

  SECTION("createNestedArray(const char*)") {
    JsonArray array = variant.createNestedArray("key");
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

  SECTION("createNestedArray(std::string)") {
    JsonArray array = variant.createNestedArray("key"_s);
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("createNestedArray(VLA)") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "key");
    JsonArray array = variant.createNestedArray(vla);
    array.add(42);
    REQUIRE(doc.as<std::string>() == "{\"key\":[42]}");
  }
#endif
}
