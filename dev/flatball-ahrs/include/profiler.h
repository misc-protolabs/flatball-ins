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