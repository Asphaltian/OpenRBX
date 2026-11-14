#include "gui/ProfanityFilter.h"

#include "compat.h"
#include "util/ContentProvider.h"

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <vector>

namespace RBX {

bool WordList::ContainsProfanity(std::string p_unk0x04)
{
	boost::algorithm::to_lower(p_unk0x04);
	return blacklist.find(p_unk0x04) != blacklist.end();
}

WordList::~WordList()
{
}

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

ProfanityFilter::ProfanityFilter()
{
}

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

ProfanityFilter::~ProfanityFilter()
{
}

} // namespace RBX
