#include "PerformanceTests.h"

namespace GamaGameEngine {

	char PerformanceTests::cpuusage() {

		FILETIME               ft_sys_idle;
		FILETIME               ft_sys_kernel;
		FILETIME               ft_sys_user;

		ULARGE_INTEGER         ul_sys_idle;
		ULARGE_INTEGER         ul_sys_kernel;
		ULARGE_INTEGER         ul_sys_user;

		static ULARGE_INTEGER  ul_sys_idle_old;
		static ULARGE_INTEGER  ul_sys_kernel_old;
		static ULARGE_INTEGER  ul_sys_user_old;

		CHAR  usage = 0;

		s_pfnGetSystemTimes(&ft_sys_idle, &ft_sys_kernel, &ft_sys_user);

		CopyMemory(&ul_sys_idle, &ft_sys_idle, sizeof(FILETIME));
		CopyMemory(&ul_sys_kernel, &ft_sys_kernel, sizeof(FILETIME));
		CopyMemory(&ul_sys_user, &ft_sys_user, sizeof(FILETIME));

		usage = (((((ul_sys_kernel.QuadPart - ul_sys_kernel_old.QuadPart) + (ul_sys_user.QuadPart - ul_sys_user_old.QuadPart))
			- (ul_sys_idle.QuadPart - ul_sys_idle_old.QuadPart)) * (100)) /
			((ul_sys_kernel.QuadPart - ul_sys_kernel_old.QuadPart) + (ul_sys_user.QuadPart - ul_sys_user_old.QuadPart)));

		ul_sys_idle_old.QuadPart = ul_sys_idle.QuadPart;
		ul_sys_user_old.QuadPart = ul_sys_user.QuadPart;
		ul_sys_kernel_old.QuadPart = ul_sys_kernel.QuadPart;

		return usage;
	}

	void PerformanceTests::GetSystemTimesAddress() {
		if (s_hKernel == NULL) {
			s_hKernel = LoadLibrary(L"Kernel32.dll");
		
			if (s_hKernel != NULL) {
				s_pfnGetSystemTimes = (pfnGetSystemTimes)GetProcAddress(s_hKernel, "GetSystemTimes");
			
				if (s_pfnGetSystemTimes == NULL) {
					FreeLibrary(s_hKernel); s_hKernel = NULL;
				}
			}
		}
	}

}