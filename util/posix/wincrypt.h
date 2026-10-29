#ifndef POSIX_WINCRYPT_H
#define POSIX_WINCRYPT_H

#include <G3D/Crypto.h>
#include <string>
#include <windows.h>

typedef uintptr_t HCRYPTPROV;
typedef uintptr_t HCRYPTHASH;
typedef uintptr_t HCRYPTKEY;
typedef unsigned int ALG_ID;

#define PROV_RSA_FULL 1
#define CRYPT_VERIFYCONTEXT 0xF0000000
#define CALG_MD5 0x00008003
#define HP_HASHVAL 0x0002
#define HP_HASHSIZE 0x0004

inline BOOL CryptAcquireContext(HCRYPTPROV* provider, LPCSTR, LPCSTR, DWORD, DWORD)
{
	*provider = 1;
	return TRUE;
}

inline BOOL CryptReleaseContext(HCRYPTPROV, DWORD)
{
	return TRUE;
}

inline BOOL CryptCreateHash(HCRYPTPROV, ALG_ID algorithm, HCRYPTKEY, DWORD, HCRYPTHASH* hash)
{
	if (algorithm != CALG_MD5) {
		return FALSE;
	}
	*hash = (HCRYPTHASH) new std::string();
	return TRUE;
}

inline BOOL CryptHashData(HCRYPTHASH hash, const BYTE* data, DWORD length, DWORD)
{
	((std::string*) hash)->append((const char*) data, length);
	return TRUE;
}

inline BOOL CryptGetHashParam(HCRYPTHASH hash, DWORD param, BYTE* data, DWORD* length, DWORD)
{
	if (param == HP_HASHSIZE) {
		*(DWORD*) data = 16;
		*length = sizeof(DWORD);
		return TRUE;
	}
	if (param == HP_HASHVAL) {
		const std::string& bytes = *(std::string*) hash;
		G3D::MD5Hash digest = G3D::Crypto().md5(bytes.data(), bytes.size());
		for (int i = 0; i < 16; ++i) {
			data[i] = digest[i];
		}
		*length = 16;
		return TRUE;
	}
	return FALSE;
}

#endif // POSIX_WINCRYPT_H
