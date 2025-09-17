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
#include <stdint.h>
#include <catch.hpp>

#include "Literals.hpp"

TEST_CASE("JsonVariantConst::as<T>()") {
  JsonDocument doc;
  JsonVariantConst var = doc.to<JsonVariant>();

  doc.set("hello");

  REQUIRE(var.as<bool>() == true);
  REQUIRE(var.as<long>() == 0L);
  REQUIRE(var.as<const char*>() == "hello"_s);
  REQUIRE(var.as<std::string>() == "hello"_s);
}

TEST_CASE("Invalid conversions") {
  using namespace ArduinoJson::detail;

  JsonVariantConst variant;

  CHECK(is_same<decltype(variant.as<int>()), int>::value);
  CHECK(is_same<decltype(variant.as<float>()), float>::value);
  CHECK(is_same<decltype(variant.as<JsonVariantConst>()),
                JsonVariantConst>::value);
  CHECK(
      is_same<decltype(variant.as<JsonObjectConst>()), JsonObjectConst>::value);
  CHECK(is_same<decltype(variant.as<JsonArrayConst>()), JsonArrayConst>::value);

  CHECK(is_same<decltype(variant.as<JsonVariant>()),
                InvalidConversion<JsonVariantConst, JsonVariant>>::value);
  CHECK(is_same<decltype(variant.as<JsonObject>()),
                InvalidConversion<JsonVariantConst, JsonObject>>::value);
  CHECK(is_same<decltype(variant.as<JsonArray>()),
                InvalidConversion<JsonVariantConst, JsonArray>>::value);
}
