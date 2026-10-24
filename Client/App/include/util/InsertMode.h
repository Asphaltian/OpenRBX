#ifndef UTIL_INSERTMODE_H
#define UTIL_INSERTMODE_H

namespace RBX {

enum InsertMode
{
	INSERT_RAW = 0,
	INSERT_TO_TREE = 1,
	INSERT_TO_3D_VIEW = 2,
};

enum PromptMode
{
	ALLOW_PROMPTS = 0,
	SUPPRESS_PROMPTS = 1,
};

} // namespace RBX

#endif // UTIL_INSERTMODE_H
