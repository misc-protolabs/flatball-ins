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

/**
 * TaskScheduler Test of OnEnable and OnDisable methods and illustration of using wrapper tasks for timout purposes
 *
 * 	A wrapper task runs every 10 seconds and initiates the test case
 * 	Another task is run once for 5 seconds, and serves as a LED blinking timeout - 5 seconds
 * 	Finally, a dedicated task which controls LED is running periodically until stopped, and makes the LED blink with 0.5 to 1 second interval. 
 *
 */

#define _TASK_SLEEP_ON_IDLE_RUN
#include <TaskScheduler.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 13    // define appropriate pin for your board
#endif

Scheduler ts;

// Callback methods prototypes
void WrapperCallback();
bool BlinkOnEnable();
void BlinkOnDisable();
void LEDOn();
void LEDOff();

// Tasks
Task tWrapper(10000L, TASK_FOREVER, &WrapperCallback, &ts, true);
Task tBlink(5000, TASK_ONCE, NULL, &ts, false, &BlinkOnEnable, &BlinkOnDisable);
Task tLED(0, TASK_FOREVER, NULL, &ts, false, NULL, &LEDOff);

void WrapperCallback() {
	tBlink.restartDelayed(); // LED blinking is initiated
							 //every 30 seconds for 5 seconds
}


// Upon being enabled, tBlink will define the parameters
// and enable LED blinking task, which actually controls
// the hardware (LED in this example)
bool BlinkOnEnable() {
	tLED.setInterval( 200 + random(801) );
	tLED.setCallback( &LEDOn);
	tLED.enable();

	return true; // Task should be enabled
}

// tBlink does not really need a callback function
// since it just waits for 5 seconds for the first
// and only iteration to occur. Once the iteration
// takes place, tBlink is disabled by the Scheduler,
// thus executing its OnDisable method below.

void BlinkOnDisable() {
	tLED.disable();
}

void LEDOn () {
	digitalWrite(LED_BUILTIN , HIGH);
	tLED.setCallback( &LEDOff);
}

void LEDOff () {
	digitalWrite(LED_BUILTIN , LOW);
	tLED.setCallback( &LEDOn);
}

// Note that LEDOff method serves as OnDisable method
// to make sure the LED is turned off when the tBlink
// task finishes (or disabled ahead of time)

void setup() {
// put your setup code here, to run once:
  pinMode(LED_BUILTIN , OUTPUT);
}

void loop() {
// put your main code here, to run repeatedly:
	ts.execute();
}