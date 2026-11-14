#ifndef UTIL_VECTOR6_H
#define UTIL_VECTOR6_H

template <class T>
class Vector6
{
	T data[6];

public:
	Vector6(const T& value)
	{
		for (int i = 0; i < 6; ++i) {
			data[i] = value;
		}
	}

	Vector6() {}

	T& operator[](int index) { return data[index]; }

	const T& operator[](int index) const { return data[index]; }
};

#endif // UTIL_VECTOR6_H
