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

#include "profiler.h"
#include <Arduino.h>
#include <math.h>

static TaskProfiler tasks_core0[NUM_TASKS] = {
    {"Core0_100Hz", {}, {}, 0, 100, 0.0, 0.0},
    {"Core0_10Hz", {}, {}, 0, 10, 0.0, 0.0},
    {"Core0_1Hz", {}, {}, 0, 1, 0.0, 0.0},
};

static TaskProfiler tasks_core1[NUM_TASKS] = {
    {"Core1_100Hz", {}, {}, 0, 100, 0.0, 0.0},
    {"Core1_10Hz", {}, {}, 0, 10, 0.0, 0.0},
    {"Core1_1Hz", {}, {}, 0, 1, 0.0, 0.0},
};

void Profiler_init()
{
    for (int i = 0; i < NUM_TASKS; i++)
    {
        tasks_core0[i].index = 0;
        tasks_core1[i].index = 0;
        tasks_core0[i].mean = 0.0;
        tasks_core0[i].stddev = 0.0;
        for (int j = 0; j < NUM_TASKS; j++)
        {
            tasks_core0[i].t_entry[j] = 0;
            tasks_core0[i].t_exit[j] = 0;
            tasks_core1[i].t_entry[j] = 0;
            tasks_core1[i].t_exit[j] = 0;
        }
    }
}

void Profiler_markEntry(uint8_t core, uint8_t task_id, uint32_t timestamp_us)
{
    if (core == 0)
    {
        TaskProfiler &t = tasks_core0[task_id];
        t.t_entry[t.index % BUFFER_SIZE] = timestamp_us;
    }
    else
    {
        TaskProfiler &t = tasks_core1[task_id];
        t.t_entry[t.index % BUFFER_SIZE] = timestamp_us;
    }
}

void Profiler_markExit(uint8_t core, uint8_t task_id, uint32_t timestamp_us)
{
    TaskProfiler *t;
    if (core == 0)
    {
        t = &tasks_core0[task_id];
    }
    else
    {
        t = &tasks_core1[task_id];
    }
    t->t_exit[t->index % BUFFER_SIZE] = timestamp_us;
    t->index++;
    if (t->index >= BUFFER_SIZE)
    {
        t->index = 0; // Reset index if it exceeds BUFFER_SIZE
    }
}

void Profiler_computeStats()
{
    for (int i = 0; i < NUM_TASKS; i++)
    {
        TaskProfiler &t = tasks_core0[i];
        float sum = 0.0f;
        float min = 1e9;
        float max = -1e9;
        for (int j = 0; j < BUFFER_SIZE; j++)
        {
            float duration = t.t_exit[j] - t.t_entry[j];
            sum += duration;
            if (duration < min)
                min = duration;
            if (duration > max)
                max = duration;
        }
        t.mean = (sum / BUFFER_SIZE);
        t.min = min;
        t.max = max;

        float sum_squared_diff = 0.0f;
        for (int j = 0; j < BUFFER_SIZE; j++)
        {
            float duration = t.t_exit[j] - t.t_entry[j];
            if (duration >= 0.0f)
            {
                float diff = duration - t.mean;
                sum_squared_diff += diff * diff;
            }
        }
        t.stddev = sqrtf(sum_squared_diff / (BUFFER_SIZE - 1));
    }
    for (int i = 0; i < NUM_TASKS; i++)
    {
        TaskProfiler &t = tasks_core1[i];
        float sum = 0.0f;
        float min = 1e9;
        float max = -1e9;
        for (int j = 0; j < BUFFER_SIZE; j++)
        {
            float duration = t.t_exit[j] - t.t_entry[j];
            sum += duration;
            if (duration < min)
                min = duration;
            if (duration > max)
                max = duration;
        }
        t.mean = (sum / BUFFER_SIZE);
        t.min = min;
        t.max = max;

        float sum_squared_diff = 0.0f;
        for (int j = 0; j < BUFFER_SIZE; j++)
        {
            float duration = t.t_exit[j] - t.t_entry[j];
            if (duration >= 0.0f)
            {
                float diff = duration - t.mean;
                sum_squared_diff += diff * diff;
            }
        }
        t.stddev = sqrtf(sum_squared_diff / (BUFFER_SIZE - 1));
    }
}

void Profiler_printStats()
{
    Serial.printf("[%8.2f,%8.2f], [%8.2f,%8.2f], [%8.2f,%8.2f], [%8.2f,%8.2f], [%8.2f,%8.2f], [%8.2f,%8.2f]",
                  tasks_core0[0].mean, tasks_core0[0].stddev,
                  tasks_core0[1].mean, tasks_core0[1].stddev,
                  tasks_core0[2].mean, tasks_core0[2].stddev,
                  tasks_core1[0].mean, tasks_core1[0].stddev,
                  tasks_core1[1].mean, tasks_core1[1].stddev,
                  tasks_core1[2].mean, tasks_core1[2].stddev);
    /*
    Serial.printf("\n");
    Serial.printf("[%8.2f,%8.2f], [%8.2f,%8.2f], [%8.2f,%8.2f], [%8.2f,%8.2f], [%8.2f,%8.2f], [%8.2f,%8.2f]",
                  tasks_core0[0].min, tasks_core0[0].max,
                  tasks_core0[1].min, tasks_core0[1].max,
                  tasks_core0[2].min, tasks_core0[2].max,
                  tasks_core1[0].min, tasks_core1[0].max,
                  tasks_core1[1].min, tasks_core1[1].max,
                  tasks_core1[2].min, tasks_core1[2].max);
    */
}

void Profiler_printUtilization()
{
    for (int i = 0; i < NUM_TASKS; i++)
    {
        TaskProfiler &t = tasks_core0[i];
        uint8_t count = (t.index > BUFFER_SIZE) ? BUFFER_SIZE : t.index;
        float total_active_time_us = 0.0f;

        for (int j = 0; j < count; j++)
        {
            total_active_time_us += t.t_exit[j] - t.t_entry[j];
        }

        float period_us = 1e6f / t.frequency;
        float total_time_window_us = count * period_us;
        float utilization = (total_time_window_us > 0) ? (100.0f * total_active_time_us / total_time_window_us) : 0;

        printf("[%s] CPU Utilization: %.2f%%\n", t.name, utilization);
    }
}