#include "gui/ProfanityFilter.h"

#include "compat.h"
#include "util/ContentProvider.h"

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <vector>

namespace RBX {

// FUNCTION: ROBLOX 0x00666de0
bool WordList::ContainsProfanity(std::string p_unk0x04)
{
	boost::algorithm::to_lower(p_unk0x04);
	return blacklist.find(p_unk0x04) != blacklist.end();
}

// FUNCTION: ROBLOX 0x00666f70 FOLDED
WordList::~WordList()
{
}

// FUNCTION: ROBLOX 0x00666fb0
WordList::WordList()
{
	std::string fileName = ContentProvider::singleton().getAssetFile("Fonts\\diogenes.fnt");
	std::ifstream file(fileName.c_str());
	std::string line;

	while (std::getline(file, line, '\n')) {
		boost::algorithm::to_lower(line);
		boost::algorithm::trim(line);
		blacklist.insert(line);
	}

	file.close();
}

// FUNCTION: ROBLOX 0x006672c0
ProfanityFilter::ProfanityFilter()
{
}

// FUNCTION: ROBLOX 0x006672d0
bool ProfanityFilter::ContainsProfanity(std::string p_unk0x04)
{
	boost::algorithm::to_lower(p_unk0x04);

	std::vector<std::string> words;
	boost::algorithm::split(words, p_unk0x04, boost::algorithm::is_any_of(" !.?,:;><[]{}|\\/@#$%^&*()_-+=\""));

	for (unsigned int i = 0; i < words.size(); i++) {
		if (wordlist.ContainsProfanity(words[i])) {
			return true;
		}
	}

	return false;
}

// FUNCTION: ROBLOX 0x00666f70 FOLDED
ProfanityFilter::~ProfanityFilter()
{
}

} // namespace RBX
