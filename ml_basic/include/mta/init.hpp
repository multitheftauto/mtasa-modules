#pragma once

#include <cstring>

namespace Helper {
#define _F static inline
#ifndef _WIN32
	_F char* strncpy_s(
		char* destination,
		const char* source,
		std::size_t count
	) {
		return ::strncpy(destination, source, count);
	}
	_F char* strncpy_s(char* destination, std::size_t size, const char* source) {
		return ::strncpy(destination, source, size);
	}
	_F char* strcpy_s(char* destination, const char* source) {
		return ::strcpy(destination, source);
	}
	_F char* strcpy_s(char* destination, std::size_t size, const char* source) {
		return ::strcpy(destination, source);
	}
#else
	_F errno_t strncpy_s(char* destination, const char* source, std::size_t size) {
		return ::strncpy_s(destination, strlen(source) + 1, source, size);
	}
	_F errno_t strncpy_s(char* destination, std::size_t size, const char* source) {
		return ::strncpy_s(destination, size, source, size);
	}
	_F errno_t strcpy_s(char* destination, std::size_t size, const char* source) {
		return ::strcpy_s(destination, size, source);
	}
#endif
#undef _F
}