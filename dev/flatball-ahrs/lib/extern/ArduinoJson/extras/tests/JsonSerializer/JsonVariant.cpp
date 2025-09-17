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
#include <limits>

#include "Literals.hpp"

template <typename T>
void check(T value, const std::string& expected) {
  JsonDocument doc;
  doc.to<JsonVariant>().set(value);
  char buffer[256] = "";
  size_t returnValue = serializeJson(doc, buffer, sizeof(buffer));
  REQUIRE(expected == buffer);
  REQUIRE(expected.size() == returnValue);
}

TEST_CASE("serializeJson(JsonVariant)") {
  SECTION("Undefined") {
    check(JsonVariant(), "null");
  }

  SECTION("Null string") {
    check(static_cast<char*>(0), "null");
  }

  SECTION("const char*") {
    check("hello", "\"hello\"");
  }

  SECTION("string") {
    check("hello"_s, "\"hello\"");

    SECTION("Escape quotation mark") {
      check("hello \"world\""_s, "\"hello \\\"world\\\"\"");
    }

    SECTION("Escape reverse solidus") {
      check("hello\\world"_s, "\"hello\\\\world\"");
    }

    SECTION("Don't escape solidus") {
      check("fifty/fifty"_s, "\"fifty/fifty\"");
    }

    SECTION("Don't escape single quote") {
      check("hello'world"_s, "\"hello'world\"");
    }

    SECTION("Escape backspace") {
      check("hello\bworld"_s, "\"hello\\bworld\"");
    }

    SECTION("Escape formfeed") {
      check("hello\fworld"_s, "\"hello\\fworld\"");
    }

    SECTION("Escape linefeed") {
      check("hello\nworld"_s, "\"hello\\nworld\"");
    }

    SECTION("Escape carriage return") {
      check("hello\rworld"_s, "\"hello\\rworld\"");
    }

    SECTION("Escape tab") {
      check("hello\tworld"_s, "\"hello\\tworld\"");
    }

    SECTION("NUL char") {
      check("hello\0world"_s, "\"hello\\u0000world\"");
    }
  }

  SECTION("SerializedValue<const char*>") {
    check(serialized("[1,2]"), "[1,2]");
  }

  SECTION("SerializedValue<std::string>") {
    check(serialized("[1,2]"_s), "[1,2]");
  }

  SECTION("Double") {
    check(3.1415927, "3.1415927");
  }

  SECTION("Float") {
    REQUIRE(sizeof(float) == 4);
    check(3.1415927f, "3.141593");
  }

  SECTION("Zero") {
    check(0, "0");
  }

  SECTION("Integer") {
    check(42, "42");
  }

  SECTION("NegativeLong") {
    check(-42, "-42");
  }

  SECTION("UnsignedLong") {
    check(4294967295UL, "4294967295");
  }

  SECTION("True") {
    check(true, "true");
  }

  SECTION("OneFalse") {
    check(false, "false");
  }

#if ARDUINOJSON_USE_LONG_LONG
  SECTION("NegativeInt64") {
    check(-9223372036854775807 - 1, "-9223372036854775808");
  }

  SECTION("PositiveInt64") {
    check(9223372036854775807, "9223372036854775807");
  }

  SECTION("UInt64") {
    check(18446744073709551615U, "18446744073709551615");
  }
#endif
}
