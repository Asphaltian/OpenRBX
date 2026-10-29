#include "FileMesh.h"

#include "util/ContentProvider.h"

#include <stdio.h>
#include <vector>

namespace RBX {
namespace View {

// FUNCTION: WEBSERVICE 0x1018a8f0
// FUNCTION: ROBLOX 0x00494bd0
bool FileMesh::loadFromMeshFile(const G3D::Vector3& scale, const MeshId meshFile)
{
	try {
		std::string filename;
		if (!ContentProvider::singleton().requestContentFile(meshFile, filename)) {
			return false;
		}

		LevelRef level = new Level(G3D::RenderDevice::TRIANGLES);
		levels.append(level);

		std::vector<unsigned int> verts;

		FILE* f = fopen(filename.c_str(), "r");

		fscanf(f, "version 1.00\n");
		int num_faces;
		fscanf(f, "%d\n", &num_faces);

		for (int i = 0; i < num_faces; i++) {
			for (int v = 0; v < 3; v++) {
				float vx, vy, vz;
				float nx, ny, nz;
				float tu, tv, tw;
				fscanf(f, "[%f,%f,%f][%f,%f,%f][%f,%f,%f]", &vx, &vy, &vz, &nx, &ny, &nz, &tu, &tv, &tw);

				G3D::Vector3 vertex(vx * 0.5, vy * 0.5, vz * 0.5);

				verts.push_back(allocVertex(scale * vertex, G3D::Vector3(nx, ny, nz), G3D::Vector2(tu, 1 - tv), true));
			}

			level->indexArray.append(allocVertex(verts[0], 1), allocVertex(verts[1], 1), allocVertex(verts[2], 1));

			for (int j = 0; j < 3; j++) {
				freeVertex(verts[j]);
			}
			verts.clear();
		}

		fclose(f);
	}
	catch (...) {
		return false;
	}

	return true;
}

// FUNCTION: WEBSERVICE 0x1018ad50
// FUNCTION: ROBLOX 0x00495030
G3D::ReferenceCountedPointer<Render::Mesh> FileMesh::create(const G3D::Vector3& scale, const MeshId meshFile)
{
	FileMesh* mesh = new FileMesh();
	G3D::ReferenceCountedPointer<Render::Mesh> result = mesh;

	if (!mesh->loadFromMeshFile(scale, meshFile)) {
		return NULL;
	}

	return result;
}

} // namespace View
} // namespace RBX
