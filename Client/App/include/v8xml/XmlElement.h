#ifndef V8XML_XMLELEMENT_H
#define V8XML_XMLELEMENT_H

#include "placeholder.h"
#include "util/ContentProvider.h"
#include "util/Handle.h"

#include <cstddef>
#include <map>
#include <string>

namespace RBX {

class Instance;
class InstanceHandle;
class Name;

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

	T* next;
};

template <class T>
class Parent
{
public:
	Parent() : first(NULL), last(NULL) {}

	T* firstChild() const { return first; }

	void pushBackChild(T* child)
	{
		if (last == NULL) {
			first = child;
		}
		else {
			last->setNextSibling(child);
		}

		last = child;
	}

	void pushFrontChild(T* child)
	{
		if (first == NULL) {
			last = child;
		}
		else {
			child->setNextSibling(first);
		}

		first = child;
	}

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
	T* first;
	T* last;
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

	XmlNameValuePair(const RBX::Name& tag, const std::string& text) : tag(tag), valueType(STRING)
	{
		stringValue = new std::string(text);
	}

	XmlNameValuePair(const RBX::Name& tag, RBX::InstanceHandle value)
		: tag(tag), valueType(HANDLE), handleValue(new RBX::InstanceHandle(value))
	{
	}

	XmlNameValuePair(const RBX::Name& tag, const RBX::Name* value) : tag(tag), valueType(NAME), nameValue(value) {}
	XmlNameValuePair(const RBX::Name& tag, const int value) : tag(tag), valueType(INT), intValue(value) {}
	XmlNameValuePair(const RBX::Name& tag, float value) : tag(tag), valueType(FLOAT), floatValue(value) {}
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

	void setValue(std::string value)
	{
		clearValue();

		stringValue = new std::string(value);
		valueType = STRING;
	}

	void setValue(RBX::ContentId contentId)
	{
		clearValue();

		contentIdValue = new RBX::ContentId(contentId);
		valueType = CONTENTID;
	}

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
	const RBX::Name& tag;
	mutable ValueType valueType;

	union {
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

class XmlAttribute : public RBX::Sibling<XmlAttribute>, public XmlNameValuePair
{
public:
	template <class T>
	XmlAttribute(const RBX::Name& tag, T value) : XmlNameValuePair(tag, value)
	{
	}
};

class XmlElement : public RBX::Sibling<XmlElement>, public RBX::Parent<XmlElement>, public XmlNameValuePair
{
public:
	XmlElement(const RBX::Name& tag) : XmlNameValuePair(tag) {}

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

	XmlElement* addChild(const RBX::Name& tag)
	{
		XmlElement* child = new XmlElement(tag);
		addChild(child);
		return child;
	}

	template <class T>
	void addAttribute(const RBX::Name& _tag, T value)
	{
		addAttribute(new XmlAttribute(_tag, value));
	}

protected:
	void addAttribute(XmlAttribute* attribute) { attributes.addChild(attribute); }

private:
	RBX::Parent<XmlAttribute> attributes;
};

#endif // V8XML_XMLELEMENT_H
