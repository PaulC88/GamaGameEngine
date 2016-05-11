#include "TextureCache.h"
#include "ImageLoader.h"
#include "LogandDebug.h"

#include <iostream>

namespace GamaGameEngine {

	TextureCache::TextureCache() {
		//Empty
	}


	TextureCache::~TextureCache() {
		//Empty
	}

	GLTexture TextureCache::getTexture(std::string texturePath) {

		auto mit = m_textureMap.find(texturePath);

		if (mit == m_textureMap.end()) {
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			m_textureMap.insert(make_pair(texturePath, newTexture));

			logData("Loaded Texture to cache: " + texturePath);
			return newTexture;
		}
		return mit->second;
	}

	bool TextureCache::removeTexture(std::string texturePath) {

		auto mit = m_textureMap.find(texturePath);
		if (mit == m_textureMap.end()) {
			logData("Attempted to remove nonexistant texture from cache: " + texturePath);
			return true;
		}
		else {
			m_textureMap.erase(texturePath);
			logData("Texture removed from cache by Resource Manager: " + texturePath);
			return true;
		}
		return false;
	}
}