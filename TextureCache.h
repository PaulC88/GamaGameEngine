#pragma once
#include <map>

#include "DataTypes.h"

namespace GamaGameEngine {

	class TextureCache {

	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(std::string texturePath);

		bool removeTexture(std::string texturePath);

	private:
		std::map<std::string, GLTexture> m_textureMap;
	};
}