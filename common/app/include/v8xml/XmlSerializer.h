#ifndef V8XML_XMLSERIALIZER_H
#define V8XML_XMLSERIALIZER_H

#include "decomp.h"
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

// SIZE 0x14
class __declspec(novtable) XmlWriter : public boost::noncopyable
{
public:
	virtual void serialize(const XmlElement* element) = 0; // vtable+0x00

	// FUNCTION: WEBSERVICE 0x1008fd40
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

	std::map<RBX::InstanceHandle, int> handles; // 0x04
	std::ostream& stream;                       // 0x10
};

DECOMP_SIZE_ASSERT(XmlWriter, 0x14)

// SIZE 0x14
class TextXmlWriter : public XmlWriter
{
public:
	TextXmlWriter(std::ostream& stream) : XmlWriter(stream) {}

	virtual void serialize(const XmlElement* element); // vtable+0x00

	static void encodedWrite(std::ostream& stream, const std::string& text) { encodedWrite(stream, text.c_str()); }

	static void encodedWrite(std::ostream& stream, const char* text);

protected:
	void writeOpenTag(const XmlElement* element, int depth, const XmlAttribute* extraAttribute);
	void writeCloseTag(const XmlElement* element, int depth);

	DECOMP_NOINLINE void serialize(const XmlElement* xmlNode, int depth);

	virtual void serializeNode(const XmlElement* xmlNode, int depth); // vtable+0x04
};

DECOMP_SIZE_ASSERT(TextXmlWriter, 0x14)

// SIZE 0x20
class TextXmlWriterWithEmbeddedContent : public TextXmlWriter
{
public:
	TextXmlWriterWithEmbeddedContent(std::ostream& stream) : TextXmlWriter(stream) {}

protected:
	virtual void serializeNode(const XmlElement* xmlNode, int depth); // vtable+0x04

private:
	std::set<RBX::ContentId> embeddedContent; // 0x14
};

DECOMP_SIZE_ASSERT(TextXmlWriterWithEmbeddedContent, 0x20)

// SIZE 0x1c
class XmlParser : public boost::noncopyable
{
public:
	virtual std::auto_ptr<XmlElement> parse() = 0; // vtable+0x00

protected:
	XmlParser(std::streambuf* buffer);

	std::streambuf* buffer;                                     // 0x04
	std::stack<XmlElement*, std::deque<XmlElement*> > elements; // 0x08
};

DECOMP_SIZE_ASSERT(XmlParser, 0x1c)

// SIZE 0x28
class TextXmlParser : public XmlParser
{
public:
	TextXmlParser(std::streambuf* buffer) : XmlParser(buffer) {}

	virtual std::auto_ptr<XmlElement> parse();

private:
	void skipWhitespace();

	std::map<std::string, std::string> legacyHashes; // 0x1c
};

DECOMP_SIZE_ASSERT(TextXmlParser, 0x28)

#endif // V8XML_XMLSERIALIZER_H
