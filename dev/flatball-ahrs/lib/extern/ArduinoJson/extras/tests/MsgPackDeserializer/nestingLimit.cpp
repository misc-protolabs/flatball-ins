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

#define SHOULD_WORK(expression) REQUIRE(DeserializationError::Ok == expression);
#define SHOULD_FAIL(expression) \
  REQUIRE(DeserializationError::TooDeep == expression);

TEST_CASE("JsonDeserializer nesting") {
  JsonDocument doc;

  SECTION("Input = const char*") {
    SECTION("limit = 0") {
      DeserializationOption::NestingLimit nesting(0);
      SHOULD_WORK(deserializeMsgPack(doc, "\xA1H", nesting));  // "H"
      SHOULD_FAIL(deserializeMsgPack(doc, "\x90", nesting));   // []
      SHOULD_FAIL(deserializeMsgPack(doc, "\x80", nesting));   // {}
    }

    SECTION("limit = 1") {
      DeserializationOption::NestingLimit nesting(1);
      SHOULD_WORK(deserializeMsgPack(doc, "\x90", nesting));           // {}
      SHOULD_WORK(deserializeMsgPack(doc, "\x80", nesting));           // []
      SHOULD_FAIL(deserializeMsgPack(doc, "\x81\xA1H\x80", nesting));  // {H:{}}
      SHOULD_FAIL(deserializeMsgPack(doc, "\x91\x90", nesting));       // [[]]
    }
  }

  SECTION("char* and size_t") {
    SECTION("limit = 0") {
      DeserializationOption::NestingLimit nesting(0);
      SHOULD_WORK(deserializeMsgPack(doc, "\xA1H", 2, nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, "\x90", 1, nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, "\x80", 1, nesting));
    }

    SECTION("limit = 1") {
      DeserializationOption::NestingLimit nesting(1);
      SHOULD_WORK(deserializeMsgPack(doc, "\x90", 1, nesting));
      SHOULD_WORK(deserializeMsgPack(doc, "\x80", 1, nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, "\x81\xA1H\x80", 4, nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, "\x91\x90", 2, nesting));
    }
  }

  SECTION("Input = std::string") {
    using std::string;

    SECTION("limit = 0") {
      DeserializationOption::NestingLimit nesting(0);
      SHOULD_WORK(deserializeMsgPack(doc, string("\xA1H"), nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, string("\x90"), nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, string("\x80"), nesting));
    }

    SECTION("limit = 1") {
      DeserializationOption::NestingLimit nesting(1);
      SHOULD_WORK(deserializeMsgPack(doc, string("\x90"), nesting));
      SHOULD_WORK(deserializeMsgPack(doc, string("\x80"), nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, string("\x81\xA1H\x80"), nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, string("\x91\x90"), nesting));
    }
  }

  SECTION("Input = std::istream") {
    SECTION("limit = 0") {
      DeserializationOption::NestingLimit nesting(0);
      std::istringstream good("\xA1H");  // "H"
      std::istringstream bad("\x90");    // []
      SHOULD_WORK(deserializeMsgPack(doc, good, nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, bad, nesting));
    }

    SECTION("limit = 1") {
      DeserializationOption::NestingLimit nesting(1);
      std::istringstream good("\x90");     // []
      std::istringstream bad("\x91\x90");  // [[]]
      SHOULD_WORK(deserializeMsgPack(doc, good, nesting));
      SHOULD_FAIL(deserializeMsgPack(doc, bad, nesting));
    }
  }
}
