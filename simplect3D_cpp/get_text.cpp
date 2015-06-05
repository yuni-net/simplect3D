#include <util_MMD.h>

namespace si3
{
	std::string get_text(const char * words, const int buffer_byte)
	{
		char * pool = new char[buffer_byte+1];

		memcpy(pool, words, buffer_byte);
		pool[buffer_byte] = '\0';

		delete [] pool;
		pool = nullptr;

		return std::string(pool);
	}
}