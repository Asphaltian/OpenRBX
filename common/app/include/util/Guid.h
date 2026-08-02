#ifndef UTIL_GUID_H
#define UTIL_GUID_H

namespace RBX {

class Name;

class Guid
{
public:
	struct Data
	{
		const Name* scope; // 0x00
		int index;         // 0x04
	};

	Guid();

	void assign(Data data);

	static int compare(const Guid* a, const Guid* b);
	static int compare(const Guid* a0, const Guid* a1, const Guid* b0, const Guid* b1);

	Data data; // 0x00
};

template <class T>
class GuidItem
{
public:
	virtual ~GuidItem() {}

private:
	bool registeredGuid; // 0x04
	Guid guid;           // 0x08
};

} // namespace RBX

#endif // UTIL_GUID_H
