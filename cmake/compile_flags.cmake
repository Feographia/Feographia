# ****************************************************************************
#   Project:  Feographia
#   Purpose:  The application to work with the biblical text
#   Author:   NikitaFeodonit, nfeodonit@yandex.com
# ****************************************************************************
#     Copyright (c) 2017-2020 NikitaFeodonit
#
#     This file is part of the Feographia project.
#
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published
#     by the Free Software Foundation, either version 3 of the License,
#     or (at your option) any later version.
#
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#     See the GNU General Public License for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with this program. If not, see <http://www.gnu.org/licenses/>.
# ****************************************************************************

# Used by LibCMaker_Boost.
set(CMAKE_CXX_STANDARD 17)

if(NOT BUILD_SHARED_LIBS)
  option(cmr_USE_STATIC_RUNTIME "cmr_USE_STATIC_RUNTIME" ON)
endif()

# Set compile flags.
if(MSVC)
  # Determine MSVC runtime library flag
  set(MSVC_LIB_USE "/MD")
  set(MSVC_LIB_REPLACE "/MT")
  if(cmr_USE_STATIC_RUNTIME AND NOT BUILD_SHARED_LIBS)
    set(MSVC_LIB_USE "/MT")
    set(MSVC_LIB_REPLACE "/MD")
  endif()
  # Set MSVC runtime flags for all configurations
  # See:
  # https://stackoverflow.com/a/20804336
  # https://stackoverflow.com/a/14172871
  foreach(cfg "" ${CMAKE_CONFIGURATION_TYPES})
    set(c_flag_var CMAKE_C_FLAGS)
    set(cxx_flag_var CMAKE_CXX_FLAGS)
    if(cfg)
      string(TOUPPER ${cfg} cfg_upper)
      set(c_flag_var   "${c_flag_var}_${cfg_upper}")
      set(cxx_flag_var "${cxx_flag_var}_${cfg_upper}")
    endif()
    if(${c_flag_var} MATCHES ${MSVC_LIB_REPLACE})
      string(REPLACE
        ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${c_flag_var} "${${c_flag_var}}"
      )
      set(${c_flag_var} ${${c_flag_var}} CACHE STRING
        "Flags used by the C compiler during ${cfg_upper} builds." FORCE
      )
    endif()
    if(${cxx_flag_var} MATCHES ${MSVC_LIB_REPLACE})
      string(REPLACE
        ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${cxx_flag_var} "${${cxx_flag_var}}"
      )
      set(${cxx_flag_var} ${${cxx_flag_var}} CACHE STRING
        "Flags used by the CXX compiler during ${cfg_upper} builds." FORCE
      )
    endif()
  endforeach()

elseif(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    OR ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
  if(NOT ANDROID AND NOT IOS
      AND cmr_USE_STATIC_RUNTIME AND NOT BUILD_SHARED_LIBS)
    if(MINGW)
      set(STATIC_LINKER_FLAGS "-static")
    elseif(NOT ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND APPLE))
      set(STATIC_LINKER_FLAGS "-static-libgcc -static-libstdc++")
    endif()
    set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} ${STATIC_LINKER_FLAGS}"
    )
    set(CMAKE_SHARED_LINKER_FLAGS
      "${CMAKE_SHARED_LINKER_FLAGS} ${STATIC_LINKER_FLAGS}"
    )
  endif()
endif()

# TODO: use link-time optimization (LTO).

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(
    # GNU C/C++
    -Werror -Wall -Wextra -Wpedantic -pedantic -pedantic-errors
    -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2 -Wdouble-promotion
    -Wnull-dereference -Wimplicit-fallthrough=5 -Wmissing-include-dirs
    -Wswitch-default -Wswitch-enum -Wunused-parameter -Wuninitialized
    -Wstrict-overflow=5 -Wstringop-truncation -Wsuggest-attribute=const
    -Wsuggest-override
    -Walloc-zero -Walloca -Wattribute-alias=2
    -Wduplicated-branches -Wduplicated-cond -Wtrampolines -Wfloat-equal
    -Wplacement-new=2 -Wshadow -Wunsafe-loop-optimizations -Wundef
    -Wunused-macros -Wcast-qual -Wcast-align=strict -Wcatch-value=3
    -Wconditionally-supported -Wconversion -Wsign-conversion
    -Wzero-as-null-pointer-constant -Wdate-time -Wuseless-cast -Wextra-semi
    -Wlogical-op -Wmissing-declarations -Wpacked
    -Wredundant-decls -Winline -Winvalid-pch
    -Wvector-operation-performance -Wvla -Wdisabled-optimization

    # GNU C++
    -Wctor-dtor-privacy -Wdeprecated-copy-dtor -Wnoexcept -Wnoexcept-type
    -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Wmultiple-inheritance
    -Wvirtual-inheritance -Wnon-virtual-dtor -Weffc++
    # NOTE: -Weffc++: be aware that the standard library headers
    # do not obey all of these guidelines.

    # -Wno-*
    -Wno-unknown-pragmas
  )
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"
     OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(
    -Werror -Wall -Wextra -Wpedantic -pedantic-errors
    -Warray-bounds-pointer-arithmetic -Wassign-enum -Wauto-import
    -Wbad-function-cast -Wbind-to-temporary-copy -Wbitfield-enum-conversion
    -Wc++11-narrowing -Wcast-align -Wcast-qual -Wclass-varargs -Wcomma
    -Wconditional-uninitialized -Wconsumed -Wconversion -Wdeprecated
    -Wdirect-ivar-access -Wdisabled-macro-expansion -Wdocumentation
    -Wdocumentation-pedantic -Wdouble-promotion
    -Wduplicate-decl-specifier -Wduplicate-enum -Wduplicate-method-arg
    -Wduplicate-method-match -Wdynamic-exception-spec
    -Weffc++ -Wnon-virtual-dtor
    -Wexit-time-destructors -Wexpansion-to-defined -Wexplicit-ownership-type
    -Wextra-semi -Wextra-semi-stmt
    -Wfloat-conversion -Wfloat-equal -Wfloat-overflow-conversion
    -Wfloat-zero-conversion -Wformat-non-iso -Wformat-pedantic -Wformat=2
    -Wglobal-constructors -Wheader-hygiene -Widiomatic-parentheses
    -Wimplicit-atomic-properties -Wimplicit-fallthrough
    -Wimplicit-float-conversion -Wimplicit-int-conversion -Wimplicit-retain-self
    -Wincomplete-module -Winconsistent-missing-destructor-override
    -Wloop-analysis -Wmain -Wmethod-signatures
    -Wmissing-noreturn -Wmissing-prototypes -Wmissing-variable-declarations
    -Wnewline-eof -Wnonportable-system-include-path
    -Wnullable-to-nonnull-conversion -Wold-style-cast -Wover-aligned
    -Woverriding-method-mismatch -Wpacked
    -Wpointer-arith -Wprofile-instr-missing -Wreceiver-forward-class
    -Wredundant-parens -Wreserved-id-macro -Wreserved-user-defined-literal
    -Wreturn-std-move-in-c++11 -Wselector -Wshadow-all -Wshift-sign-overflow
    -Wshorten-64-to-32 -Wsign-conversion -Wsigned-enum-bitfield
    -Wsometimes-uninitialized -Wstatic-in-inline
    -Wstrict-prototypes -Wstrict-selector-match -Wstring-conversion
    -Wsuper-class-method-mismatch -Wswitch-enum -Wtautological-compare
    -Wtautological-constant-in-range-compare -Wtautological-overlap-compare
    -Wthread-safety -Wthread-safety-beta -Wthread-safety-negative
    -Wthread-safety-verbose -Wundeclared-selector -Wundef
    -Wundefined-func-template -Wundefined-reinterpret-cast
    -Wunguarded-availability -Wunnamed-type-template-args
    -Wunneeded-internal-declaration -Wunneeded-member-function
    -Wunreachable-code-aggressive -Wunsupported-dll-base-class-template
    -Wunused-const-variable -Wunused-exception-parameter -Wunused-macros
    -Wunused-member-function -Wunused-template -Wused-but-marked-unused
    -Wvariadic-macros -Wvector-conversion -Wvla -Wweak-template-vtables
    -Wweak-vtables -Wzero-as-null-pointer-constant
    -Wgnu -Wmicrosoft

    # -Wno-*
    -Wno-unknown-pragmas -Wno-unknown-warning-option
  )
endif()

if(MSVC)
  add_compile_options(
    /WX /W4 /permissive-

    # Compiler warnings that are off by default
    -we4061 # (level 4)  enumerator 'identifier' in a switch of enum 'enumeration' is not explicitly handled by a case label
    -we4062 # (level 4)  enumerator 'identifier' in a switch of enum 'enumeration' is not handled
    -we4165 # (level 1)  'HRESULT' is being converted to 'bool'; are you sure this is what you want?
    -we4191 # (level 3)  'operator': unsafe conversion from 'type_of_expression' to 'type_required'
    -we4242 # (level 4)  'identifier': conversion from 'type1' to 'type2', possible loss of data
    -we4254 # (level 4)  'operator': conversion from 'type1' to 'type2', possible loss of data
    -we4255 # (level 4)  'function': no function prototype given: converting '()' to '(void)'
    -we4263 # (level 4)  'function': member function does not override any base class virtual member function
    -we4264 # (level 1)  'virtual_function': no override available for virtual member function from base 'class'; function is hidden
    -we4265 # (level 3)  'class': class has virtual functions, but destructor is not virtual
    -we4266 # (level 4)  'function': no override available for virtual member function from base 'type'; function is hidden
    -we4287 # (level 3)  'operator': unsigned/negative constant mismatch
    -we4289 # (level 4)  nonstandard extension used : 'var' : loop control variable declared in the for-loop is used outside the for-loop scope
    -we4296 # (level 4)  'operator': expression is always false
    -we4339 # (level 4)  'type' : use of undefined type detected in CLR meta-data - use of this type may lead to a runtime exception
    -we4342 # (level 1)  behavior change: 'function' called, but a member operator was called in previous versions
    -we4350 # (level 1)  behavior change: 'member1' called instead of 'member2'
    -we4355 #            'this' : used in base member initializer list
# System headers
#    -we4365 # (level 4)  'action': conversion from 'type_1' to 'type_2', signed/unsigned mismatch
    -we4370 # (level 3)  layout of class has changed from a previous version of the compiler due to better packing
    -we4371 # (level 3)  'classname': layout of class may have changed from a previous version of the compiler due to better packing of member 'member'
# GTest
#    -we4388 # (level 4)  signed/unsigned mismatch
    -we4412 # (level 2)  'function': function signature contains type 'type'; C++ objects are unsafe to pass between pure code and mixed or native
    -we4426 # (level 1)  optimization flags changed after including header, may be due to #pragma optimize() 14.1
    -we4435 # (level 4)  'class1' : Object layout under /vd2 will change due to virtual base 'class2'
    -we4437 # (level 4)  dynamic_cast from virtual base 'class1' to 'class2' could fail in some contexts
    -we4444 # (level 3)  top level '__unaligned' is not implemented in this context
    -we4464 # (level 4)  relative include path contains '..'
    -we4471 # (level 4)  a forward declaration of an unscoped enumeration must have an underlying type (int assumed) Perm
    -we4472 # (level 1)  'identifier' is a native enum: add an access specifier (private/public) to declare a managed enum
# System headers
#    -we4514 # (level 4)  'function': unreferenced inline function has been removed
    -we4536 # (level 4)  'type name': type-name exceeds meta-data limit of 'limit' characters
    -we4545 # (level 1)  expression before comma evaluates to a function which is missing an argument list
    -we4546 # (level 1)  function call before comma missing argument list
    -we4547 # (level 1)  'operator': operator before comma has no effect; expected operator with side-effect
# System headers
#    -we4548 # (level 1)  expression before comma has no effect; expected expression with side-effect
    -we4549 # (level 1)  'operator1': operator before comma has no effect; did you intend 'operator2'?
    -we4555 # (level 1)  expression has no effect; expected expression with side-effect
    -we4557 # (level 3)  '__assume' contains effect 'effect'
# System headers
#    -we4571 # (level 4)  informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
    -we4574 # (level 4)  'identifier' is defined to be '0': did you mean to use '#if identifier'?
    -we4577 # (level 1)  'noexcept' used with no exception handling mode specified; termination on exception is not guaranteed. Specify /EHsc
    -we4582 # (level 4)  'type': constructor is not implicitly called
    -we4583 # (level 4)  'type': destructor is not implicitly called
    -we4587 # (level 1)  'anonymous_structure': behavior change: constructor is no longer implicitly called
    -we4588 # (level 1)  'anonymous_structure': behavior change: destructor is no longer implicitly called
    -we4596 # (level 4)  'identifier': illegal qualified name in member declaration 14.3 Perm
    -we4598 # (level 1 and level 3)  '#include "header"': header number header-number in the precompiled header does not match current compilation at that position 14.3
    -we4599 # (level 3)  'option path': command-line argument number number does not match pre-compiled header 14.3
    -we4605 # (level 1)  '/Dmacro' specified on current command line, but was not specified when precompiled header was built
    -we4608 # (level 3)  'union_member' has already been initialized by another union member in the initializer list, 'union_member' Perm
    -we4619 # (level 3)  #pragma warning: there is no warning number 'number'
# System headers
#    -we4623 # (level 4)  'derived class': default constructor could not be generated because a base class default constructor is inaccessible
# System headers
#    -we4625 # (level 4)  'derived class': copy constructor could not be generated because a base class copy constructor is inaccessible
# System headers
#    -we4626 # (level 4)  'derived class': assignment operator could not be generated because a base class assignment operator is inaccessible
    -we4628 # (level 1)  digraphs not supported with -Ze. Character sequence 'digraph' not interpreted as alternate token for 'char'
    -we4640 # (level 3)  'instance': construction of local static object is not thread-safe
    -we4643 # (level 4)  Forward declaring 'identifier' in namespace std is not permitted by the C++ Standard. 15.8
    -we4647 # (level 3)  behavior change: __is_pod(type) has different value in previous versions
    -we4654 # (level 4)  Code placed before include of precompiled header line will be ignored. Add code to precompiled header. 14.1
# GTest
#    -we4668 # (level 4)  'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
    -we4682 # (level 4)  'symbol' : no directional parameter attribute specified, defaulting to [in]
    -we4686 # (level 3)  'user-defined type': possible change in behavior, change in UDT return calling convention
    -we4692 # (level 1)  'function': signature of non-private member contains assembly private native type 'native_type'
# System headers
#    -we4710 # (level 4)  'function': function not inlined
    -we4738 # (level 3)  storing 32-bit float result in memory, possible loss of performance
    -we4746 #            volatile access of 'expression' is subject to /volatile:<iso|ms> setting; consider using __iso_volatile_load/store intrinsic functions
    -we4749 # (level 4)  conditionally supported: offsetof applied to non-standard-layout type 'type'
    -we4767 # (level 4)  section name 'symbol' is longer than 8 characters and will be truncated by the linker
    -we4768 # (level 3)  __declspec attributes before linkage specification are ignored
# System headers
#    -we4774 # (level 4)  'string' : format string expected in argument number is not a string literal
    -we4777 # (level 4)  'function' : format string 'string' requires an argument of type 'type1', but variadic argument number has type 'type2'
    -we4786 # (level 3)  'symbol' : object name was truncated to 'number' characters in the debug information
    -we4800 # (level 4)  Implicit conversion from 'type' to bool. Possible information loss 16.0
# System headers
#    -we4820 # (level 4)  'bytes' bytes padding added after construct 'member_name'
    -we4822 # (level 1)  'member': local class member function does not have a body
    -we4826 # (level 2)  Conversion from 'type1' to 'type2' is sign-extended. This may cause unexpected runtime behavior.
    -we4837 # (level 4)  trigraph detected: '??character' replaced by 'character'
    -we4841 # (level 4)  non-standard extension used: compound member designator used in offsetof
    -we4842 # (level 4)  the result of 'offsetof' applied to a type using multiple inheritance is not guaranteed to be consistent between compiler releases
    -we4868 # (level 4)  'file(line_number)' compiler may not enforce left-to-right evaluation order in braced initialization list
    -we4905 # (level 1)  wide string literal cast to 'LPSTR'
    -we4906 # (level 1)  string literal cast to 'LPWSTR'
    -we4917 # (level 1)  'declarator': a GUID can only be associated with a class, interface, or namespace
    -we4928 # (level 1)  illegal copy-initialization; more than one user-defined conversion has been implicitly applied
    -we4931 # (level 4)  we are assuming the type library was built for number-bit pointers
    -we4946 # (level 1)  reinterpret_cast used between related classes: 'class1' and 'class2'
    -we4962 #            'function': profile-guided optimizations disabled because optimizations caused profile data to become inconsistent
    -we4986 # (level 4)  'symbol': exception specification does not match previous declaration
    -we4987 # (level 4)  nonstandard extension used: 'throw (...)'
    -we4988 # (level 4)  'symbol': variable declared outside class/function scope
    -we5022 #            'type': multiple move constructors specified
    -we5023 #            'type': multiple move assignment operators specified
    -we5024 # (level 4)  'type': move constructor was implicitly defined as deleted
    -we5025 # (level 4)  'type': move assignment operator was implicitly defined as deleted
# System headers
#    -we5026 # (level 1 and level 4)  'type': move constructor was implicitly defined as deleted
# System headers
#    -we5027 # (level 1 and level 4)  'type': move assignment operator was implicitly defined as deleted
    -we5029 # (level 4)  nonstandard extension used: alignment attributes in C++ apply to variables, data members and tag types only
    -we5031 # (level 4)  #pragma warning(pop): likely mismatch, popping warning state pushed in different file 14.1
    -we5032 # (level 4)  detected #pragma warning(push) with no corresponding #pragma warning(pop) 14.1
    -we5034 #            use of intrinsic 'intrinsic' causes function function-name to be compiled as guest code 15.3
    -we5035 #            use of feature 'feature' causes function function-name to be compiled as guest code 15.3
    -we5036 # (level 1)  varargs function pointer conversion when compiling with /hybrid:x86arm64 'type1' to 'type2' 15.3
    -we5038 # (level 4)  data member 'member1' will be initialized after data member 'member2' 15.3
    -we5039 # (level 4)  'function': pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavior may occur if this function throws an exception. 15.5
    -we5042 # (level 3)  'function': function declarations at block scope cannot be specified 'inline' in standard C++; remove 'inline' specifier 15.5
    -we5045 #            Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified 15.7

    # Suppress warnings.
    -wd4068 # (level 1) unknown pragma
  )
endif()

option(cmr_BUILD_MULTIPROC "cmr_BUILD_MULTIPROC" ON)

if(cmr_BUILD_MULTIPROC)
  # Enable /MP flag for Visual Studio 2008 and greater.
  if(MSVC AND MSVC_VERSION GREATER 1400)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP${cmr_BUILD_MULTIPROC_CNT}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP${cmr_BUILD_MULTIPROC_CNT}")
  endif()
endif()

#if(ANDROID)
# Using an explicit -fuse-ld=lld instead of ANDROID_LD=lld will produce output that cannot be debugged with Android Studio.
#  # https://github.com/android/ndk/issues/1183
#  # https://developer.android.com/ndk/downloads/revision_history
#  set(CMAKE_EXE_LINKER_FLAGS
#    "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld"
#  )
#  set(CMAKE_SHARED_LINKER_FLAGS
#    "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=lld"
#  )
#endif()
