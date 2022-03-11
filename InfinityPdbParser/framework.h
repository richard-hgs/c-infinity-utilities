#pragma once

#pragma once
#pragma warning( disable:4996 )

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <comutil.h>
#include <xstring> 
#include <string> 
#include <map>
#include <list>
#include <vector>

using namespace std;

#include <comdef.h>
#pragma warning(disable:4995)
#include <crtdbg.h>

//	This is our interface to parse PDB files
#include "PdbParser.h"
using namespace PdbParser;

// Debug Interface Access (DIA) specific
#include "dia2.h"