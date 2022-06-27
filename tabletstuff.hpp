#pragma once

#ifdef __cplusplus
#define dlle extern "C"
#else
#define dlle /* nothing, because we're already compiling as pure C */
#endif

#include <vector>
#include <queue>
#include <map>
#include <string>
#include <utility>
#include <cmath>
#include <sstream>

using _Tsstring = std::string;
using _Tseventmap = std::vector<std::pair<_Tsstring, std::pair<double, _Tsstring>>>;
using _Tseventlist = std::queue<_Tseventmap>;

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOMCX
#define NOSOUND
#include <Windows.h>
#include <windowsx.h>

#ifdef TABLETSTUFF_EXPORTS
#define dlls __declspec(dllexport)
#else
#define dlls __declspec(dllimport)
#endif

#define dllc __cdecl

#else

#define dlls __attribute__((visibility("default")))
#define dllc /* nothing */

#endif

#define dllx dlle dlls




//////PUBLIC TABLETSTUFF API:

///MACROS:
typedef enum _Tserror {
    tabletstuff_error_none = 1,
    tabletstuff_error_invalid = 0,
    tabletstuff_error_window_handle_is_null = -1,
    tabletstuff_error_wndproc_is_null = -2,
    tabletstuff_error_already_initialized = -3,
    tabletstuff_error_hook_failed = -4,
    tabletstuff_error_out_of_memory = -5
} tabletstuff_error;

///FUNCTIONS:
dllx const char* dllc tabletstuff_error_to_string(double tabletstuff_error_code);
dllx double      dllc tabletstuff_is_present();
dllx double      dllc tabletstuff_init_new(int* window_handle_result);
dllx double      dllc tabletstuff_init_legacy(double window_handle_result_as_real);
dllx double      dllc tabletstuff_is_in_focus();
dllx double      dllc tabletstuff_event_begin();
dllx double      dllc tabletstuff_event_length();
dllx double      dllc tabletstuff_event_get_type();
dllx const char* dllc tabletstuff_event_get_name();
dllx double      dllc tabletstuff_event_get_number();
dllx const char* dllc tabletstuff_event_get_string();
dllx double      dllc tabletstuff_event_next();

///END.



