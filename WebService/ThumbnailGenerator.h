#ifndef THUMBNAILGENERATOR_H
#define THUMBNAILGENERATOR_H

#include "placeholder.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <string>

namespace G3D {
class BinaryOutput;
}

extern const char sThumbnailGenerator[];

class ThumbnailGenerator : public RBX::DescribedCreatable<ThumbnailGenerator, RBX::Instance, sThumbnailGenerator>,
						   public RBX::Service
{
public:
	ThumbnailGenerator();
	virtual ~ThumbnailGenerator() {}

	std::string click(std::string fileType, int cx, int cy, bool hideSky);
	std::string clickTexture(std::string textureId, std::string fileType, int cx, int cy);
	void doClick(std::string fileType, int cx, int cy, bool hideSky, G3D::BinaryOutput* binaryOutput);
};

#endif // THUMBNAILGENERATOR_H
