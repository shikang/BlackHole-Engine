#ifndef BH_CLOCK_CYCLE_H
#define BH_CLOCK_CYCLE_H

#include "Platform/Typedefs.h"

namespace BH
{
	// Cycles / Hz = Seconds (Use for profilng or any need for precise timing)
	// Cycles (Can be used to calculate percentage)

	// The number of clock cycles
	s64 GetCurrentClockCycles();

	// Hz, Get processor frequency in Hz not GHz
	s64 CalculateProcessorFrequency();

	// Get System Ticks
	s64 GetSystemTicks();

	// Get System Frequency
	s64 GetSystemFrequency();

	// Get Internal Ticks
	s64 GetInternalTicks();
}

#endif