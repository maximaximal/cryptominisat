/******************************************
Copyright (C) 2009-2020 Authors of CryptoMiniSat, see AUTHORS file

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
***********************************************/

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include <cstdint>
#include <cstdlib>
#include <stdio.h>
#include <limits>

using std::numeric_limits;

// #define VERBOSE_DEBUG
// #define VERBOSE_DEBUG_FULLPROP
// #define DEBUG_DEPTH
// #define SLOW_DEBUG

#if defined(_MSC_VER)
#define release_assert(a) \
    do { \
    __pragma(warning(push)) \
    __pragma(warning(disable:4127)) \
        if (!(a)) {\
    __pragma(warning(pop)) \
            fprintf(stderr, "*** ASSERTION FAILURE in %s() [%s:%d]: %s\n", \
            __FUNCTION__, __FILE__, __LINE__, #a); \
            abort(); \
        } \
    } while (0)
#else
#define release_assert(a) \
    do { \
        if (!(a)) {\
            fprintf(stderr, "*** ASSERTION FAILURE in %s() [%s:%d]: %s\n", \
            __FUNCTION__, __FILE__, __LINE__, #a); \
            abort(); \
        } \
    } while (0)
#endif

#if !defined(__GNUC__) && !defined(__clang__)
#define cmsat_prefetch(x) (void)(x)
#else
#define cmsat_prefetch(x) __builtin_prefetch(x)
#endif

//We shift stuff around in Watched, so not all of 32 bits are useable.
//for STATS we have 64b values in the Clauses, so they must be aligned to 64

#if defined(STATS_NEEDED)
#define LARGE_OFFSETS
#define STATS_DO(x) do {x;} while (0)
#define INC_ID(cl) \
    do { \
        auto prev_id = (cl).stats.ID; \
        (cl).stats.ID = ++solver->clauseID; \
        if (solver->sqlStats && (cl).stats.is_tracked) solver->sqlStats->update_id(prev_id, (cl).stats.ID); \
    } while (0)
#else
#define STATS_DO(x) do {} while (0)
#define INC_ID(cl) \
    do { \
        (cl).stats.ID = ++solver->clauseID; \
    } while (0)
#endif

#if defined(LARGE_OFFSETS)
#define BASE_DATA_TYPE uint64_t
#define EFFECTIVELY_USEABLE_BITS 62
#else
#define BASE_DATA_TYPE uint32_t
#define EFFECTIVELY_USEABLE_BITS 30
#endif

#define MAX_XOR_RECOVER_SIZE 8

#if defined _WIN32
    #define DLL_PUBLIC __declspec(dllexport)
#else
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#endif

///////////////////
// Verbose Debug
///////////////////

//#define FRAT_DEBUG
//#define VERBOSE_DEBUG

// verbose print
#ifdef VERBOSE_DEBUG
#define VERBOSE_PRINT(x) \
    do { std::cout << x << std::endl; } while (0)
#define VERBOSE_DEBUG_DO(x) do { x; } while (0)
#else
#define VERBOSE_PRINT(x) do { } while (0)
#define VERBOSE_DEBUG_DO(x) do { } while (0)
#endif
/////

// slow debug
#ifdef SLOW_DEBUG
#define SLOW_DEBUG_DO(x) \
    do { x; } while (0)
#else
#define SLOW_DEBUG_DO(x) do { } while (0)
#endif
/////

// verb_print
#define verb_print(a, x) \
    do { if (solver->conf.verbosity >= a) {std::cout << "c " << x << std::endl;} } while (0)
/////

// debug watched
#ifdef DEBUG_WATCHED
#define DEBUG_WATCHED_DO(x) \
    do { x; } while (0)
#else
#define DEBUG_WATCHED_DO(x) do { } while (0)
#endif


// tbuddy
#ifdef USE_TBUDDY
#define TBUDDY_DO(x) \
    do { x; } while (0)
#else
#define TBUDDY_DO(x)  do { } while (0)
#endif

#ifdef VERBOSE_DEBUG
#define FAST_DEBUG
#define DEBUG_ATTACH_FULL
#define VERBOSE_DEBUG_XOR
#define VERBOSE_DEBUG_RECONSTRUCT
#endif

//Thanks to Axel Kemper for the definitions below
#ifdef _MSC_VER
#pragma warning(disable : 4244)  //  C4244 : 'Argument': Konvertierung von 'const uint64_t' in 'double', möglicher Datenverlust
#pragma warning(disable : 4267)  //  C4267 : 'return': Konvertierung von 'size_t' nach 'uint32_t', Datenverlust möglich
#pragma warning(disable : 4302)  //  C4302 : truncation
#pragma warning(disable : 4311)  //  C4311 : pointer truncation
#pragma warning(disable : 4800)  //  C4800 : 'const uint32_t' : Variable wird auf booleschen Wert('True' oder 'False') gesetzt(Auswirkungen auf Leistungsverhalten möglich)
#pragma warning(disable : 4805)  //  C4805 : '==' : unsichere Kombination von Typ 'unsigned short' mit Typ 'bool' in einer Operation
#endif


#ifdef __GNUC__
    #define likely(x) __builtin_expect((x), 1)
    #define unlikely(x) __builtin_expect((x), 0)
#else
    #define likely(x) x
    #define unlikely(x) x
#endif

///////////////////
// Silent Debug
///////////////////

#ifndef NDEBUG
//#define FAST_DEBUG
#endif

#ifdef SLOW_DEBUG
#define FAST_DEBUG
#define DEBUG_PROPAGATEFROM
#define ENQUEUE_DEBUG
#define DEBUG_ATTACH_MORE
#define DEBUG_IMPLICIT_PAIRS_TRIPLETS
#define DEBUG_IMPLICIT_STATS
#define DEBUG_GAUSS
#define XOR_DEBUG
#endif

#ifdef FAST_DEBUG
#define DEBUG_VARELIM
#define DEBUG_WATCHED
#define DEBUG_ATTACH
#define DEBUG_REPLACER
#define DEBUG_MARKED_CLAUSE
#define CHECK_N_OCCUR
#endif

//#define DEBUG_ATTACH_FULL

#endif //__CONSTANTS_H__
