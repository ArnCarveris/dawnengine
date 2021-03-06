// --------------------------------------------------------------
// Configures prerequisits for this library
// --------------------------------------------------------------

#ifndef __ONTOLOGY_CONFIG_HPP__
#   define __ONTOLOGY_CONFIG_HPP__

    // --------------------------------------------------------------
    // build settings
    // --------------------------------------------------------------

/* #undef ONTOLOGY_WITH_THREAD */
/* #undef ONTOLOGY_WITH_EXCEPTIONS */
/* #undef ONTOLOGY_WITH_ASSERT */
/* #undef ONTOLOGY_SHARED */
    #define ONTOLOGY_STATIC

    // --------------------------------------------------------------
    // Identify the operating system
    // --------------------------------------------------------------

#   if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#       define ONTOLOGY_PLATFORM_WINDOWS
#   elif defined(linux) || defined(__linux)
#       define ONTOLOGY_PLATFORM_LINUX
#   elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)
#       define ONTOLOGY_PLATFORM_MAC
#   else
#       error This operating system is not supported
#   endif

    // --------------------------------------------------------------
    // define visibility macros
    // --------------------------------------------------------------

    // define platform dependent and build dependent visibility macro helpers
#   if defined(ONTOLOGY_SHARED)
#       if defined(ONTOLOGY_PLATFORM_WINDOWS)
#           if defined(__GNUC__)
                // cygwin visbibility
#               define ONTOLOGY_HELPER_API_EXPORT __attribute__ ((dllexport))
#               define ONTOLOGY_HELPER_API_IMPORT __attribute__ ((dllimport)
#           else
                // msvc visibility
#               define ONTOLOGY_HELPER_API_EXPORT __declspec(dllexport)
#               define ONTOLOGY_HELPER_API_IMPORT __declspec(dllimport)
#           endif
#           define ONTOLOGY_HELPER_API_LOCAL
#       else
#           if __GNUC__ >= 4
                // gcc 4+ visibility
#               define ONTOLOGY_HELPER_API_EXPORT __attribute__ ((visibility ("default")))
#               define ONTOLOGY_HELPER_API_IMPORT __attribute__ ((visibility ("default")))
#               define ONTOLOGY_HELPER_API_LOCAL  __attribute__ ((visibility ("hidden")))
#           else
                // gcc lower than 4 doesn't have any explicit visibility, everything is exported
#               define ONTOLOGY_HELPER_API_EXPORT
#               define ONTOLOGY_HELPER_API_IMPORT
#               define ONTOLOGY_HELPER_API_LOCAL
#           endif
#       endif
#   elif defined(ONTOLOGY_STATIC)
        // static build
#       define ONTOLOGY_HELPER_API_EXPORT
#       define ONTOLOGY_HELPER_API_IMPORT
#       define ONTOLOGY_HELPER_API_LOCAL
#   else
#       error Please define ONTOLOGY_SHARED or ONTOLOGY_STATIC
#   endif

    // define public API macro, depending on whether the library is being
    // built or being linked against.
#   if defined(ONTOLOGY_BUILDING_LIBRARY) // defined by CMake when library is being compiled
#       define ONTOLOGY_PUBLIC_API ONTOLOGY_HELPER_API_EXPORT
#   else
#       define ONTOLOGY_PUBLIC_API ONTOLOGY_HELPER_API_IMPORT
#   endif

    // define local visibility macro. If we're testing, everything
    // is visible
#   if defined(TESTING)
#       define ONTOLOGY_LOCAL_API ONTOLOGY_PUBLIC_API
#   else
#       define ONTOLOGY_LOCAL_API ONTOLOGY_HELPER_API_LOCAL
#   endif

    // define class member visibility macros. If we're testing, everything
    // is public
#   if defined(TESTING)
#       define PUBLIC public
#       define PROTECTED public
#       define PRIVATE public
#   else
#       define PUBLIC public
#       define PROTECTED protected
#       define PRIVATE private
#   endif

namespace Ontology {

// --------------------------------------------------------------
// define fixed-type sizes
// --------------------------------------------------------------

// All "common" platforms use the same size for char, short and int
// (basically there are 3 types for 3 sizes, so no other match is possible),
// we can use them without doing any kind of check

// 8 bits integer types
typedef signed char int8;
typedef unsigned char uint8;

// 16 bits integer types
typedef signed short int16;
typedef unsigned short uint16;

// 32 bits integer types
typedef signed int int32;
typedef unsigned int uint32;

// 64 bits integer types
#   if defined(_MSC_VER)
typedef signed   __int64 Int64;
typedef unsigned __int64 Uint64;
#   else
typedef signed long long int64;
typedef unsigned long long uint64;
#   endif

} // namespace Ontology

#endif // __ONTOLOGY_CONFIG_HPP__

