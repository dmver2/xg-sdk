/*	$Id: Util.h,v 1.1 2005/11/14 17:07:59 dmitry.vereshchagin Exp $
**	Game SDK
**
*/

#pragma once

#include <decl6n.h>
#include <unordered_map>

namespace GSDK
{

	class Util
	{
		DECLARE_NO_INSTANCE(Util)
		DECLARE_NO_COPY(Util)

	public:
		static inline size_t hash_code(uint x)
		{
			x += (x << 12);
			x ^= (x >> 22);
			x += (x << 4);
			x ^= (x >> 9);
			x += (x << 10);
			x ^= (x >> 2);
			x += (x << 7);
			x ^= (x >> 12);
			return x;
		}

		static inline void rol_hash(size_t& hash, TCHAR ch) {
/*
#ifdef WIN32
			__asm
			{
				rol  hash, 3
			}
#else
			hash = (hash << 3) | (hash >> 29);
#endif
*/
			hash = (hash << 3) | (hash >> 29);
			hash ^= ch;
		}

		static inline size_t hash_code(const string& s)
		{
			size_t hash = 0;
			const TCHAR* pch_end = s.c_str() + s.length();
			for (const TCHAR* pch = s.c_str(); pch != pch_end; ++pch)
			{
				rol_hash(hash,*pch);
			}
			return hash;
		}

		static inline size_t hash_code(const TCHAR* s)
		{
			size_t hash = 0;
			for (const TCHAR* pch = s; *pch; ++pch)
			{
				rol_hash(hash, *pch);
			}
			return hash;
		}


		struct int_hash_comparator : public std::hash_compare<int>
		{
			size_t operator()(const int& keyval) const
			{
				return hash_code(keyval);
			}

			bool operator()(const int& keyval1, const int& keyval2) const
			{
				return keyval1 < keyval2;
			}

		};

		struct string_hash_comparator : public std::hash_compare<string>
		{
			size_t operator()(const string& keyval) const
			{
				return hash_code(keyval);
			}

			bool operator()(const int& keyval1, const int& keyval2) const
			{
				return keyval1 < keyval2;
			}

		};

	};

} // namespace