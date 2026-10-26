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

#endif // THUMBNAILGENERATOR_H
