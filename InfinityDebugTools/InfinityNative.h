#pragma once

// ASSEMBLY IMPLEMENTATIONS
// OBS: Needs to be tested in multiple versions of Windows operating system
namespace InfinityNative {
	// ================================= x64 =================================
	// Get current thread id
	extern "C" DWORD ASMX64GetCurrentThreadId();
	// Get the bottom frame of current stack
	extern "C" void** ASMX64GetStackFrameBottom();
	// Get Rbp
	extern "C" void*** ASMX64GetRbp();
	// Get Rsp
	extern "C" PVOID64 ASMX64GetRsp();
	// Get Peb
	extern "C" PVOID64 ASMX64GetPeb();
	// ================================= x86 =================================
	// Get current thread id
	extern "C" DWORD ASMX86GetCurrentThreadId();
	// Get the bottom frame of current stack
	extern "C" void** ASMX86GetStackFrameBottom();
	// Assembly printf example
	extern "C" void ASMX86PrintfExample(DWORD address);
	// Get TEB
	extern "C" void* ASMX86GetTeb();

}