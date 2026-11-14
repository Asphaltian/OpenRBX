#ifndef V8XML_XMLSERIALIZER_H
#define V8XML_XMLSERIALIZER_H

#include "placeholder.h"
#include "util/ContentProvider.h"
#include "util/Handle.h"

#include <boost/noncopyable.hpp>
#include <deque>
#include <iosfwd>
#include <map>
#include <memory>
#include <set>
#include <stack>
#include <string>

class XmlAttribute;
class XmlElement;

class __declspec(novtable) XmlWriter : public boost::noncopyable
{
public:
	virtual void serialize(const XmlElement* element) = 0;

	int getHandleIndex(RBX::InstanceHandle h)
	{
		std::map<RBX::InstanceHandle, int>::iterator iter = handles.find(h);

		if (iter == handles.end()) {
			int index = handles.size();

			handles[h] = index;

			return index;
		}

		return iter->second;
	}

protected:
	XmlWriter(std::ostream& stream);

	std::map<RBX::InstanceHandle, int> handles;
	std::ostream& stream;
};

class TextXmlWriter : public XmlWriter
{
public:
	TextXmlWriter(std::ostream& stream) : XmlWriter(stream) {}

	virtual void serialize(const XmlElement* xmlNode);

	static void encodedWrite(std::ostream& stream, const std::string& text) { encodedWrite(stream, text.c_str()); }

	static void encodedWrite(std::ostream& stream, const char* text);

protected:
	void writeOpenTag(const XmlElement* element, int depth, const XmlAttribute* extraAttribute);
	void writeCloseTag(const XmlElement* element, int depth);

	void serialize(const XmlElement* xmlNode, int depth);

	virtual void serializeNode(const XmlElement* xmlNode, int depth);
};

class TextXmlWriterWithEmbeddedContent : public TextXmlWriter
{
public:
	TextXmlWriterWithEmbeddedContent(std::ostream& stream) : TextXmlWriter(stream) {}

protected:
	virtual void serializeNode(const XmlElement* xmlNode, int depth);

private:
	std::set<RBX::ContentId> embeddedContent;
};

class XmlParser : public boost::noncopyable
{
public:
	virtual std::auto_ptr<XmlElement> parse() = 0;

protected:
	XmlParser(std::streambuf* buffer);

	std::streambuf* buffer;
	std::stack<XmlElement*, std::deque<XmlElement*> > elements;

public:
};

class TextXmlParser : public XmlParser
{
public:
	TextXmlParser(std::streambuf* buffer) : XmlParser(buffer) {}

	virtual std::auto_ptr<XmlElement> parse();

private:
	void skipWhitespace();
	std::string readTag();
	std::string readFirstTag();
	std::string readText(bool decode);
	std::string removeTag(const std::string& contents, int& index);
	std::string findNextToken(const std::string& contents, int& index);
	std::string findText(const std::string& tag);
	XmlElement* parseAttributes(const std::string& currentTag);

	std::map<std::string, std::string> legacyHashes;

public:
};

#endif // V8XML_XMLSERIALIZER_H
