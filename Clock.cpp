#include "Clock.h"

namespace GamaGameEngine {

	Clock::Clock() {
		start();
		stop();
	}

	Clock::~Clock() {
		//Empty
	}

	// Records current time in start variable
	void Clock::start(void)	{
		m_start = hr_clock::now();
	}

	// Records current time in stop variable
	void Clock::stop(void) {
		m_stop = hr_clock::now();
	}

	// Get current time from previous Start call
	long long Clock::elapsed(void) {
		m_current = hr_clock::now();
		return std::chrono::duration_cast<clock_freq>(m_current - m_start).count();
	}

	// Time between last Start and Stop calls
	long long Clock::difference(void) {
		return std::chrono::duration_cast<clock_freq>(m_stop - m_start).count();
	}

	// Get the current clock count
	long long Clock::current(void) {
		m_current = hr_clock::now();
		return std::chrono::duration_cast<clock_freq>(m_current.time_since_epoch()).count();
	}
}