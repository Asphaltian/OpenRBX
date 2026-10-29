#ifndef POSIX_ATLSTR_H
#define POSIX_ATLSTR_H

#include <stdarg.h>
#include <string>
#include <vector>
#include <windows.h>

class CString
{
public:
	CString() {}
	CString(const char* s) : str(s ? s : "") {}
	CString(const wchar_t* s)
	{
		if (s) {
			while (*s) {
				str += (char) *s++;
			}
		}
	}
	CString(const std::string& s) : str(s) {}

	void Format(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		char buffer[4096];
		vsnprintf(buffer, sizeof(buffer), format, args);
		va_end(args);
		str = buffer;
	}

	char* GetBuffer(int minLength)
	{
		buffer.assign(str.begin(), str.end());
		buffer.resize(minLength > (int) str.size() ? minLength + 1 : str.size() + 1, 0);
		return &buffer[0];
	}

	void ReleaseBuffer(int length = -1)
	{
		if (!buffer.empty()) {
			str = length < 0 ? std::string(&buffer[0]) : std::string(&buffer[0], length);
		}
		buffer.clear();
	}

	void MakeLower()
	{
		sync();
		for (size_t i = 0; i < str.size(); ++i) {
			str[i] = (char) tolower(str[i]);
		}
	}

	int Find(const char* s, int start = 0) const
	{
		size_t at = current().find(s, start);
		return at == std::string::npos ? -1 : (int) at;
	}

	int Find(char c, int start = 0) const
	{
		size_t at = current().find(c, start);
		return at == std::string::npos ? -1 : (int) at;
	}

	CString Right(int count) const
	{
		const std::string& s = current();
		return count >= (int) s.size() ? CString(s) : CString(s.substr(s.size() - count));
	}

	bool operator==(const char* other) const { return current() == other; }
	bool operator!=(const char* other) const { return current() != other; }

	int GetLength() const { return (int) current().size(); }
	bool IsEmpty() const { return current().empty(); }

	operator const char*() const { return current().c_str(); }

	CString& operator=(const char* s)
	{
		buffer.clear();
		str = s ? s : "";
		return *this;
	}

	CString& operator+=(const char* s)
	{
		sync();
		str += s;
		return *this;
	}

	CString& operator+=(const CString& s)
	{
		sync();
		str += s.current();
		return *this;
	}

private:
	const std::string& current() const
	{
		if (!buffer.empty()) {
			cached = std::string(&buffer[0]);
			return cached;
		}
		return str;
	}

	void sync()
	{
		if (!buffer.empty()) {
			str = std::string(&buffer[0]);
			buffer.clear();
		}
	}

	std::string str;
	std::vector<char> buffer;
	mutable std::string cached;
};

#endif // POSIX_ATLSTR_H
