#pragma once

namespace GamaGameEngine {

	class FpsLimiter {
	public:
		FpsLimiter();
		
		void setMaxFPS(float maxFPS);

		void begin();

		float limitFPS();

	private:

		void calculateFPS();

		float m_fps;
		float m_maxFPS;
		float m_frameTime;
		unsigned int m_startTicks;
	};
}