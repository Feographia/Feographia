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
    -Wno-unknown-warning-option
  )
endif()

if(MSVC)
  add_compile_options(/WX /Wall)
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
