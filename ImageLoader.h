#pragma once
#include "DataTypes.h"

#include<string>

namespace GamaGameEngine {

	class ImageLoader {

	public:
		static GLTexture loadPNG(std::string filePath);
	};
}

