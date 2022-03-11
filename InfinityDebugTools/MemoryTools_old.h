#pragma once

#include "debug.h"

#ifndef MEMORY_TOOLS_H
	#define MEMORY_TOOLS_H

	#ifdef DEBUG_MEM
		// #define new new(__FILE__, __LINE__)
		// #define delete delete(__FILE__, __LINE__)
	// #define delete debug_mem_allocation(__FILE__, __LINE__, __FUNCTION__) >> delete
	#endif // DEBUG_MEM

#endif // MEMORY_TOOLS_H