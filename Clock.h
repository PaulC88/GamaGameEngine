#include <chrono>

typedef std::chrono::high_resolution_clock hr_clock;
typedef std::chrono::nanoseconds clock_freq;


namespace GamaGameEngine {

	class Clock
	{
	public:
		Clock();
		~Clock();

		void start(void);

		void stop(void);

		long long elapsed(void);

		long long difference(void);

		long long current(void);

	private:
		hr_clock::time_point m_start;
		hr_clock::time_point m_stop;
		hr_clock::time_point m_current;
	};
}