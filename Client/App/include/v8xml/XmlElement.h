#ifndef V8XML_XMLELEMENT_H
#define V8XML_XMLELEMENT_H

#include "decomp.h"
#include "util/Handle.h"

#include <cstddef>
#include <map>
#include <string>

namespace RBX {

class ContentId;
class Instance;
class InstanceHandle;
class Name;

// SIZE 0x04
template <class T>
class Sibling
{
public:
	Sibling() : next(NULL) {}

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

	XmlNameValuePair(const RBX::Name& tag, const char* text);

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

	void setValue(RBX::InstanceHandle value)
	{
		clearValue();

		handleValue = new RBX::InstanceHandle(value);
		valueType = HANDLE;
	}

private:
	void clearValue() const;

public:
	std::string toString(XmlWriter* writer) const;

	void replaceHandles(const std::map<RBX::Instance*, RBX::InstanceHandle>& handleMap);

	template <class T>
	bool isValueType() const;

	template <class T>
	XmlNameValuePair(const RBX::Name& tag, T value) : tag(tag)
	{
		setValue(value);
	}

protected:
	XmlNameValuePair(const RBX::Name& tag) : tag(tag), valueType(NONE) {}

	void setValue(const RBX::Name* value)
	{
		valueType = NAME;
		nameValue = value;
	}

	void setValue(int value)
	{
		valueType = INT;
		intValue = value;
	}

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

	const XmlElement* findFirstChildByTag(const RBX::Name& _tag) const;
	const XmlElement* findNextChildWithSameTag(const XmlElement* node) const;

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10096250
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
// `dynamic initializer for 'value_IDREF_nil''
// SYNTHETIC: WEBSERVICE 0x1021a0b0
// `dynamic initializer for 'name_xsinil''
// SYNTHETIC: WEBSERVICE 0x1021a0d0
// `dynamic initializer for 'name_xsitype''
// SYNTHETIC: WEBSERVICE 0x1021a0f0
// `dynamic initializer for 'tag_xmlnsxsi''
// SYNTHETIC: WEBSERVICE 0x1021a110
// `dynamic initializer for 'name_root''
// SYNTHETIC: WEBSERVICE 0x1021a130
// `dynamic initializer for 'name_referent''
// SYNTHETIC: WEBSERVICE 0x1021a150
// `dynamic initializer for 'name_DeleteItem''
// SYNTHETIC: WEBSERVICE 0x1021a170
// `dynamic initializer for 'tag_roblox''
// SYNTHETIC: WEBSERVICE 0x1021a190
// `dynamic initializer for 'tag_version''
// SYNTHETIC: WEBSERVICE 0x1021a1b0
// `dynamic initializer for 'tag_External''
// SYNTHETIC: WEBSERVICE 0x1021a1d0
// `dynamic initializer for 'name_Ref''
// SYNTHETIC: WEBSERVICE 0x1021a1f0
// `dynamic initializer for 'name_token''
// SYNTHETIC: WEBSERVICE 0x1021a210
// `dynamic initializer for 'name_name''
// SYNTHETIC: WEBSERVICE 0x1021a230
// `dynamic initializer for 'tag_Refs''
// SYNTHETIC: WEBSERVICE 0x1021a250
// `dynamic initializer for 'tag_X''
// SYNTHETIC: WEBSERVICE 0x1021a270
// `dynamic initializer for 'tag_Y''
// SYNTHETIC: WEBSERVICE 0x1021a290
// `dynamic initializer for 'tag_Z''
// SYNTHETIC: WEBSERVICE 0x1021a2b0
// `dynamic initializer for 'tag_R00''
// SYNTHETIC: WEBSERVICE 0x1021a2d0
// `dynamic initializer for 'tag_R01''
// SYNTHETIC: WEBSERVICE 0x1021a2f0
// `dynamic initializer for 'tag_R02''
// SYNTHETIC: WEBSERVICE 0x1021a310
// `dynamic initializer for 'tag_R10''
// SYNTHETIC: WEBSERVICE 0x1021a330
// `dynamic initializer for 'tag_R11''
// SYNTHETIC: WEBSERVICE 0x1021a350
// `dynamic initializer for 'tag_R12''
// SYNTHETIC: WEBSERVICE 0x1021a370
// `dynamic initializer for 'tag_R20''
// SYNTHETIC: WEBSERVICE 0x1021a390
// `dynamic initializer for 'tag_R21''
// SYNTHETIC: WEBSERVICE 0x1021a3b0
// `dynamic initializer for 'tag_R22''
// SYNTHETIC: WEBSERVICE 0x1021a3d0
// `dynamic initializer for 'tag_R''
// SYNTHETIC: WEBSERVICE 0x1021a3f0
// `dynamic initializer for 'tag_G''
// SYNTHETIC: WEBSERVICE 0x1021a410
// `dynamic initializer for 'tag_B''
// SYNTHETIC: WEBSERVICE 0x1021a430
// `dynamic initializer for 'tag_class''
// SYNTHETIC: WEBSERVICE 0x1021a450
// `dynamic initializer for 'tag_Item''
// SYNTHETIC: WEBSERVICE 0x1021a470
// `dynamic initializer for 'tag_Properties''
// SYNTHETIC: WEBSERVICE 0x1021a490
// `dynamic initializer for 'tag_Feature''
// SYNTHETIC: WEBSERVICE 0x1021a4b0
// `dynamic initializer for 'tag_hash''
// SYNTHETIC: WEBSERVICE 0x1021a4d0
// `dynamic initializer for 'tag_null''
// SYNTHETIC: WEBSERVICE 0x1021a4f0
// `dynamic initializer for 'tag_mimeType''
// SYNTHETIC: WEBSERVICE 0x1021a510
// `dynamic initializer for 'tag_xsinoNamespaceSchemaLocation''
// clang-format on

#endif // V8XML_XMLELEMENT_H
