/*
  main.cpp - Main loop for Arduino sketches
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#define AUTOSTART_ENABLE 1

#include <AppHardwareApi.h>
#include <BbcAndPhyRegs.h>

#include <stdio.h>

extern "C" {
#include <contiki.h>
#include <lib/random.h>
}

#define ARDUINO_MAIN
#include "Arduino.h"

/*
 * Cortex-M3 Systick IT handler
 */
/*
extern void SysTick_Handler( void )
{
  // Increment tick count each ms
  TimeTick_Increment() ;
}
*/

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

/*
 * \brief Main entry point of Arduino application
 */
int main( void )
{
	// Initialize watchdog
	watchdogSetup();

	init();

	initVariant();

	delay(1);

#if defined(USBCON)
	USBDevice.attach();
#endif

	setup();
	
	autostart_start(autostart_processes);
	
	int r;
	while(1) {
		r = process_run();
	}
	
	return 0;
}

// non static declaration
#undef PROCESS_THREAD
#define PROCESS_THREAD PUBLIC_PROCESS_THREAD

PROCESS(loop_, "loop_");
AUTOSTART_PROCESSES(&loop_);

PROCESS_THREAD(loop_, ev, data) __attribute__((weak));
PROCESS_THREAD(loop_, ev, data)
{
	PROCESS_BEGIN();
	for (;;)
	{
		loop();
		if (serialEventRun) serialEventRun();
	}

	PROCESS_END();
}
