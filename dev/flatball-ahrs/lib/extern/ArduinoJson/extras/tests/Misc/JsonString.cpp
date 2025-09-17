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

TEST_CASE("JsonString") {
  SECTION("Default constructor creates a null JsonString") {
    JsonString s;

    CHECK(s.isNull() == true);
    CHECK(s.c_str() == 0);
    CHECK(s == JsonString());
    CHECK(s != "");
  }

  SECTION("Null converts to false") {
    JsonString s;

    CHECK(bool(s) == false);
  }

  SECTION("Empty string converts to true") {
    JsonString s("");

    CHECK(bool(s) == true);
  }

  SECTION("Non-empty string converts to true") {
    JsonString s("");

    CHECK(bool(s) == true);
  }

  SECTION("Null strings equals each others") {
    JsonString a, b;

    CHECK(a == b);
    CHECK_FALSE(a != b);
  }

  SECTION("Null and empty strings differ") {
    JsonString a, b("");

    CHECK_FALSE(a == b);
    CHECK(a != b);

    CHECK_FALSE(b == a);
    CHECK(b != a);
  }

  SECTION("Null and non-empty strings differ") {
    JsonString a, b("hello");

    CHECK_FALSE(a == b);
    CHECK(a != b);

    CHECK_FALSE(b == a);
    CHECK(b != a);
  }

  SECTION("Compare different strings") {
    JsonString a("hello"), b("world");

    CHECK_FALSE(a == b);
    CHECK(a != b);
  }

  SECTION("Compare identical by pointer") {
    JsonString a("hello"), b("hello");

    CHECK(a == b);
    CHECK_FALSE(a != b);
  }

  SECTION("Compare identical by value") {
    char s1[] = "hello";
    char s2[] = "hello";
    JsonString a(s1), b(s2);

    CHECK(a == b);
    CHECK_FALSE(a != b);
  }

  SECTION("std::stream") {
    std::stringstream ss;
    ss << JsonString("hello world!");
    CHECK(ss.str() == "hello world!");
  }

  SECTION("Construct with a size") {
    JsonString s("hello world", 5);

    CHECK(s.size() == 5);
    CHECK(s == "hello");
    CHECK(s != "hello world");
  }
}
