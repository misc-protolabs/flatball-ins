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
#include <string>
#include <utility>

using namespace std;

TEST_CASE("std::swap") {
  SECTION("JsonDocument*") {
    JsonDocument *p1, *p2;
    swap(p1, p2);  // issue #1678
  }

  SECTION("JsonDocument") {
    JsonDocument doc1, doc2;
    doc1.set("hello");
    doc2.set("world");

    swap(doc1, doc2);

    CHECK(doc1.as<string>() == "world");
    CHECK(doc2.as<string>() == "hello");
  }
}
