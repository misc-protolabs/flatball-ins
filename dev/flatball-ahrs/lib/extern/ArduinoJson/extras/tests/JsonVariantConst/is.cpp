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

enum MYENUM2 { ONE = 1, TWO = 2 };

TEST_CASE("JsonVariantConst::is<T>()") {
  JsonDocument doc;
  JsonVariantConst var = doc.to<JsonVariant>();

  SECTION("unbound") {
    var = JsonVariantConst();

    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<JsonArrayConst>() == false);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonObjectConst>() == false);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<JsonVariantConst>() == false);
    CHECK(var.is<bool>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<std::string>() == false);
    CHECK(var.is<JsonString>() == false);
    CHECK(var.is<float>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }

  SECTION("null") {
    CHECK(var.is<JsonVariantConst>() == true);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<bool>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<std::string>() == false);
    CHECK(var.is<JsonString>() == false);
    CHECK(var.is<float>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }

  SECTION("true") {
    doc.set(true);

    CHECK(var.is<bool>() == true);
    CHECK(var.is<JsonVariantConst>() == true);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<std::string>() == false);
    CHECK(var.is<JsonString>() == false);
    CHECK(var.is<float>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }

  SECTION("false") {
    doc.set(false);

    CHECK(var.is<bool>() == true);
    CHECK(var.is<JsonVariantConst>() == true);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<std::string>() == false);
    CHECK(var.is<JsonString>() == false);
    CHECK(var.is<float>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }

  SECTION("int") {
    doc.set(42);

    CHECK(var.is<int>() == true);
    CHECK(var.is<short>() == true);
    CHECK(var.is<long>() == true);
    CHECK(var.is<double>() == true);
    CHECK(var.is<float>() == true);
    CHECK(var.is<MYENUM2>() == true);
    CHECK(var.is<JsonVariantConst>() == true);
    CHECK(var.is<bool>() == false);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<std::string>() == false);
    CHECK(var.is<JsonString>() == false);
  }

  SECTION("double") {
    doc.set(4.2);

    CHECK(var.is<double>() == true);
    CHECK(var.is<float>() == true);
    CHECK(var.is<JsonVariantConst>() == true);
    CHECK(var.is<bool>() == false);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<std::string>() == false);
    CHECK(var.is<JsonString>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }

  SECTION("const char*") {
    doc.set("4.2");

    CHECK(var.is<const char*>() == true);
    CHECK(var.is<const char*>() == true);
    CHECK(var.is<std::string>() == true);
    CHECK(var.is<JsonString>() == true);
    CHECK(var.is<double>() == false);
    CHECK(var.is<float>() == false);
    CHECK(var.is<bool>() == false);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }

  SECTION("JsonArray") {
    doc.to<JsonArray>();

    CHECK(var.is<JsonArrayConst>() == true);
    CHECK(var.is<JsonVariantConst>() == true);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonObjectConst>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<float>() == false);
    CHECK(var.is<bool>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }

  SECTION("JsonObject") {
    doc.to<JsonObject>();

    CHECK(var.is<JsonObjectConst>() == true);
    CHECK(var.is<JsonVariantConst>() == true);
    CHECK(var.is<JsonObject>() == false);
    CHECK(var.is<JsonVariant>() == false);
    CHECK(var.is<JsonArray>() == false);
    CHECK(var.is<JsonArrayConst>() == false);
    CHECK(var.is<int>() == false);
    CHECK(var.is<float>() == false);
    CHECK(var.is<bool>() == false);
    CHECK(var.is<const char*>() == false);
    CHECK(var.is<MYENUM2>() == false);
  }
}
