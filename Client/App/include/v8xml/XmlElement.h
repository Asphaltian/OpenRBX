#ifndef V8XML_XMLELEMENT_H
#define V8XML_XMLELEMENT_H

#include "decomp.h"
#include "util/ContentProvider.h"
#include "util/Handle.h"

#include <cstddef>
#include <map>
#include <string>

namespace RBX {

class Instance;
class InstanceHandle;
class Name;

// SIZE 0x04
template <class T>
class Sibling
{
protected:
	Sibling() : next(NULL) {}

public:
	T* nextSibling() const { return next; }

private:
	template <class U>
	friend class Parent;

	void setNextSibling(T* value) { next = value; }

	T* next; // 0x00
};

// SIZE 0x08
template <class T>
class Parent
{
public:
	Parent() : first(NULL), last(NULL) {}

	T* firstChild() const { return first; }

	void addChild(T* child)
	{
		if (last == NULL) {
			first = child;
		}
		else {
			last->setNextSibling(child);
		}

		last = child;
	}

private:
	T* first; // 0x00
	T* last;  // 0x04
};

} // namespace RBX

class XmlWriter;

extern const RBX::Name& value_IDREF_null;
extern const RBX::Name& value_IDREF_nil;
extern const RBX::Name& name_xsinil;
extern const RBX::Name& name_xsitype;
extern const RBX::Name& tag_xmlnsxsi;
extern const RBX::Name& name_root;
extern const RBX::Name& name_referent;
extern const RBX::Name& name_DeleteItem;
extern const RBX::Name& tag_roblox;
extern const RBX::Name& tag_version;
extern const RBX::Name& tag_External;
extern const RBX::Name& name_Ref;
extern const RBX::Name& name_token;
extern const RBX::Name& name_name;
extern const RBX::Name& tag_Refs;
extern const RBX::Name& tag_X;
extern const RBX::Name& tag_Y;
extern const RBX::Name& tag_Z;
extern const RBX::Name& tag_R00;
extern const RBX::Name& tag_R01;
extern const RBX::Name& tag_R02;
extern const RBX::Name& tag_R10;
extern const RBX::Name& tag_R11;
extern const RBX::Name& tag_R12;
extern const RBX::Name& tag_R20;
extern const RBX::Name& tag_R21;
extern const RBX::Name& tag_R22;
extern const RBX::Name& tag_R;
extern const RBX::Name& tag_G;
extern const RBX::Name& tag_B;
extern const RBX::Name& tag_class;
extern const RBX::Name& tag_Item;
extern const RBX::Name& tag_Properties;
extern const RBX::Name& tag_Feature;
extern const RBX::Name& tag_hash;
extern const RBX::Name& tag_null;
extern const RBX::Name& tag_mimeType;
extern const RBX::Name& tag_xsinoNamespaceSchemaLocation;

// SIZE 0x10
class XmlNameValuePair
{
public:
	enum ValueType
	{
		NONE = 0,
		NAME = 1,
		STRING = 2,
		CONTENTID = 3,
		BOOL = 4,
		INT = 5,
		UINT = 6,
		FLOAT = 7,
		HANDLE = 8,
		DOUBLE = 9,
	};

	// FUNCTION: WEBSERVICE 0x10084a50
	// STUB: ROBLOX 0x0059d170 FOLDED
	XmlNameValuePair(const RBX::Name& tag, const std::string& text) : tag(tag), valueType(STRING)
	{
		stringValue = new std::string(text);
	}

	// FUNCTION: WEBSERVICE 0x10048150
	// FUNCTION: ROBLOX 0x00576600
	XmlNameValuePair(const RBX::Name& tag, RBX::InstanceHandle value)
		: tag(tag), valueType(HANDLE), handleValue(new RBX::InstanceHandle(value))
	{
	}

	XmlNameValuePair(const RBX::Name& tag, const RBX::Name* value) : tag(tag), valueType(NAME), nameValue(value) {}
	XmlNameValuePair(const RBX::Name& tag, const int value) : tag(tag), valueType(INT), intValue(value) {}
	XmlNameValuePair(const RBX::Name& tag, float value) : tag(tag), valueType(FLOAT), floatValue(value) {}
	XmlNameValuePair(const RBX::Name& tag, const char* text);

	// FUNCTION: WEBSERVICE 0x1003e8a0
	~XmlNameValuePair() { clearValue(); }

	const RBX::Name& getTag() const { return tag; }

	ValueType getValueType() const { return valueType; }

	bool getValue(std::string& value) const;
	bool getValue(const RBX::Name*& value) const;
	bool getValue(int& value) const;
	bool getValue(unsigned int& value) const;
	bool getValue(bool& value) const;
	bool getValue(float& value) const;
	bool getValue(double& value) const;
	bool getValue(RBX::ContentId& value) const;
	bool getValue(RBX::InstanceHandle& value) const;

	bool isValueEqual(const RBX::Name* value) const;

	// FUNCTION: WEBSERVICE 0x1003e8b0
	// STUB: ROBLOX 0x004af9f0
	void setValue(std::string value)
	{
		clearValue();

		stringValue = new std::string(value);
		valueType = STRING;
	}

	// FUNCTION: WEBSERVICE 0x1003eed0
	// FUNCTION: ROBLOX 0x0059dd80
	void setValue(RBX::ContentId contentId)
	{
		clearValue();

		contentIdValue = new RBX::ContentId(contentId);
		valueType = CONTENTID;
	}

	// FUNCTION: WEBSERVICE 0x1004b9a0
	// STUB: ROBLOX 0x004ab780
	void setValue(RBX::InstanceHandle handle)
	{
		clearValue();

		handleValue = new RBX::InstanceHandle(handle);
		valueType = HANDLE;
	}

	void setValue(const RBX::Name* value)
	{
		clearValue();

		nameValue = value;
		valueType = NAME;
	}

	void setValue(int value)
	{
		clearValue();

		intValue = value;
		valueType = INT;
	}

	void setValue(unsigned int value)
	{
		clearValue();

		uintValue = value;
		valueType = UINT;
	}

	void setValue(bool value)
	{
		clearValue();

		boolValue = value;
		valueType = BOOL;
	}

	void setValue(float value)
	{
		clearValue();

		floatValue = value;
		valueType = FLOAT;
	}

	void setValue(double value)
	{
		clearValue();

		doubleValue = value;
		valueType = DOUBLE;
	}

private:
	void clearValue() const;

public:
	std::string toString(XmlWriter* writer) const;

	void replaceHandles(const std::map<RBX::Instance*, RBX::InstanceHandle>& handleMap);

	template <class T>
	bool isValueType() const;

public:
	XmlNameValuePair(const RBX::Name& tag) : tag(tag), valueType(NONE) {}

private:
	const RBX::Name& tag;        // 0x00
	mutable ValueType valueType; // 0x04

	union // 0x08
	{
		mutable std::string* stringValue;
		mutable RBX::ContentId* contentIdValue;
		mutable bool boolValue;
		mutable int intValue;
		mutable unsigned int uintValue;
		mutable float floatValue;
		mutable double doubleValue;
		mutable const RBX::Name* nameValue;
		mutable RBX::InstanceHandle* handleValue;
	};
};

DECOMP_SIZE_ASSERT(XmlNameValuePair, 0x10)

// SIZE 0x18
class XmlAttribute : public RBX::Sibling<XmlAttribute>, public XmlNameValuePair
{
public:
	// SYNTHETIC: WEBSERVICE 0x10047a90
	// SYNTHETIC: ROBLOX 0x0040ffa0
	// XmlAttribute::~XmlAttribute

	// TEMPLATE: WEBSERVICE 0x10048d50
	// TEMPLATE: ROBLOX 0x00576fc0
	// XmlAttribute::XmlAttribute<RBX::InstanceHandle>

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10085670
	// STUB: ROBLOX 0x0059dd20
	// XmlAttribute::XmlAttribute<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >
	// clang-format on

	template <class T>
	XmlAttribute(const RBX::Name& tag, T value) : XmlNameValuePair(tag, value)
	{
	}
};

DECOMP_SIZE_ASSERT(XmlAttribute, 0x18)

// SIZE 0x28
class XmlElement : public RBX::Sibling<XmlElement>, public RBX::Parent<XmlElement>, public XmlNameValuePair
{
public:
	XmlElement(const RBX::Name& tag) : XmlNameValuePair(tag) {}

	// FUNCTION: WEBSERVICE 0x10048200
	~XmlElement()
	{
		XmlAttribute* attribute = attributes.firstChild();
		while (attribute) {
			XmlAttribute* old = attribute;
			attribute = attribute->nextSibling();
			delete old;
		}

		XmlElement* child = firstChild();
		while (child) {
			XmlElement* old = child;
			child = child->nextSibling();
			delete old;
		}
	}

	// TEMPLATE: WEBSERVICE 0x100489c0
	// TEMPLATE: ROBLOX 0x00576cd0
	// XmlElement::XmlElement<RBX::InstanceHandle>

	// TEMPLATE: WEBSERVICE 0x10190db0
	// STUB: ROBLOX 0x004afc20
	// XmlElement::XmlElement<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >

	template <class T>
	XmlElement(const RBX::Name& tag, T value) : XmlNameValuePair(tag, value)
	{
	}

	bool isXsiNil() const;

	XmlAttribute* getFirstAttribute() { return attributes.firstChild(); }
	const XmlAttribute* getFirstAttribute() const { return attributes.firstChild(); }

	XmlAttribute* getNextAttribute(XmlAttribute* attribute) { return attribute->nextSibling(); }

	const XmlAttribute* getNextAttribute(const XmlAttribute* attribute) const { return attribute->nextSibling(); }

	const XmlAttribute* findAttribute(const RBX::Name& name) const;
	XmlAttribute* findAttribute(const RBX::Name& name);

	bool findAttributeValue(const RBX::Name& name, std::string& value) const
	{
		const XmlAttribute* attribute = findAttribute(name);
		return attribute ? attribute->getValue(value) : false;
	}

	bool findAttributeValue(const RBX::Name& name, const RBX::Name*& value) const
	{
		const XmlAttribute* attribute = findAttribute(name);
		return attribute ? attribute->getValue(value) : false;
	}

	const XmlElement* findFirstChildByTag(const RBX::Name& _tag) const;
	const XmlElement* findNextChildWithSameTag(const XmlElement* node) const;

	XmlElement* addChild(XmlElement* child)
	{
		RBX::Parent<XmlElement>::addChild(child);
		return child;
	}

	// FUNCTION: WEBSERVICE 0x1003f890
	// FUNCTION: ROBLOX 0x004b0580
	XmlElement* addChild(const RBX::Name& tag)
	{
		XmlElement* child = new XmlElement(tag);
		addChild(child);
		return child;
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10048a70
	// TEMPLATE: ROBLOX 0x00576d80
	// XmlElement::addAttribute<RBX::Name const *>
	// TEMPLATE: WEBSERVICE 0x10049980
	// TEMPLATE: ROBLOX 0x005776a0
	// XmlElement::addAttribute<RBX::InstanceHandle>
	// TEMPLATE: WEBSERVICE 0x10085990
	// TEMPLATE: ROBLOX 0x0059e0d0
	// XmlElement::addAttribute<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >
	// TEMPLATE: WEBSERVICE 0x10096250
	// STUB: ROBLOX 0x005a1f60
	// XmlElement::addAttribute<char const *>
	// clang-format on
	template <class T>
	void addAttribute(const RBX::Name& _tag, T value)
	{
		addAttribute(new XmlAttribute(_tag, value));
	}

protected:
	void addAttribute(XmlAttribute* attribute) { attributes.addChild(attribute); }

private:
	RBX::Parent<XmlAttribute> attributes; // 0x20
};

DECOMP_SIZE_ASSERT(XmlElement, 0x28)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021a070
// `dynamic initializer for 'value_IDREF_null''
// SYNTHETIC: WEBSERVICE 0x1021a090
// SYNTHETIC: ROBLOX 0x007e93e0
// `dynamic initializer for 'value_IDREF_nil''
// SYNTHETIC: WEBSERVICE 0x1021a0b0
// SYNTHETIC: ROBLOX 0x007e9400
// `dynamic initializer for 'name_xsinil''
// SYNTHETIC: WEBSERVICE 0x1021a0d0
// SYNTHETIC: ROBLOX 0x007e9420
// `dynamic initializer for 'name_xsitype''
// SYNTHETIC: WEBSERVICE 0x1021a0f0
// SYNTHETIC: ROBLOX 0x007e9440
// `dynamic initializer for 'tag_xmlnsxsi''
// SYNTHETIC: WEBSERVICE 0x1021a110
// SYNTHETIC: ROBLOX 0x007e9460
// `dynamic initializer for 'name_root''
// SYNTHETIC: WEBSERVICE 0x1021a130
// SYNTHETIC: ROBLOX 0x007e9480
// `dynamic initializer for 'name_referent''
// SYNTHETIC: WEBSERVICE 0x1021a150
// SYNTHETIC: ROBLOX 0x007e94a0
// `dynamic initializer for 'name_DeleteItem''
// SYNTHETIC: WEBSERVICE 0x1021a170
// SYNTHETIC: ROBLOX 0x007e94c0
// `dynamic initializer for 'tag_roblox''
// SYNTHETIC: WEBSERVICE 0x1021a190
// SYNTHETIC: ROBLOX 0x007e94e0
// `dynamic initializer for 'tag_version''
// SYNTHETIC: WEBSERVICE 0x1021a1b0
// SYNTHETIC: ROBLOX 0x007e9500
// `dynamic initializer for 'tag_External''
// SYNTHETIC: WEBSERVICE 0x1021a1d0
// SYNTHETIC: ROBLOX 0x007e9520
// `dynamic initializer for 'name_Ref''
// SYNTHETIC: WEBSERVICE 0x1021a1f0
// SYNTHETIC: ROBLOX 0x007e9540
// `dynamic initializer for 'name_token''
// SYNTHETIC: WEBSERVICE 0x1021a210
// SYNTHETIC: ROBLOX 0x007e9560
// `dynamic initializer for 'name_name''
// SYNTHETIC: WEBSERVICE 0x1021a230
// SYNTHETIC: ROBLOX 0x007e9580
// `dynamic initializer for 'tag_Refs''
// SYNTHETIC: WEBSERVICE 0x1021a250
// SYNTHETIC: ROBLOX 0x007e95a0
// `dynamic initializer for 'tag_X''
// SYNTHETIC: WEBSERVICE 0x1021a270
// SYNTHETIC: ROBLOX 0x007e95c0
// `dynamic initializer for 'tag_Y''
// SYNTHETIC: WEBSERVICE 0x1021a290
// SYNTHETIC: ROBLOX 0x007e95e0
// `dynamic initializer for 'tag_Z''
// SYNTHETIC: WEBSERVICE 0x1021a2b0
// SYNTHETIC: ROBLOX 0x007e9600
// `dynamic initializer for 'tag_R00''
// SYNTHETIC: WEBSERVICE 0x1021a2d0
// SYNTHETIC: ROBLOX 0x007e9620
// `dynamic initializer for 'tag_R01''
// SYNTHETIC: WEBSERVICE 0x1021a2f0
// SYNTHETIC: ROBLOX 0x007e9640
// `dynamic initializer for 'tag_R02''
// SYNTHETIC: WEBSERVICE 0x1021a310
// SYNTHETIC: ROBLOX 0x007e9660
// `dynamic initializer for 'tag_R10''
// SYNTHETIC: WEBSERVICE 0x1021a330
// SYNTHETIC: ROBLOX 0x007e9680
// `dynamic initializer for 'tag_R11''
// SYNTHETIC: WEBSERVICE 0x1021a350
// SYNTHETIC: ROBLOX 0x007e96a0
// `dynamic initializer for 'tag_R12''
// SYNTHETIC: WEBSERVICE 0x1021a370
// SYNTHETIC: ROBLOX 0x007e96c0
// `dynamic initializer for 'tag_R20''
// SYNTHETIC: WEBSERVICE 0x1021a390
// SYNTHETIC: ROBLOX 0x007e96e0
// `dynamic initializer for 'tag_R21''
// SYNTHETIC: WEBSERVICE 0x1021a3b0
// SYNTHETIC: ROBLOX 0x007e9700
// `dynamic initializer for 'tag_R22''
// SYNTHETIC: WEBSERVICE 0x1021a3d0
// SYNTHETIC: ROBLOX 0x007e9720
// `dynamic initializer for 'tag_R''
// SYNTHETIC: WEBSERVICE 0x1021a3f0
// SYNTHETIC: ROBLOX 0x007e9740
// `dynamic initializer for 'tag_G''
// SYNTHETIC: WEBSERVICE 0x1021a410
// SYNTHETIC: ROBLOX 0x007e9760
// `dynamic initializer for 'tag_B''
// SYNTHETIC: WEBSERVICE 0x1021a430
// SYNTHETIC: ROBLOX 0x007e9780
// `dynamic initializer for 'tag_class''
// SYNTHETIC: WEBSERVICE 0x1021a450
// SYNTHETIC: ROBLOX 0x007e97a0
// `dynamic initializer for 'tag_Item''
// SYNTHETIC: WEBSERVICE 0x1021a470
// SYNTHETIC: ROBLOX 0x007e97c0
// `dynamic initializer for 'tag_Properties''
// SYNTHETIC: WEBSERVICE 0x1021a490
// SYNTHETIC: ROBLOX 0x007e97e0
// `dynamic initializer for 'tag_Feature''
// SYNTHETIC: WEBSERVICE 0x1021a4b0
// SYNTHETIC: ROBLOX 0x007e9800
// `dynamic initializer for 'tag_hash''
// SYNTHETIC: WEBSERVICE 0x1021a4d0
// `dynamic initializer for 'tag_null''
// SYNTHETIC: WEBSERVICE 0x1021a4f0
// SYNTHETIC: ROBLOX 0x007e9840
// `dynamic initializer for 'tag_mimeType''
// SYNTHETIC: WEBSERVICE 0x1021a510
// SYNTHETIC: ROBLOX 0x007e9860
// `dynamic initializer for 'tag_xsinoNamespaceSchemaLocation''
// clang-format on

#endif // V8XML_XMLELEMENT_H
