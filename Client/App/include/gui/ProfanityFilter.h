#ifndef GUI_PROFANITYFILTER_H
#define GUI_PROFANITYFILTER_H

#include "placeholder.h"

#include <set>
#include <string>

namespace RBX {

class WordList
{
private:
	std::set<std::string> blacklist;

public:
	WordList();
	~WordList();

	bool ContainsProfanity(std::string p_unk0x04);
};

class ProfanityFilter
{
private:
	WordList wordlist;

public:
	ProfanityFilter();
	~ProfanityFilter();

	bool ContainsProfanity(std::string p_unk0x04);
};

} // namespace RBX

#endif // GUI_PROFANITYFILTER_H
