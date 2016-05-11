#pragma once

#include <windows.h>
#include <stdio.h>

#undef min
#undef max

typedef BOOL(__stdcall * pfnGetSystemTimes)(LPFILETIME lpIdleTime, LPFILETIME lpKernelTime, LPFILETIME lpUserTime);
static pfnGetSystemTimes s_pfnGetSystemTimes = NULL;

static HMODULE s_hKernel = NULL;

namespace GamaGameEngine {

	class PerformanceTests {

	public:
		static char cpuusage();

		static void GetSystemTimesAddress();
	};

}