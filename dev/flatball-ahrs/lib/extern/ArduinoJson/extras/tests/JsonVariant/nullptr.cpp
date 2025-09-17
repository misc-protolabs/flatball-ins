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

#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("nullptr") {
  JsonDocument doc;
  JsonVariant variant = doc.to<JsonVariant>();

  SECTION("JsonVariant == nullptr") {
    REQUIRE(variant == nullptr);
    REQUIRE_FALSE(variant != nullptr);
  }

  SECTION("JsonVariant != nullptr") {
    variant.set(42);

    REQUIRE_FALSE(variant == nullptr);
    REQUIRE(variant != nullptr);
  }

  SECTION("JsonVariant.set(nullptr)") {
    variant.set(42);
    variant.set(nullptr);

    REQUIRE(variant.isNull());
  }

  SECTION("JsonVariant.set(nullptr) with unbound reference") {
    JsonVariant unboundReference;

    unboundReference.set(nullptr);

    REQUIRE(variant.isNull());
  }

  SECTION("JsonVariant.is<nullptr_t>()") {
    variant.set(42);
    REQUIRE(variant.is<std::nullptr_t>() == false);

    variant.clear();
    REQUIRE(variant.is<std::nullptr_t>() == true);
  }
}
