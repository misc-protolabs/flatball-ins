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

// This file is NOT use by Google's OSS fuzz
// I only use it to reproduce the bugs found

#include <stdint.h>  // size_t
#include <stdio.h>   // fopen et al.
#include <stdlib.h>  // exit
#include <iostream>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size);

std::vector<uint8_t> read(const char* path) {
  FILE* f = fopen(path, "rb");
  if (!f) {
    std::cerr << "Failed to open " << path << std::endl;
    exit(1);
  }

  fseek(f, 0, SEEK_END);
  size_t size = static_cast<size_t>(ftell(f));
  fseek(f, 0, SEEK_SET);

  std::vector<uint8_t> buffer(size);
  if (fread(buffer.data(), 1, size, f) != size) {
    fclose(f);
    std::cerr << "Failed to read " << path << std::endl;
    exit(1);
  }

  fclose(f);
  return buffer;
}

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: msgpack_fuzzer files" << std::endl;
    return 1;
  }

  for (int i = 1; i < argc; i++) {
    std::cout << "Loading " << argv[i] << std::endl;
    std::vector<uint8_t> buffer = read(argv[i]);
    LLVMFuzzerTestOneInput(buffer.data(), buffer.size());
  }
  return 0;
}
