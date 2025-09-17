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

#include <catch.hpp>
#include <limits>
#include <string>

#include <ArduinoJson/Json/TextFormatter.hpp>
#include <ArduinoJson/Serialization/Writer.hpp>

using namespace ArduinoJson::detail;

template <typename T>
void checkWriteInteger(T value, std::string expected) {
  char output[64] = {0};
  StaticStringWriter sb(output, sizeof(output));
  TextFormatter<StaticStringWriter> writer(sb);
  writer.writeInteger<T>(value);
  REQUIRE(expected == output);
  REQUIRE(writer.bytesWritten() == expected.size());
}

TEST_CASE("int8_t") {
  checkWriteInteger<int8_t>(0, "0");
  checkWriteInteger<int8_t>(-128, "-128");
  checkWriteInteger<int8_t>(127, "127");
}

TEST_CASE("uint8_t") {
  checkWriteInteger<uint8_t>(0, "0");
  checkWriteInteger<uint8_t>(255, "255");
}

TEST_CASE("int16_t") {
  checkWriteInteger<int16_t>(0, "0");
  checkWriteInteger<int16_t>(-32768, "-32768");
  checkWriteInteger<int16_t>(32767, "32767");
}

TEST_CASE("uint16_t") {
  checkWriteInteger<uint16_t>(0, "0");
  checkWriteInteger<uint16_t>(65535, "65535");
}

TEST_CASE("int32_t") {
  checkWriteInteger<int32_t>(0, "0");
  checkWriteInteger<int32_t>(-2147483647 - 1, "-2147483648");
  checkWriteInteger<int32_t>(2147483647, "2147483647");
}

TEST_CASE("uint32_t") {
  checkWriteInteger<uint32_t>(0, "0");
  checkWriteInteger<uint32_t>(4294967295U, "4294967295");
}
