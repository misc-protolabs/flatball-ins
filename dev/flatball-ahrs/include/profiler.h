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

#pragma once
#include <stdint.h>

#define NUM_TASKS 3     // 3 per core: [1Hz, 10Hz, 100Hz]
#define BUFFER_SIZE 100 // Samples per task

struct TaskProfiler
{
    const char *name;
    uint32_t t_entry[BUFFER_SIZE];
    uint32_t t_exit[BUFFER_SIZE];
    uint8_t index;
    uint32_t frequency;
    float mean;
    float stddev;
    float min, max;
};

void Profiler_init();
void Profiler_markEntry(uint8_t core, uint8_t task_id, uint32_t timestamp_us);
void Profiler_markExit(uint8_t core, uint8_t task_id, uint32_t timestamp_us);
void Profiler_computeStats();
void Profiler_printStats();
void Profiler_printUtilization();