#include "InputManager.h"

namespace GamaGameEngine {

	InputManager::InputManager() {
		//Empty
	}


	InputManager::~InputManager() {
		//Empty
	}

	void InputManager::update() {
		for (auto& it : m_keyMap) {
			m_previousKeyMap[it.first] = it.second;
		}
	}

	void InputManager::pressKey(unsigned int keyID) {
		m_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID) {
		m_keyMap[keyID] = false;
	}

	void InputManager::setMouseCoords(float x, float y) {
		m_mouseCoords.x = x;
		m_mouseCoords.y = y;
	}

	bool InputManager::isKeyHeld(unsigned int keyID) {

		auto it = m_keyMap.find(keyID);
		if (it != m_keyMap.end()) {
			return it->second;
		}
		else {
			return false;
		}
	}

	bool InputManager::isKeyPressed(unsigned int keyID) {
		if (isKeyHeld(keyID) == true && wasKeyHeld(keyID) == false) {
			return true;
		}
		return false;
	}

	bool InputManager::wasKeyHeld(unsigned int keyID) {
		auto it = m_previousKeyMap.find(keyID);
		if (it != m_previousKeyMap.end()) {
			return it->second;
		}
		else {
			return false;
		}
	}

}