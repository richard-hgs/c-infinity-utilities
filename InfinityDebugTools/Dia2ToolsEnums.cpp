// #include "stdafx.h"				// CABECALHO PADRAO DO APLICATIVO

#include "Dia2ToolsEnums.h"		// CABECALHO DA CLASSE

// Basic types
const wchar_t* const Dia2ToolsEnums::rgBaseType[35] =
{
  L"<NoType>",                         // btNoType = 0,
  L"void",                             // btVoid = 1,
  L"char",                             // btChar = 2,
  L"wchar_t",                          // btWChar = 3,
  L"signed char",
  L"unsigned char",
  L"int",                              // btInt = 6,
  L"unsigned int",                     // btUInt = 7,
  L"float",                            // btFloat = 8,
  L"<BCD>",                            // btBCD = 9,
  L"bool",                             // btBool = 10,
  L"short",
  L"unsigned short",
  L"long",                             // btLong = 13,
  L"unsigned long",                    // btULong = 14,
  L"__int8",
  L"__int16",
  L"__int32",
  L"__int64",
  L"__int128",
  L"unsigned __int8",
  L"unsigned __int16",
  L"unsigned __int32",
  L"unsigned __int64",
  L"unsigned __int128",
  L"<currency>",                       // btCurrency = 25,
  L"<date>",                           // btDate = 26,
  L"VARIANT",                          // btVariant = 27,
  L"<complex>",                        // btComplex = 28,
  L"<bit>",                            // btBit = 29,
  L"BSTR",                             // btBSTR = 30,
  L"HRESULT",                          // btHresult = 31
  L"char16_t",                         // btChar16 = 32
  L"char32_t"                          // btChar32 = 33
  L"char8_t",                          // btChar8  = 34
};

// Tags returned by Dia
const wchar_t* const Dia2ToolsEnums::rgTags[43] =
{
  L"(SymTagNull)",                     // SymTagNull
  L"Executable (Global)",              // SymTagExe
  L"Compiland",                        // SymTagCompiland
  L"CompilandDetails",                 // SymTagCompilandDetails
  L"CompilandEnv",                     // SymTagCompilandEnv
  L"Function",                         // SymTagFunction
  L"Block",                            // SymTagBlock
  L"Data",                             // SymTagData
  L"Annotation",                       // SymTagAnnotation
  L"Label",                            // SymTagLabel
  L"PublicSymbol",                     // SymTagPublicSymbol
  L"UserDefinedType",                  // SymTagUDT
  L"Enum",                             // SymTagEnum
  L"FunctionType",                     // SymTagFunctionType
  L"PointerType",                      // SymTagPointerType
  L"ArrayType",                        // SymTagArrayType
  L"BaseType",                         // SymTagBaseType
  L"Typedef",                          // SymTagTypedef
  L"BaseClass",                        // SymTagBaseClass
  L"Friend",                           // SymTagFriend
  L"FunctionArgType",                  // SymTagFunctionArgType
  L"FuncDebugStart",                   // SymTagFuncDebugStart
  L"FuncDebugEnd",                     // SymTagFuncDebugEnd
  L"UsingNamespace",                   // SymTagUsingNamespace
  L"VTableShape",                      // SymTagVTableShape
  L"VTable",                           // SymTagVTable
  L"Custom",                           // SymTagCustom
  L"Thunk",                            // SymTagThunk
  L"CustomType",                       // SymTagCustomType
  L"ManagedType",                      // SymTagManagedType
  L"Dimension",                        // SymTagDimension
  L"CallSite",                         // SymTagCallSite
  L"InlineSite",                       // SymTagInlineSite
  L"BaseInterface",                    // SymTagBaseInterface
  L"VectorType",                       // SymTagVectorType
  L"MatrixType",                       // SymTagMatrixType
  L"HLSLType",                         // SymTagHLSLType
  L"Caller",                           // SymTagCaller,
  L"Callee",                           // SymTagCallee,
  L"Export",                           // SymTagExport,
  L"HeapAllocationSite",               // SymTagHeapAllocationSite
  L"CoffGroup",                        // SymTagCoffGroup
  L"Inlinee",                          // SymTagInlinee
};


// Processors
const wchar_t* const Dia2ToolsEnums::rgFloatPackageStrings[4] =
{
  L"hardware processor (80x87 for Intel processors)",    // CV_CFL_NDP
  L"emulator",                                           // CV_CFL_EMU
  L"altmath",                                            // CV_CFL_ALT
  L"???"
};

const wchar_t* const Dia2ToolsEnums::rgProcessorStrings[257] =
{
  L"8080",                             //  CV_CFL_8080
  L"8086",                             //  CV_CFL_8086
  L"80286",                            //  CV_CFL_80286
  L"80386",                            //  CV_CFL_80386
  L"80486",                            //  CV_CFL_80486
  L"Pentium",                          //  CV_CFL_PENTIUM
  L"Pentium Pro/Pentium II",           //  CV_CFL_PENTIUMII/CV_CFL_PENTIUMPRO
  L"Pentium III",                      //  CV_CFL_PENTIUMIII
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"MIPS (Generic)",                   //  CV_CFL_MIPSR4000
  L"MIPS16",                           //  CV_CFL_MIPS16
  L"MIPS32",                           //  CV_CFL_MIPS32
  L"MIPS64",                           //  CV_CFL_MIPS64
  L"MIPS I",                           //  CV_CFL_MIPSI
  L"MIPS II",                          //  CV_CFL_MIPSII
  L"MIPS III",                         //  CV_CFL_MIPSIII
  L"MIPS IV",                          //  CV_CFL_MIPSIV
  L"MIPS V",                           //  CV_CFL_MIPSV
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"M68000",                           //  CV_CFL_M68000
  L"M68010",                           //  CV_CFL_M68010
  L"M68020",                           //  CV_CFL_M68020
  L"M68030",                           //  CV_CFL_M68030
  L"M68040",                           //  CV_CFL_M68040
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"Alpha 21064",                      // CV_CFL_ALPHA, CV_CFL_ALPHA_21064
  L"Alpha 21164",                      // CV_CFL_ALPHA_21164
  L"Alpha 21164A",                     // CV_CFL_ALPHA_21164A
  L"Alpha 21264",                      // CV_CFL_ALPHA_21264
  L"Alpha 21364",                      // CV_CFL_ALPHA_21364
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"PPC 601",                          // CV_CFL_PPC601
  L"PPC 603",                          // CV_CFL_PPC603
  L"PPC 604",                          // CV_CFL_PPC604
  L"PPC 620",                          // CV_CFL_PPC620
  L"PPC w/FP",                         // CV_CFL_PPCFP
  L"PPC (Big Endian)",                 // CV_CFL_PPCBE
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"SH3",                              // CV_CFL_SH3
  L"SH3E",                             // CV_CFL_SH3E
  L"SH3DSP",                           // CV_CFL_SH3DSP
  L"SH4",                              // CV_CFL_SH4
  L"SHmedia",                          // CV_CFL_SHMEDIA
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"ARM3",                             // CV_CFL_ARM3
  L"ARM4",                             // CV_CFL_ARM4
  L"ARM4T",                            // CV_CFL_ARM4T
  L"ARM5",                             // CV_CFL_ARM5
  L"ARM5T",                            // CV_CFL_ARM5T
  L"ARM6",                             // CV_CFL_ARM6
  L"ARM (XMAC)",                       // CV_CFL_ARM_XMAC
  L"ARM (WMMX)",                       // CV_CFL_ARM_WMMX
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"Omni",                             // CV_CFL_OMNI
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"Itanium",                          // CV_CFL_IA64, CV_CFL_IA64_1
  L"Itanium (McKinley)",               // CV_CFL_IA64_2
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"CEE",                              // CV_CFL_CEE
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"AM33",                             // CV_CFL_AM33
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"M32R",                             // CV_CFL_M32R
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"TriCore",                          // CV_CFL_TRICORE
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"x64",                              // CV_CFL_X64
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"EBC",                              // CV_CFL_EBC
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"Thumb (CE)",                       // CV_CFL_THUMB
  L"???",
  L"???",
  L"???",
  L"ARM",                              // CV_CFL_ARMNT
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"???",
  L"D3D11_SHADE",                      // CV_CFL_D3D11_SHADER
};

const wchar_t* const Dia2ToolsEnums::rgDataKind[10] =
{
  L"Unknown",
  L"Local",
  L"Static Local",
  L"Param",
  L"Object Ptr",
  L"File Static",
  L"Global",
  L"Member",
  L"Static Member",
  L"Constant",
};

const wchar_t* const Dia2ToolsEnums::rgUdtKind[4] =
{
  L"struct",
  L"class",
  L"union",
  L"interface",
};

const wchar_t* const Dia2ToolsEnums::rgAccess[4] =
{
  L"",                     // No access specifier
  L"private",
  L"protected",
  L"public"
};

const wchar_t* const Dia2ToolsEnums::rgCallingConvention[26] =
{
  L"CV_CALL_NEAR_C      ",
  L"CV_CALL_FAR_C       ",
  L"CV_CALL_NEAR_PASCAL ",
  L"CV_CALL_FAR_PASCAL  ",
  L"CV_CALL_NEAR_FAST   ",
  L"CV_CALL_FAR_FAST    ",
  L"CV_CALL_SKIPPED     ",
  L"CV_CALL_NEAR_STD    ",
  L"CV_CALL_FAR_STD     ",
  L"CV_CALL_NEAR_SYS    ",
  L"CV_CALL_FAR_SYS     ",
  L"CV_CALL_THISCALL    ",
  L"CV_CALL_MIPSCALL    ",
  L"CV_CALL_GENERIC     ",
  L"CV_CALL_ALPHACALL   ",
  L"CV_CALL_PPCCALL     ",
  L"CV_CALL_SHCALL      ",
  L"CV_CALL_ARMCALL     ",
  L"CV_CALL_AM33CALL    ",
  L"CV_CALL_TRICALL     ",
  L"CV_CALL_SH5CALL     ",
  L"CV_CALL_M32RCALL    ",
  L"CV_ALWAYS_INLINED   ",
  L"CV_CALL_NEAR_VECTOR ",
  L"CV_CALL_SWIFT       ",
  L"CV_CALL_RESERVED    "
};

const wchar_t* const Dia2ToolsEnums::rgLanguage[20] =
{
  L"C",                                // CV_CFL_C
  L"C++",                              // CV_CFL_CXX
  L"FORTRAN",                          // CV_CFL_FORTRAN
  L"MASM",                             // CV_CFL_MASM
  L"Pascal",                           // CV_CFL_PASCAL
  L"Basic",                            // CV_CFL_BASIC
  L"COBOL",                            // CV_CFL_COBOL
  L"LINK",                             // CV_CFL_LINK
  L"CVTRES",                           // CV_CFL_CVTRES
  L"CVTPGD",                           // CV_CFL_CVTPGD
  L"C#",                               // CV_CFL_CSHARP
  L"Visual Basic",                     // CV_CFL_VB
  L"ILASM",                            // CV_CFL_ILASM
  L"Java",                             // CV_CFL_JAVA
  L"JScript",                          // CV_CFL_JSCRIPT
  L"MSIL",                             // CV_CFL_MSIL
  L"HLSL",                             // CV_CFL_HLSL
  L"Objective-C",                      // CV_CFL_OBJC
  L"Objective-C++",                    // CV_CFL_OBJCXX
  L"Swift",                            // CV_CFL_SWIFT
};

const wchar_t* const Dia2ToolsEnums::rgLocationTypeString[12] =
{
  L"NULL",
  L"static",
  L"TLS",
  L"RegRel",
  L"ThisRel",
  L"Enregistered",
  L"BitField",
  L"Slot",
  L"IL Relative",
  L"In MetaData",
  L"Constant",
  L"RegRelAliasIndir"
};