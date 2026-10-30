#ifndef GUI_PROFANITYFILTER_H
#define GUI_PROFANITYFILTER_H

#include "decomp.h"

#include <set>
#include <string>

namespace RBX {

// SIZE 0x0c
class WordList
{
private:
	std::set<std::string> blacklist; // 0x00

public:
	WordList();
	~WordList();

	bool ContainsProfanity(std::string p_unk0x04);
};

DECOMP_SIZE_ASSERT(WordList, 0x0c)

// SIZE 0x0c
class ProfanityFilter
{
private:
	WordList wordlist; // 0x00

public:
	ProfanityFilter();
	~ProfanityFilter();

	bool ContainsProfanity(std::string p_unk0x04);
};

DECOMP_SIZE_ASSERT(ProfanityFilter, 0x0c)

} // namespace RBX

#endif // GUI_PROFANITYFILTER_H
