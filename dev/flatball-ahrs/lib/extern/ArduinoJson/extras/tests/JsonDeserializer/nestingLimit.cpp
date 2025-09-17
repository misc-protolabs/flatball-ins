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

#include <sstream>

#include "Literals.hpp"

#define SHOULD_WORK(expression) REQUIRE(DeserializationError::Ok == expression);
#define SHOULD_FAIL(expression) \
  REQUIRE(DeserializationError::TooDeep == expression);

TEST_CASE("JsonDeserializer nesting") {
  JsonDocument doc;

  SECTION("Input = const char*") {
    SECTION("limit = 0") {
      DeserializationOption::NestingLimit nesting(0);
      SHOULD_WORK(deserializeJson(doc, "\"toto\"", nesting));
      SHOULD_WORK(deserializeJson(doc, "123", nesting));
      SHOULD_WORK(deserializeJson(doc, "true", nesting));
      SHOULD_FAIL(deserializeJson(doc, "[]", nesting));
      SHOULD_FAIL(deserializeJson(doc, "{}", nesting));
      SHOULD_FAIL(deserializeJson(doc, "[\"toto\"]", nesting));
      SHOULD_FAIL(deserializeJson(doc, "{\"toto\":1}", nesting));
    }

    SECTION("limit = 1") {
      DeserializationOption::NestingLimit nesting(1);
      SHOULD_WORK(deserializeJson(doc, "[\"toto\"]", nesting));
      SHOULD_WORK(deserializeJson(doc, "{\"toto\":1}", nesting));
      SHOULD_FAIL(deserializeJson(doc, "{\"toto\":{}}", nesting));
      SHOULD_FAIL(deserializeJson(doc, "{\"toto\":[]}", nesting));
      SHOULD_FAIL(deserializeJson(doc, "[[\"toto\"]]", nesting));
      SHOULD_FAIL(deserializeJson(doc, "[{\"toto\":1}]", nesting));
    }
  }

  SECTION("char* and size_t") {
    SECTION("limit = 0") {
      DeserializationOption::NestingLimit nesting(0);
      SHOULD_WORK(deserializeJson(doc, "\"toto\"", 6, nesting));
      SHOULD_WORK(deserializeJson(doc, "123", 3, nesting));
      SHOULD_WORK(deserializeJson(doc, "true", 4, nesting));
      SHOULD_FAIL(deserializeJson(doc, "[]", 2, nesting));
      SHOULD_FAIL(deserializeJson(doc, "{}", 2, nesting));
      SHOULD_FAIL(deserializeJson(doc, "[\"toto\"]", 8, nesting));
      SHOULD_FAIL(deserializeJson(doc, "{\"toto\":1}", 10, nesting));
    }

    SECTION("limit = 1") {
      DeserializationOption::NestingLimit nesting(1);
      SHOULD_WORK(deserializeJson(doc, "[\"toto\"]", 8, nesting));
      SHOULD_WORK(deserializeJson(doc, "{\"toto\":1}", 10, nesting));
      SHOULD_FAIL(deserializeJson(doc, "{\"toto\":{}}", 11, nesting));
      SHOULD_FAIL(deserializeJson(doc, "{\"toto\":[]}", 11, nesting));
      SHOULD_FAIL(deserializeJson(doc, "[[\"toto\"]]", 10, nesting));
      SHOULD_FAIL(deserializeJson(doc, "[{\"toto\":1}]", 12, nesting));
    }
  }

  SECTION("Input = std::string") {
    SECTION("limit = 0") {
      DeserializationOption::NestingLimit nesting(0);
      SHOULD_WORK(deserializeJson(doc, "\"toto\""_s, nesting));
      SHOULD_WORK(deserializeJson(doc, "123"_s, nesting));
      SHOULD_WORK(deserializeJson(doc, "true"_s, nesting));
      SHOULD_FAIL(deserializeJson(doc, "[]"_s, nesting));
      SHOULD_FAIL(deserializeJson(doc, "{}"_s, nesting));
      SHOULD_FAIL(deserializeJson(doc, "[\"toto\"]"_s, nesting));
      SHOULD_FAIL(deserializeJson(doc, "{\"toto\":1}"_s, nesting));
    }

    SECTION("limit = 1") {
      DeserializationOption::NestingLimit nesting(1);
      SHOULD_WORK(deserializeJson(doc, "[\"toto\"]"_s, nesting));
      SHOULD_WORK(deserializeJson(doc, "{\"toto\":1}"_s, nesting));
      SHOULD_FAIL(deserializeJson(doc, "{\"toto\":{}}"_s, nesting));
      SHOULD_FAIL(deserializeJson(doc, "{\"toto\":[]}"_s, nesting));
      SHOULD_FAIL(deserializeJson(doc, "[[\"toto\"]]"_s, nesting));
      SHOULD_FAIL(deserializeJson(doc, "[{\"toto\":1}]"_s, nesting));
    }
  }

  SECTION("Input = std::istream") {
    SECTION("limit = 0") {
      DeserializationOption::NestingLimit nesting(0);
      std::istringstream good("true");
      std::istringstream bad("[]");
      SHOULD_WORK(deserializeJson(doc, good, nesting));
      SHOULD_FAIL(deserializeJson(doc, bad, nesting));
    }

    SECTION("limit = 1") {
      DeserializationOption::NestingLimit nesting(1);
      std::istringstream good("[\"toto\"]");
      std::istringstream bad("{\"toto\":{}}");
      SHOULD_WORK(deserializeJson(doc, good, nesting));
      SHOULD_FAIL(deserializeJson(doc, bad, nesting));
    }
  }
}
