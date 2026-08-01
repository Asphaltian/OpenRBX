#ifndef V8XML_XMLELEMENT_H
#define V8XML_XMLELEMENT_H

#include "decomp.h"

#include <cstddef>
#include <set>

namespace RBX {

// SIZE 0x04
template <class T>
class Sibling
{
public:
	T* nextSibling() const { return next; }
	void setNextSibling(T* value) { next = value; }

private:
	T* next; // 0x00
};

// SIZE 0x08
template <class T>
class Parent
{
public:
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

// SIZE 0x10
class XmlNameValuePair
{
private:
	undefined m_unk0x00[0x10 - 0x00]; // 0x00
};

DECOMP_SIZE_ASSERT(XmlNameValuePair, 0x10)

// SIZE 0x28
class XmlElement : public RBX::Sibling<XmlElement>, public RBX::Parent<XmlElement>, public XmlNameValuePair
{
private:
	std::set<XmlNameValuePair*> attributes; // 0x20
};

DECOMP_SIZE_ASSERT(XmlElement, 0x28)

#endif // V8XML_XMLELEMENT_H
