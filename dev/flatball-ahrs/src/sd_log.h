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
