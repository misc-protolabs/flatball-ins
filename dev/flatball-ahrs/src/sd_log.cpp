#include "sd_log.h"
#include "app.h"
#include "vfb.h"

#include "SPI.h"
#include "FS.h"
#include "SD_MMC.h"
#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"

File logfile;
sd_log_T sd_log = {
    .buf = {0},
    .idx = 0,
    .cnt = 0,
    .len = 0,
    .sd_present = false,
    .logging = false,
    .srvr_active = false};

void sdmmc_init(void)
{
  // SDMMC host configuration
  sdmmc_host_t host = SDMMC_HOST_DEFAULT();
  host.max_freq_khz = SDMMC_FREQ_HIGHSPEED; // Set to high-speed frequency

  // SDMMC slot configuration
  sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

  // Initialize the SD card with default parameters
  if (!SD_MMC.begin("/sdcard", true))
  {
    Serial.println("misc::SD-MMC::Card Mount Failed");
    return;
  }
  sd_log.sd_present = 1;

  uint8_t cardType = SD_MMC.cardType();

  if (cardType == CARD_NONE)
  {
    Serial.println("misc::SD-MMC::No SD_MMC card attached");
    return;
  }

  Serial.print("misc::SD-MMC::SD_MMC Card Type: ");
  if (cardType == CARD_MMC)
  {
    Serial.println("MMC");
  }
  else if (cardType == CARD_SD)
  {
    Serial.println("SDSC");
  }
  else if (cardType == CARD_SDHC)
  {
    Serial.println("SDHC");
  }
  else
  {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  Serial.printf("misc::SD-MMC::Card Size: %lluMB\n", cardSize);
  Serial.printf("misc::SD-MMC::Total space: %lluMB\n", SD_MMC.totalBytes() / (1024 * 1024));
  Serial.printf("misc::SD-MMC::Used space: %lluMB\n", SD_MMC.usedBytes() / (1024 * 1024));
}

void sd_log_init(void)
{
  sd_log.srvr_active = 0;
  sd_log.logging = 0;
  sd_log.sd_present = 0;

  Serial.println("misc::SD-MMC::configuration...");
  sdmmc_init();

  Serial.println("misc::SD-MMC");
}

unsigned int sd_log_new(void)
{
  sd_log.logging = 1;

  unsigned int n = 0;
  unsigned int n_log = 0;
  char fname[32];
  int stat = SD_MMC.exists("/logs");
  if (!stat)
  {
    stat = SD_MMC.mkdir("/logs");
  }

  char hdr[128];
  int len;
  for (n = 0; n < 0xffff; n++)
  {
    sprintf((char *)(&fname[0]), "/logs/x%08x.csv", n);
    stat = SD_MMC.exists((const char *)(&fname[0]));
    if (!stat)
    {
      logfile = SD_MMC.open(fname, FILE_WRITE);

      len = sprintf(hdr, "idx");
      logfile.write((const uint8_t *)(hdr), len);
      len = sprintf(hdr, ",batt-v,patm-Pa,amb-temp-degC");
      logfile.write((const uint8_t *)(hdr), len);
      len = sprintf(hdr, ",acc-x,acc-y,acc-z");
      logfile.write((const uint8_t *)(hdr), len);
      len = sprintf(hdr, ",gyro-x,gyro-y,gyro-z");
      logfile.write((const uint8_t *)(hdr), len);
      len = sprintf(hdr, ",mag-x,mag-y,mag-z");
      logfile.write((const uint8_t *)(hdr), len);
      len = sprintf(hdr, ",roll,pitch,yaw");
      logfile.write((const uint8_t *)(hdr), len);
      len = sprintf(hdr, ",qw, qx, qy, qz");
      logfile.write((const uint8_t *)(hdr), len);
      // len = sprintf( hdr, ",gravx,gravy,gravz");
      // logfile.write( (const uint8_t*)( hdr), len);
      len = sprintf(hdr, ",fe-acc-x,fe-acc-y,fe-acc-z");
      logfile.write((const uint8_t *)(hdr), len);
      len = sprintf(hdr, ",fl-acc-x,fl-acc-y,fl-acc-z");
      logfile.write((const uint8_t *)(hdr), len);
      len = sprintf(hdr, ",ahrs_accel_err,ahrs_accel_recovery_trig");
      logfile.write((const uint8_t *)(hdr), len);
      len = sprintf(hdr, ",ahrs_mag_err,ahrs_mag_recovery_trig");
      logfile.write((const uint8_t *)(hdr), len);
      len = sprintf(hdr, ",ahrs_flgs");
      logfile.write((const uint8_t *)(hdr), len);

      len = sprintf(hdr, "\n");
      logfile.write((const uint8_t *)(hdr), len);
      sd_log.cnt = 0;
      n_log = n;
      n = 0xffff;
      Serial.printf("SD - (%S) created.\n", fname);
    }
  }

  return n_log;
}

void sd_log_write(void)
{
  if (sd_log.logging)
  {
    sd_log.len = snprintf(sd_log.buf + sd_log.idx, sizeof(sd_log.buf) - sd_log.idx,
                          "%lu,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%u\n",
                          sd_log.cnt++,
                          v_batt, patm, degC,
                          ax, ay, az,
                          gx, gy, gz,
                          mx, my, mz,
                          roll, pitch, yaw,
                          qw, qx, qy, qz,
                          fe_ax, fe_ay, fe_az,
                          fl_ax, fl_ay, fl_az,
                          ahrs_accel_err, ahrs_accel_recovery_trig,
                          ahrs_mag_err, ahrs_mag_recovery_trig,
                          ahrs_flgs);
  }

  if (sd_log.len > 0)
  {
    logfile.write((const uint8_t *)(&sd_log.buf[0]), sd_log.len);
    sd_log.len = 0;
  }
}
/*
void sd_log_write(bool forceFlush) {
  float dt = 0.01; // 100Hz
  int len;
    if (sd_log.logging) {

    // Force flush if the flag is set
    if (forceFlush) {
      logfile.write(reinterpret_cast<const uint8_t *>(sd_log.buf), sd_log.idx);
      sd_log.idx = 0;  // Reset the index
    }

    len = snprintf(sd_log.buf + sd_log.idx, sizeof(sd_log.buf) - sd_log.idx,
        "%lu,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
        sd_log.cnt++,
        v_batt, patm, degC,
        ax, ay, az,
        gx, gy, gz,
        mx, my, mz,
        roll, pitch, yaw,
        qw, qx, qy, qz,
        fe_ax, fe_ay, fe_az,
        fl_ax, fl_ay, fl_az);
        // Check if there's enough space in the buffer or if a flush is required
        if (sd_log.idx + len < sizeof(sd_log.buf)) {
      sd_log.idx += len;
        } else {
            // Flush the buffer to SDMMC
            logfile.write(reinterpret_cast<const uint8_t *>(sd_log.buf), sd_log.idx);

            // Write the new data into the now-empty buffer
            sd_log.idx = snprintf(sd_log.buf, sizeof(sd_log.buf),
        "%lu,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
        sd_log.cnt++,
        v_batt, patm, degC,
        ax, ay, az,
        gx, gy, gz,
        mx, my, mz,
        roll, pitch, yaw,
        qw, qx, qy, qz,
        fe_ax, fe_ay, fe_az,
        fl_ax, fl_ay, fl_az);
        }
    } else {
    len = snprintf(sd_log.buf + sd_log.idx, sizeof(sd_log.buf) - sd_log.idx,
        "%lu,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
        sd_log.cnt++,
        v_batt, patm, degC,
        ax, ay, az,
        gx, gy, gz,
        mx, my, mz,
        roll, pitch, yaw,
        qw, qx, qy, qz,
        fe_ax, fe_ay, fe_az,
        fl_ax, fl_ay, fl_az);
        // Check if there's enough space in the buffer
        if (sd_log.idx + len < sizeof(sd_log.buf)) {
      sd_log.idx += len;
        } else {
      sd_log.idx = snprintf(sd_log.buf, sizeof(sd_log.buf),
        "%lu,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
        sd_log.cnt++,
        v_batt, patm, degC,
        ax, ay, az,
        gx, gy, gz,
        mx, my, mz,
        roll, pitch, yaw,
        qw, qx, qy, qz,
        fe_ax, fe_ay, fe_az,
        fl_ax, fl_ay, fl_az);
    }
  }
}
*/

void sd_log_close(void)
{
  if (sd_log.logging)
  {
    Serial.println("Closing logfile.");
    sd_log_write(); //(1);
    logfile.close();
    sd_log.logging = 0;
  }
}
