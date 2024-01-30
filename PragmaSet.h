#ifndef __PRAGMA_SET_H__
#define __PRAGMA_SET_H__
#pragma once

#if _WIN32
    // VC10 Migration: Memory leaks
    #ifdef _DEBUG
        #pragma comment(lib, "mfc140ud.lib")
    #endif
    #pragma message("Pragma warnings and messages included")
#endif

// Disabling
#pragma warning (disable : 4100) // The given formal parameter was never referenced in the body of the function for which it was declared
#pragma warning (disable : 4201) // nonstandard extension used : nameless struct/union
#pragma warning (disable : 4239) // nonstandard extension used: 'argument' : conversion
#pragma warning (disable : 4238) // nonstandard extension used : class rvalue used as lvalue
#pragma warning (disable : 4310) // cast truncates constant value
#pragma warning (disable : 4503) // 'insert_unique' : decorated name length exceeded, name was truncated
#pragma warning (disable : 4786) // disables warning: identifier was truncated to '255' characters in the browser information
#pragma warning (disable : 4663) // warning C++ language change
#pragma warning (disable : 4701) // local variable may be used without having been initialized
#pragma warning (disable : 4834) // discarding return value of function with 'nodiscard' attribute
#pragma warning (disable : 4996) // 'function' : This function or variable may be unsafe.

// Set as Error
#pragma warning (error : 4018)  // signed/unsigned mismatch
#pragma warning (error : 4267)  // 'argument': conversion from 'type1' to 'type2', possible loss of data
#pragma warning (error : 4456)  // error if hides previous local declaration
#pragma warning (error : 4189)  // identifier : local variable is initialized but not referenced"
#pragma warning (error : 4700)  // local variable 'name' used without having been initialized
#pragma warning (error : 4101)  // 'identifier' : unreferenced local variable
#pragma warning (error : 4858)  // discarding return value: The 'remove' and 'remove_if' algorithms return the iterator past the last element that should be kept
 
#ifndef _AFXDLL
    // warning C4710: 'std::_Exception_ptr std::_Exception_ptr::_Current_exception(void)' : function not inlined
    #pragma warning(disable: 4710)
    // warning C4512: 'stlp_std::pair<_T1,_T2>' : assignment operator could not be generated
    #pragma warning(disable: 4512)
    // non dll-interface class used as base for dll-interface 
    #pragma warning (disable : 4275)
    // needs to have dll-interface to be used by clients of class
    #pragma warning (disable : 4251)
#else
    #pragma warning(default: 4710) //'function' : function not inlined
#endif

#pragma warning (default: 4714)  // 'function' : function marked as __forceinline not inlined

// we do no not flollow MS standart 
// strcpy, fopen and other (ANSI standart) "unsafe" functions are legal.
#define _CRT_SECURE_NO_DEPRECATE

#pragma warning (disable : 4995) // disable depreciation of 'CDaoDatabase'
#pragma warning (disable : 4996) // disable depreciation of 'stricmp'

#define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers

#include <stdint.h>

#endif