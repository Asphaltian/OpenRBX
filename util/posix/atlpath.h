#ifndef POSIX_ATLPATH_H
#define POSIX_ATLPATH_H

#include <atlstr.h>
#include <string>
#include <sys/stat.h>
#include <windows.h>

class CPath
{
public:
	CPath() {}
	CPath(const char* path) : m_strPath(normalize(path)) {}
	CPath(const CString& path) : m_strPath(normalize(path)) {}

	BOOL Append(const char* more)
	{
		std::string s = (const char*) m_strPath;
		if (!s.empty() && s[s.size() - 1] != '/') {
			s += '/';
		}
		s += normalize(more);
		m_strPath = s.c_str();
		return TRUE;
	}

	void AddBackslash()
	{
		std::string s = (const char*) m_strPath;
		if (s.empty() || s[s.size() - 1] != '/') {
			s += '/';
		}
		m_strPath = s.c_str();
	}

	BOOL FileExists() const
	{
		struct stat st;
		return stat(m_strPath, &st) == 0;
	}

	BOOL IsDirectory() const
	{
		struct stat st;
		return stat(m_strPath, &st) == 0 && S_ISDIR(st.st_mode);
	}

	BOOL IsRelative() const
	{
		const char* s = m_strPath;
		return s[0] != '/';
	}

	operator const char*() const { return m_strPath; }

	CString m_strPath;

private:
	static std::string normalize(const char* path)
	{
		std::string s = path ? path : "";
		for (size_t i = 0; i < s.size(); ++i) {
			if (s[i] == '\\') {
				s[i] = '/';
			}
		}
		return s;
	}
};

#endif // POSIX_ATLPATH_H
