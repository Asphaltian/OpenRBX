#ifndef V8DATAMODEL_SAFECHAT_H
#define V8DATAMODEL_SAFECHAT_H

#include "placeholder.h"

#include <boost/scoped_ptr.hpp>
#include <string>
#include <vector>

class XmlElement;

namespace RBX {

class ChatOption
{
public:
	ChatOption(std::string text);
	ChatOption();
	~ChatOption();

	std::vector<ChatOption*> children;
	std::string text;
};

class SafeChat
{
public:
	SafeChat();

	static SafeChat& singleton();

	ChatOption* getChatRoot() { return chatRoot.get(); }
	std::string getMessage(std::vector<std::string> path);

private:
	boost::scoped_ptr<ChatOption> chatRoot;

	void loadChatTree();
	void saveChatTree();
	void loadChildren(ChatOption* parent, XmlElement* element);
};

} // namespace RBX

#endif // V8DATAMODEL_SAFECHAT_H
