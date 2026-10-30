#include "v8datamodel/SafeChat.h"

#include "util/Name.h"

#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/thread/once.hpp>

static const RBX::Name& utterance = RBX::Name::declare("utterance", -1);

static boost::scoped_ptr<RBX::SafeChat> sing;

static boost::once_flag once_SafeChat_singleton = BOOST_ONCE_INIT;

namespace RBX {

static void deleteChatOption(ChatOption* option)
{
	delete option;
}

// FUNCTION: WEBSERVICE 0x10120d60
// FUNCTION: ROBLOX 0x00660f20
ChatOption::~ChatOption()
{
	std::for_each(children.begin(), children.end(), deleteChatOption);
}

// FUNCTION: ROBLOX 0x00661200
std::string SafeChat::getMessage(std::vector<std::string> path)
{
	ChatOption* option = chatRoot.get();

	for (unsigned int i = 1; i < path.size(); i++) {
		int index = boost::lexical_cast<int>(path[i]);

		if (index >= option->children.size()) {
			return "";
		}

		option = option->children[index];
	}

	return option->text;
}

// STUB: ROBLOX 0x00661330
void SafeChat::loadChatTree()
{
	STUB(0x661330);
}

SafeChat::SafeChat()
{
	loadChatTree();
}

// STUB: ROBLOX 0x006614b0
static void SafeChat_singleton()
{
	sing.reset(new SafeChat());
}

// FUNCTION: ROBLOX 0x00661550
SafeChat& SafeChat::singleton()
{
	boost::call_once(SafeChat_singleton, once_SafeChat_singleton);
	return *sing;
}

} // namespace RBX
