#include "v8datamodel/Message.h"

#include "reflection/Property.h"

namespace RBX {

char sHint[] = "Hint";

char sMessage[] = "Message";

static Reflection::PropDescriptor<Message, std::string> desc_Text;

// FUNCTION: WEBSERVICE 0x100eebb0
void Message::setText(const std::string& value)
{
	if (text != value) {
		text = value;
		raisePropertyChanged(desc_Text);
	}
}

} // namespace RBX
