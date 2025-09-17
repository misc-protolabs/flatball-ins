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

TEST_CASE("JsonArray::add()") {
  JsonDocument doc;
  JsonArray array = doc.to<JsonArray>();
  array.add().set(42);
  REQUIRE(doc.as<std::string>() == "[42]");
}

TEST_CASE("JsonDocument::add()") {
  JsonDocument doc;
  doc.add().set(42);
  REQUIRE(doc.as<std::string>() == "[42]");
}

TEST_CASE("ElementProxy::add()") {
  JsonDocument doc;
  doc[0].add().set(42);
  REQUIRE(doc.as<std::string>() == "[[42]]");
}

TEST_CASE("MemberProxy::add()") {
  JsonDocument doc;
  doc["x"].add().set(42);
  REQUIRE(doc.as<std::string>() == "{\"x\":[42]}");
}

TEST_CASE("JsonVariant::add()") {
  JsonDocument doc;
  JsonVariant v = doc.add();
  v.add().set(42);
  REQUIRE(doc.as<std::string>() == "[[42]]");
}
