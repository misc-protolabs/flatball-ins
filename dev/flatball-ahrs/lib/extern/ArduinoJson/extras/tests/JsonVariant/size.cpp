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

TEST_CASE("JsonVariant::size()") {
  JsonDocument doc;
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("unbound reference") {
    JsonVariant unbound;

    CHECK(unbound.size() == 0);
  }

  SECTION("int") {
    variant.set(42);

    CHECK(variant.size() == 0);
  }

  SECTION("string") {
    variant.set("hello");

    CHECK(variant.size() == 0);
  }

  SECTION("object") {
    variant["a"] = 1;
    variant["b"] = 2;

    CHECK(variant.size() == 2);
  }
}
