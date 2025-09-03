#include <SparkFun_MMC5983MA_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_MMC5983MA

#include "mag.h"
#include "vfb.h"

#define MAG_USE_ISR 0
#define MAG_USE_I2C 0

bool mag_update_offsts(uint32_t *offsetX, uint32_t *offsetY, uint32_t *offsetZ);

bool mag_present = 0;
SFE_MMC5983MA mag3dof;

uint32_t k_mag_x_offst = 131072;
uint32_t k_mag_y_offst = 131072;
uint32_t k_mag_z_offst = 131072;

#if MAG_USE_ISR == 1
bool mag3dof_isr_edge;

void mag3dof_isr(void)
{
  mag3dof_isr_edge = true;
}
#endif

void mag_init(void)
{
  // Begin the Mag
  mag_present = 0;
#if MAG_USE_I2C
  if (!mag3dof.begin())
  {
    Serial.println(F("Mag did not begin. Freezing..."));
  }
#else
  if (!mag3dof.begin(MAG_CS))
  {
    Serial.println(F("misc::MMC5983MA::Mag did not begin. Freezing..."));
  }
#endif
  mag_present = 1;

  Serial.println("misc::MMC5983MA::configuration...");

  mag3dof.softReset();

  Serial.println("misc::MMC5983MA::Setting filter bandwith to 800 Hz for continuous operation...");
  mag3dof.setFilterBandwidth(100);
  Serial.print("misc::MMC5983MA::Reading back filter bandwith: ");
  Serial.println(mag3dof.getFilterBandwith());

  // NOTE: 50hz timesout ~1/2 as often as 100hz, so for now, this is the "sampling frequency"
  Serial.println("misc::MMC5983MA::Setting continuous measurement frequency to 50 Hz...");
  mag3dof.setContinuousModeFrequency(100);
  Serial.print("misc::MMC5983MA::Reading back continuous measurement frequency: ");
  Serial.println(mag3dof.getContinuousModeFrequency());

  Serial.println("misc::MMC5983MA::Disabling auto set/reset...");
  // mag3dof.enableAutomaticSetReset();
  mag3dof.disableAutomaticSetReset();
  Serial.print("misc::MMC5983MA::Reading back automatic set/reset: ");
  Serial.println(mag3dof.isAutomaticSetResetEnabled() ? "enabled" : "disabled");

  Serial.println("misc::MMC5983MA::Enabling continuous mode...");
  mag3dof.enableContinuousMode();
  // mag3dof.disableContinuousMode();
  Serial.print("misc::MMC5983MA::Reading back continuous mode: ");
  Serial.println(mag3dof.isContinuousModeEnabled() ? "enabled" : "disabled");

  mag_update_offsts(&k_mag_x_offst, &k_mag_y_offst, &k_mag_z_offst);
  Serial.printf("misc::MMC5983MA::mag-offst = [ %7i, %7i, %7i]\n", k_mag_x_offst, k_mag_y_offst, k_mag_z_offst);

#if MAG_USE_ISR == 1
  // Configure the interrupt pin for the "Measurement Done" interrupt
  pinMode(MAG_INT, INPUT);
  attachInterrupt(digitalPinToInterrupt(MAG_INT), mag3dof_isr, RISING);

  Serial.println("Enabling interrupt...");
  mag3dof.enableInterrupt();
  Serial.print("Reading back interrupt status: ");
  Serial.println(mag3dof.isInterruptEnabled() ? "enabled" : "disabled");

  // Set our interrupt flag, just in case we missed the rising edge
  mag3dof_isr_edge = true;
#endif

  internals.mag3dof_rd_fail_cnt = 0;
#if MAG_USE_I2C == 1
  Serial.println("misc::MMC5983MA::I2C.");
#else
  Serial.println("misc::MMC5983MA::SPI.");
#endif
}

void mag_step(float *mx, float *my, float *mz)
{
  static float mx_z1, my_z1, mz_z1;
  uint32_t mag_raw_x, mag_raw_y, mag_raw_z;
  float mx_uT, my_uT, mz_uT;
  bool rd_ok = false;

  if (!mag_present)
  {
    *mx = 0.0;
    *my = 0.0;
    *mz = 0.0;
    return;
  }
#if MAG_USE_ISR == 1
  if (mag3dof_isr_edge == true)
  {
    mag3dof_isr_edge = false;         // Clear our interrupt flag
    mag3dof.clearMeasDoneInterrupt(); // Clear the MMC5983 interrupt

    // Read all three channels simultaneously
    // Note: we are calling readFieldsXYZ to read the fields, not getMeasurementXYZ
    // The measurement is already complete, we do not need to start a new one
    rd_ok = mag3dof.readFieldsXYZ(&mag_raw_x, &mag_raw_y, &mag_raw_z);
  }
#else
  rd_ok = mag3dof.getMeasurementXYZ(&mag_raw_x, &mag_raw_y, &mag_raw_z);
#endif
  if (!rd_ok)
  {
    internals.mag3dof_rd_fail_cnt++;
  }

  // The magnetic field values are 18-bit unsigned. The zero (mid) point is 2^17 (131072).
  // Normalize each field to +/- 800.0 uT
  mx_uT = ((float)(mag_raw_x) - (float)(k_mag_x_offst)) * (800.0 / 131072.0); // uT
  my_uT = ((float)(mag_raw_y) - (float)(k_mag_y_offst)) * (800.0 / 131072.0); // uT
  mz_uT = ((float)(mag_raw_z) - (float)(k_mag_z_offst)) * (800.0 / 131072.0); // 100 uT == 1 G

  // reject samples > 200 uT as this is likely erroneous - during calibration
  if (abs(mx_uT) >= 200.0)
  {
    mx_uT = mx_z1;
  }
  if (abs(my_uT) >= 200.0)
  {
    my_uT = my_z1;
  }
  if (abs(mz_uT) >= 200.0)
  {
    mz_uT = mz_z1;
  }
  // reject samples > 200 uT as this is likely erroneous - during calibration

  // average k and k-1 samples
  *mx = (mx_uT + mx_z1) / 2.0;
  *my = (my_uT + my_z1) / 2.0;
  *mz = (mz_uT + mz_z1) / 2.0;

  // update states
  mx_z1 = mx_uT;
  my_z1 = my_uT;
  mz_z1 = mz_uT;
}

bool mag_update_offsts(uint32_t *offsetX, uint32_t *offsetY, uint32_t *offsetZ) // Update the offsets
{
  bool success = true; // Use AND (&=) to record if any one command fails

  success &= mag3dof.performSetOperation(); // Perform the SET operation

  uint32_t setX = 131072;
  uint32_t setY = 131072;
  uint32_t setZ = 131072;

  success &= mag3dof.getMeasurementXYZ(&setX, &setY, &setZ); // Read all three channels
  success &= mag3dof.getMeasurementXYZ(&setX, &setY, &setZ); // Do it twice - just in case there is noise on the first

  success &= mag3dof.performResetOperation(); // Perform the RESET operation

  uint32_t resetX = 131072;
  uint32_t resetY = 131072;
  uint32_t resetZ = 131072;

  success &= mag3dof.getMeasurementXYZ(&resetX, &resetY, &resetZ); // Read all three channels
  success &= mag3dof.getMeasurementXYZ(&resetX, &resetY, &resetZ); // Do it twice - just in case there is noise on the first

  // Calculate the offset - as per the datasheet.
  // The measurements are 18-bit so it's OK to add them directly.
  if (success)
  {
    *offsetX = (setX + resetX) / 2;
    *offsetY = (setY + resetY) / 2;
    *offsetZ = (setZ + resetZ) / 2;
  }

  return success;
}
