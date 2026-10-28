#ifndef THUMBNAILGENERATOR_H
#define THUMBNAILGENERATOR_H

#include "decomp.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <string>

namespace G3D {
class BinaryOutput;
}

extern const char sThumbnailGenerator[];

// SIZE 0xf8
class ThumbnailGenerator : public RBX::DescribedCreatable<ThumbnailGenerator, RBX::Instance, sThumbnailGenerator>,
						   public RBX::Service
{
public:
	ThumbnailGenerator();
	virtual ~ThumbnailGenerator() {}

	std::string click(std::string fileType, int cx, int cy, bool hideSky);
	std::string clickTexture(std::string textureId, std::string fileType, int cx, int cy);
	void doClick(std::string fileType, int cx, int cy, bool hideSky, G3D::BinaryOutput* binaryOutput);

	// clang-format off
	// SYNTHETIC: WEBSERVICE 0x100085f0
	// ThumbnailGenerator::`scalar deleting destructor'
	// clang-format on
};

DECOMP_SIZE_ASSERT(ThumbnailGenerator, 0xf8)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10217da0
// `dynamic initializer for 'clickFunction''
// SYNTHETIC: WEBSERVICE 0x10217dc0
// `dynamic initializer for 'clickTextureFunction''
// SYNTHETIC: WEBSERVICE 0x10220fb0
// `dynamic atexit destructor for 'clickFunction''
// SYNTHETIC: WEBSERVICE 0x10220fc0
// `dynamic atexit destructor for 'clickTextureFunction''
// clang-format on

#endif // THUMBNAILGENERATOR_H
