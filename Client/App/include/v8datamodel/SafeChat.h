#ifndef V8DATAMODEL_SAFECHAT_H
#define V8DATAMODEL_SAFECHAT_H

#include "decomp.h"

#include <boost/scoped_ptr.hpp>
#include <string>
#include <vector>

class XmlElement;

namespace RBX {

// SIZE 0x2c
class ChatOption
{
public:
	ChatOption(std::string text);
	ChatOption();
	~ChatOption();

	std::vector<ChatOption*> children; // 0x00
	std::string text;                  // 0x10
};

DECOMP_SIZE_ASSERT(ChatOption, 0x2c)

// SIZE 0x4
class SafeChat
{
public:
	SafeChat();

	static SafeChat& singleton();

	ChatOption* getChatRoot() { return chatRoot.get(); }
	std::string getMessage(std::vector<std::string> path);

private:
	boost::scoped_ptr<ChatOption> chatRoot; // 0x00

	void loadChatTree();
	void saveChatTree();
	void loadChildren(ChatOption* parent, XmlElement* element);
};

DECOMP_SIZE_ASSERT(SafeChat, 0x4)

} // namespace RBX

// SYNTHETIC: WEBSERVICE 0x1021e750
// SYNTHETIC: ROBLOX 0x007ee220
// `dynamic initializer for 'utterance''
// SYNTHETIC: WEBSERVICE 0x1021e770
// `dynamic initializer for 'sing''
// SYNTHETIC: WEBSERVICE 0x10225560
// `dynamic atexit destructor for 'sing''

#endif // V8DATAMODEL_SAFECHAT_H
