#pragma once

#include "debug.h"

#ifndef MEMORY_TOOLS_H
	#define MEMORY_TOOLS_H

	#ifdef DEBUG_MEM
		#pragma warning(push)
		#pragma warning(disable: 4595)
		#pragma warning(disable: 4290)
		inline void* operator new(size_t size, const char* filename, int line) throw(std::bad_alloc) {
			return _dbgnew(size, filename, line);
		}
		#define new new(__FILE__, __LINE__)
	
		inline void operator delete(void* ptr) {
			_dbgdelete(ptr);
		}
	
		#define delete _dbgdeletep(__FILE__, __LINE__), delete
		#pragma warning(pop)
	#endif // DEBUG_MEM

#endif // MEMORY_TOOLS_H