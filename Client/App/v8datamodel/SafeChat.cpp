#include "v8datamodel/SafeChat.h"

#include "util/Name.h"

#include <algorithm>
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
ChatOption::~ChatOption()
{
	std::for_each(children.begin(), children.end(), deleteChatOption);
}

} // namespace RBX
