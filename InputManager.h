#pragma once

#include <unordered_map>
#include <glm\glm.hpp>

namespace GamaGameEngine {
	/*Input manager stores a key map that maps SDL_Keys to booleans.
	If the value in the key map is true, than the key is pressed.
	Otherwise, it is released.*/

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();

		//Use SDL key ids - better to wrap the SDL key iDS in an enum class so you can pick the bindings yourself. 
		//can help if transferring to a different input library from SDL
		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);
		void setMouseCoords(float x, float y);
		bool isKeyHeld(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);

		//Getters
		glm::vec2 getMouseCoords() const { return m_mouseCoords; }

	private:
		//Returns true if the key is held down
		bool wasKeyHeld(unsigned int keyID);

		std::unordered_map<unsigned int, bool> m_keyMap;
		std::unordered_map<unsigned int, bool> m_previousKeyMap;
		glm::vec2 m_mouseCoords;

	};

}