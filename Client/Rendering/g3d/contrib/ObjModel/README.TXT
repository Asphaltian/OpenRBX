Summary:

	This is meant to be a maya compatible .obj loader.  Currently, it requires that you triangulate the object before exporting.  Later versions will do directy polygon loading/rendering but that version is not publicly available.

	This version also loads the corresponding .mtl file.  You could modify this to allow the coder to specify mtl files, but make sure they correspond to the correct vertices  somehow.

Things to look out for:

	1)	The .mtl data might be misleading.  The specular/ambient/etc data is read as-is,  which may not be correct.

	2)	Currently, G3D enables GL_COLOR_MATERIAL. This means that the color setting affects  both diffuse and ambient settings directly.  This is a good setting for most things, and you can set specular/shininess/(and emissive if you like) separately or with GMaterial.  I  added that functionality just to fit the 6.02 model.

		I would not change this unless you need something like a color map.  In that case, I'd  recommend a pixel or vertex program written in Cg or HLSL.  Check out http://developer.nvidia.com/ for FX Composer and other great shader tools.

	4)	The loader does create a new object once a face line has been skipped(e.g. encounters an empty line).  If you are writing a custom material loader, such as multiple GMaterial's or custom .mtl files, be sure your .obj files are not adversely modified.

Corey Taylor
laundrypowder@spamcop.net