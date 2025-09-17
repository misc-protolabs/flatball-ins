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

#define ARDUINOJSON_ENABLE_COMMENTS 0
#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("Comments should produce InvalidInput") {
  JsonDocument doc;

  const char* testCases[] = {
      "/*COMMENT*/  [\"hello\"]",
      "[/*COMMENT*/ \"hello\"]",
      "[\"hello\"/*COMMENT*/]",
      "[\"hello\"/*COMMENT*/,\"world\"]",
      "[\"hello\",/*COMMENT*/ \"world\"]",
      "[/*/\n]",
      "[/*COMMENT]",
      "[/*COMMENT*]",
      "//COMMENT\n\t[\"hello\"]",
      "[//COMMENT\n\"hello\"]",
      "[\"hello\"//COMMENT\r\n]",
      "[\"hello\"//COMMENT\n,\"world\"]",
      "[\"hello\",//COMMENT\n\"world\"]",
      "[/COMMENT\n]",
      "[//COMMENT",
      "/*COMMENT*/ {\"hello\":\"world\"}",
      "{/*COMMENT*/\"hello\":\"world\"}",
      "{\"hello\"/*COMMENT*/:\"world\"}",
      "{\"hello\":/*COMMENT*/\"world\"}",
      "{\"hello\":\"world\"/*COMMENT*/}",
      "//COMMENT\n {\"hello\":\"world\"}",
      "{//COMMENT\n\"hello\":\"world\"}",
      "{\"hello\"//COMMENT\n:\"world\"}",
      "{\"hello\"://COMMENT\n\"world\"}",
      "{\"hello\":\"world\"//COMMENT\n}",
      "/{\"hello\":\"world\"}",
      "{/\"hello\":\"world\"}",
      "{\"hello\"/:\"world\"}",
      "{\"hello\":/\"world\"}",
      "{\"hello\":\"world\"/}",
      "{\"hello\":\"world\"/,\"answer\":42}",
      "{\"hello\":\"world\",/\"answer\":42}",
  };
  const size_t testCount = sizeof(testCases) / sizeof(testCases[0]);

  for (size_t i = 0; i < testCount; i++) {
    const char* input = testCases[i];
    CAPTURE(input);
    REQUIRE(deserializeJson(doc, input) == DeserializationError::InvalidInput);
  }
}
