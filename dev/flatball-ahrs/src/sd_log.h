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

#ifndef _SD_LOG_H_
#define _SD_LOG_H_

typedef struct
{
  char buf[1024 * 1]; // Buffer for data storage
  unsigned long idx;  // Current index in buffer
  unsigned long cnt;  // Total count of logged entries
  unsigned int len;   // the length of each write into the buffer
  bool sd_present;    // Flag to indicate SD card presence
  bool logging;       // Flag to indicate logging status
  bool srvr_active;   // Flag to indicate server activity status
} sd_log_T;

extern sd_log_T sd_log;

extern void sd_log_init(void);
extern void sd_log_write(void); //( bool forceFlush);
extern void sd_log_close(void);
extern unsigned int sd_log_new(void);

#endif
