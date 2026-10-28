#include "v8xml/XmlSerializer.h"

#include "util/ContentProvider.h"
#include "util/Name.h"
#include "util/base64.hpp"
#include "v8xml/XmlElement.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iterator>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>

// GLOBAL: WEBSERVICE 0x102d21b0
// clang-format off
static bool whitespaces[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1
};
// clang-format on

// FUNCTION: WEBSERVICE 0x10084b30
bool isCloseTag(const std::string& test)
{
	if (test.length() >= 2) {
		return test.substr(0, 2) == "</";
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x10084bc0
bool endsWithClose(const std::string& test)
{
	size_t length = test.length();
	if (length >= 2) {
		return test.substr(length - 2, 2) == "/>";
	}

	return false;
}

// FUNCTION: WEBSERVICE 0x10084c50
void TextXmlParser::skipWhitespace()
{
	while (whitespaces[(char) buffer->sgetc()]) {
		buffer->sbumpc();
	}
}

// FUNCTION: WEBSERVICE 0x10084c90
std::string TextXmlParser::readFirstTag()
{
	skipWhitespace();

	int count = 0;
	char c;
	do {
		if (count++ > 4) {
			std::string message = "tag expected after Byte-Order-Mark";
			throw std::runtime_error(message);
		}

		c = buffer->sbumpc();
		if (buffer->sgetc() == EOF) {
			throw std::runtime_error("Expected '<' but got EOF in Xml stream");
		}
	} while (c != '<');

	std::string result;
	result += '<';
	do {
		c = buffer->sbumpc();
		if (buffer->sgetc() == EOF) {
			throw std::runtime_error("Expected '>' but got EOF in Xml stream");
		}
		result += c;
	} while (c != '>');
	return result;
}

// FUNCTION: WEBSERVICE 0x10084e20
std::string TextXmlParser::readTag()
{
	skipWhitespace();

	char c = buffer->sbumpc();
	if (c != '<') {
		throw std::runtime_error("tag expected");
	}

	std::string result;
	result += '<';
	do {
		c = buffer->sbumpc();
		result += c;
	} while (c != '>');
	return result;
}

// FUNCTION: WEBSERVICE 0x10084ef0
std::string decodeString(std::string source)
{
	std::string result;
	unsigned int i = 0;
	while (i < source.size()) {
		char c = source[i++];
		if (c == '&') {
			std::string entity;
			while (i < source.size()) {
				c = source[i++];
				if (c == ';') {
					break;
				}
				entity += c;
			}

			if (entity == "lt") {
				result += '<';
			}
			else if (entity == "gt") {
				result += '>';
			}
			else if (entity == "amp") {
				result += '&';
			}
			else if (entity == "quot") {
				result += '"';
			}
			else if (entity == "apos") {
				result += '\'';
			}
			else if (entity[0] == '#') {
				result += (char) atoi(entity.substr(1).c_str());
			}
		}
		else {
			result += c;
		}
	}
	return result;
}

// FUNCTION: WEBSERVICE 0x10085160
std::string TextXmlParser::readText(bool decode)
{
	skipWhitespace();

	std::string result;
	while ((char) buffer->sgetc() != '<') {
		result += (char) buffer->sbumpc();
	}

	if (decode) {
		result = decodeString(result);
	}

	return result;
}

// FUNCTION: WEBSERVICE 0x10085230
std::string TextXmlParser::removeTag(const std::string& contents, int& index)
{
	unsigned int start = 0;
	while (whitespaces[contents[start]] && start < contents.size()) {
		start++;
	}

	unsigned int end = start;
	while (!whitespaces[contents[end]] && end < contents.size()) {
		end++;
	}

	std::string result = contents.substr(start, end - start);
	index = end + 1;
	return result;
}

// FUNCTION: WEBSERVICE 0x100852d0
std::string TextXmlParser::findNextToken(const std::string& contents, int& index)
{
	unsigned int start = index;
	while (start < contents.size() && whitespaces[contents[start]]) {
		start++;
	}

	bool inQuotes = false;
	unsigned int end;
	for (end = start; end < contents.size(); end++) {
		if (contents[end] == '"') {
			inQuotes = !inQuotes;
		}
		else if (!inQuotes && whitespaces[contents[end]]) {
			break;
		}
	}

	if (end != start) {
		std::string answer = contents.substr(start, end - start);
		index = end + 1;
		return answer;
	}

	index = contents.size();
	return "";
}

// FUNCTION: WEBSERVICE 0x10085810
void TextXmlWriter::writeCloseTag(const XmlElement* element, int depth)
{
	for (int i = 0; i < depth; ++i) {
		stream << '\t';
	}

	stream << "</" << element->getTag().name << '>';
}

// FUNCTION: WEBSERVICE 0x10085870
void TextXmlWriter::serialize(const XmlElement* xmlNode, int depth)
{
	if (xmlNode != NULL) {
		serializeNode(xmlNode, depth);

		const XmlElement* child = xmlNode->firstChild();

		if (child != NULL) {
			while (child != NULL) {
				stream << '\n';
				serialize(child, depth + 1);
				child = child->nextSibling();
			}

			stream << '\n';

			writeCloseTag(xmlNode, depth);
		}
		else {
			writeCloseTag(xmlNode, 0);
		}
	}
}

// FUNCTION: WEBSERVICE 0x10086220
void TextXmlWriter::encodedWrite(std::ostream& stream, const char* text)
{
	size_t num = strlen(text);

	for (size_t i = 0; i < num; ++i) {
		char c = *text++;

		if (c == '<') {
			stream << "&lt;";
		}
		else if (c == '>') {
			stream << "&gt;";
		}
		else if (c == '&') {
			stream << "&amp;";
		}
		else if (c == '"') {
			stream << "&quot;";
		}
		else if (c == '\'') {
			stream << "&apos;";
		}
		else if (c < ' ' || c > '~') {
			char buffer[8];

			sprintf(buffer, "&#%d;", (unsigned char) c);

			stream << buffer;
		}
		else {
			stream << c;
		}
	}
}

// FUNCTION: WEBSERVICE 0x10086310
XmlElement* TextXmlParser::parseAttributes(const std::string& currentTag)
{
	std::string tagGuts = currentTag.substr(1, currentTag.size() - 2);

	int index = 0;
	std::string tagName = removeTag(tagGuts, index);
	XmlElement* element = new XmlElement(RBX::Name::lookup(tagName));

	std::string attribute = findNextToken(tagGuts, index);
	while (attribute.size() > 0) {
		std::string tag = attribute.substr(0, attribute.find("="));

		size_t start = attribute.find('"');
		size_t end = attribute.rfind('"');
		std::string text = decodeString(attribute.substr(start + 1, end - start - 1));

		element->addAttribute(RBX::Name::lookup(tag), text);
		attribute = findNextToken(tagGuts, index);
	}

	return element;
}

// FUNCTION: WEBSERVICE 0x10086590
void TextXmlWriter::serialize(const XmlElement* xmlNode)
{
	serialize(xmlNode, 0);
}

// FUNCTION: WEBSERVICE 0x10086af0
void TextXmlWriter::writeOpenTag(const XmlElement* element, int depth, const XmlAttribute* extraAttribute)
{
	for (int i = 0; i < depth; ++i) {
		stream << '\t';
	}

	stream << '<' << element->getTag().name;

	for (const XmlAttribute* attribute = element->getFirstAttribute(); attribute != NULL;
		 attribute = element->getNextAttribute(attribute)) {
		stream << ' ' << attribute->getTag().name << "=\"";

		encodedWrite(stream, attribute->toString(this));

		stream << '"';
	}

	if (extraAttribute != NULL) {
		stream << ' ' << extraAttribute->getTag().name << "=\"";

		encodedWrite(stream, extraAttribute->toString(this));

		stream << '"';
	}

	stream << '>';
}

// FUNCTION: WEBSERVICE 0x10086c90
XmlParser::XmlParser(std::streambuf* buffer) : buffer(buffer)
{
}

// FUNCTION: WEBSERVICE 0x10086cb0
void TextXmlWriter::serializeNode(const XmlElement* xmlNode, int depth)
{
	if (xmlNode->isValueType<RBX::ContentId>()) {
		writeOpenTag(xmlNode, depth, NULL);

		RBX::ContentId contentId;

		xmlNode->getValue(contentId);

		if (contentId.isNull()) {
			stream << "<null></null>";
		}
		else {
			stream << "<hash>";
			stream << xmlNode->toString(this);
			stream << "</hash>";
		}
	}
	else {
		writeOpenTag(xmlNode, depth, NULL);

		encodedWrite(stream, xmlNode->toString(this));
	}
}

// STUB: WEBSERVICE 0x10086e00
void TextXmlWriterWithEmbeddedContent::serializeNode(const XmlElement* xmlNode, int depth)
{
	if (xmlNode->isValueType<RBX::ContentId>()) {
		RBX::ContentId contentId;

		xmlNode->getValue(contentId);

		if (&contentId.mimeType() == &RBX::Name::getNullName()) {
			writeOpenTag(xmlNode, depth, NULL);
		}
		else {
			XmlAttribute mimeType(tag_mimeType, contentId.mimeType().name.c_str());

			writeOpenTag(xmlNode, depth, &mimeType);
		}

		if (xmlNode->findAttribute(name_xsinil) == NULL) {
			if (contentId.isNull()) {
				stream << "<null></null>";
			}
			else if (contentId.isAsset() || contentId.isHttp()) {
				stream << "<url>";
				TextXmlWriter::encodedWrite(stream, contentId.c_str());
				stream << "</url>";
			}
			else if (embeddedContent.find(contentId) == embeddedContent.end()) {
				embeddedContent.insert(contentId);

				std::auto_ptr<std::istream> contentStream;

				contentStream = RBX::ContentProvider::singleton().getContent(contentId);

				if (&contentId.mimeType() == &RBX::Name::getNullName()) {
					stream << "<binary>";
				}
				else {
					stream << "<binary xmime:contentType=\"" << contentId.mimeType().name.c_str() << "\">";
				}

				int state;
				base64<char> encoder;

				encoder.put(
					std::istreambuf_iterator<char>(contentStream->rdbuf()),
					std::istreambuf_iterator<char>(),
					std::ostreambuf_iterator<char>(stream.rdbuf()),
					state,
					base64<char>::crlf()
				);

				stream << "</binary>";
			}
			else {
				embeddedContent.insert(contentId);

				stream << "<hash>";
				stream << xmlNode->toString(this);
				stream << "</hash>";
			}
		}
	}
	else {
		TextXmlWriter::serializeNode(xmlNode, depth);
	}
}

// STUB: WEBSERVICE 0x10087650
std::auto_ptr<XmlElement> TextXmlParser::parse()
{
	STUB(0x10087650);
	return std::auto_ptr<XmlElement>();
}

// FUNCTION: WEBSERVICE 0x10087f40
XmlWriter::XmlWriter(std::ostream& stream) : stream(stream)
{
}
