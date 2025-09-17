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

#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("ARDUINOJSON_USE_LONG_LONG == 1") {
  JsonDocument doc;
  JsonObject root = doc.to<JsonObject>();

  root["A"] = 123456789123456789;
  root["B"] = 987654321987654321;

  std::string json;
  serializeJson(doc, json);

  REQUIRE(json == "{\"A\":123456789123456789,\"B\":987654321987654321}");
}
