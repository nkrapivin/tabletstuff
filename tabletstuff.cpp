#include "tabletstuff.hpp"

_Tseventlist EventList;
_Tseventmap EventCurrent;
_Tsstring EmptyString;
size_t EntryIndex = 0;
double NotADouble = std::nan("");

_Tseventmap _Tseventbegin() {
    return _Tseventmap();
}

void _Tseventaddr(_Tseventmap& Self, const _Tsstring& name, const double& val) {
    Self.push_back(std::make_pair(name, std::make_pair(val, EmptyString)));
}

void _Tseventadds(_Tseventmap& Self, const _Tsstring& name, const _Tsstring& val) {
    Self.push_back(std::make_pair(name, std::make_pair(NotADouble, val)));
}

void _Tseventend(const _Tseventmap& _map) {
    EventList.push(_map);
}

#ifdef _WIN32

HMODULE hThisDLL = NULL;
HWND hGameWindow = NULL;
WNDPROC fnProcOrig = NULL;

dlle BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpvReserved) {
    switch (dwReasonForCall) {
        case DLL_PROCESS_ATTACH: {
            hThisDLL = hModule;
            break;
        }

        default: {
            break;
        }
    }

    return TRUE;
}

dlle void _Tsprocesspointergeneric(_Tseventmap& ev, const POINTER_INFO& ptrInfo) {
    std::stringstream hwndptr, sourcedeviceptr;
    hwndptr << static_cast<void*>(ptrInfo.hwndTarget);
    sourcedeviceptr << static_cast<void*>(ptrInfo.sourceDevice);
    _Tseventaddr(ev, "pointer_info", 1);
    _Tseventaddr(ev, "pointer_info_type", ptrInfo.pointerType);
    _Tseventaddr(ev, "pointer_info_id", ptrInfo.pointerId);
    _Tseventaddr(ev, "pointer_info_frame_id", ptrInfo.frameId);
    _Tseventaddr(ev, "pointer_info_flags", ptrInfo.pointerFlags);
    _Tseventadds(ev, "pointer_info_source_device", sourcedeviceptr.str().c_str());
    _Tseventadds(ev, "pointer_info_hwnd_target", hwndptr.str().c_str());
    _Tseventaddr(ev, "pointer_info_pixel_location_x", ptrInfo.ptPixelLocation.x);
    _Tseventaddr(ev, "pointer_info_pixel_location_y", ptrInfo.ptPixelLocation.y);
    _Tseventaddr(ev, "pointer_info_himetric_location_x", ptrInfo.ptHimetricLocation.x);
    _Tseventaddr(ev, "pointer_info_himetric_location_y", ptrInfo.ptHimetricLocation.y);
    _Tseventaddr(ev, "pointer_info_pixel_location_raw_x", ptrInfo.ptPixelLocationRaw.x);
    _Tseventaddr(ev, "pointer_info_pixel_location_raw_y", ptrInfo.ptPixelLocationRaw.y);
    _Tseventaddr(ev, "pointer_info_himetric_location_raw_x", ptrInfo.ptHimetricLocationRaw.x);
    _Tseventaddr(ev, "pointer_info_himetric_location_raw_y", ptrInfo.ptHimetricLocationRaw.y);
    _Tseventaddr(ev, "pointer_info_time", ptrInfo.dwTime);
    _Tseventaddr(ev, "pointer_info_history_count", ptrInfo.historyCount);
    _Tseventaddr(ev, "pointer_info_input_data", ptrInfo.InputData);
    _Tseventaddr(ev, "pointer_info_key_states", ptrInfo.dwKeyStates);
    // I know this is bad, nor do I care.
    _Tseventaddr(ev, "pointer_info_performance_count_low", (double)(ptrInfo.PerformanceCount & 0xFFFFFFFF));
    _Tseventaddr(ev, "pointer_info_performance_count_high", (double)((ptrInfo.PerformanceCount >> 0x20) & 0xFFFFFFFF));
    _Tseventaddr(ev, "pointer_info_performance_count", (double)ptrInfo.PerformanceCount);
    _Tseventaddr(ev, "pointer_info_button_change_type", ptrInfo.ButtonChangeType);
}

dlle void _Tsprocesstouch(_Tseventmap& ev, const POINTER_TOUCH_INFO& touchInfo) {
    _Tsprocesspointergeneric(ev, touchInfo.pointerInfo);
    _Tseventaddr(ev, "pointer_info_touch", 1);
    _Tseventaddr(ev, "pointer_info_touch_flags", touchInfo.touchFlags);
    _Tseventaddr(ev, "pointer_info_touch_mask", touchInfo.touchMask);
    _Tseventaddr(ev, "pointer_info_touch_contact_left", touchInfo.rcContact.left);
    _Tseventaddr(ev, "pointer_info_touch_contact_top", touchInfo.rcContact.top);
    _Tseventaddr(ev, "pointer_info_touch_contact_right", touchInfo.rcContact.right);
    _Tseventaddr(ev, "pointer_info_touch_contact_bottom", touchInfo.rcContact.bottom);
    _Tseventaddr(ev, "pointer_info_touch_contact_raw_left", touchInfo.rcContactRaw.left);
    _Tseventaddr(ev, "pointer_info_touch_contact_raw_top", touchInfo.rcContactRaw.top);
    _Tseventaddr(ev, "pointer_info_touch_contact_raw_right", touchInfo.rcContactRaw.right);
    _Tseventaddr(ev, "pointer_info_touch_contact_raw_bottom", touchInfo.rcContactRaw.bottom);
    _Tseventaddr(ev, "pointer_info_touch_orientation", touchInfo.orientation);
    _Tseventaddr(ev, "pointer_info_touch_pressure", touchInfo.pressure);
}

dlle void _Tsprocesspen(_Tseventmap& ev, const POINTER_PEN_INFO& penInfo) {
    _Tsprocesspointergeneric(ev, penInfo.pointerInfo);
    _Tseventaddr(ev, "pointer_info_pen", 1);
    _Tseventaddr(ev, "pointer_info_pen_flags", penInfo.penFlags);
    _Tseventaddr(ev, "pointer_info_pen_mask", penInfo.penMask);
    _Tseventaddr(ev, "pointer_info_pen_pressure", penInfo.pressure);
    _Tseventaddr(ev, "pointer_info_pen_rotation", penInfo.rotation);
    _Tseventaddr(ev, "pointer_info_pen_tilt_x", penInfo.tiltX);
    _Tseventaddr(ev, "pointer_info_pen_tilt_y", penInfo.tiltY);
}

dlle void _Tsaddextended(UINT32 uPointerId) {
    UINT32 uEntriesCount = 0; SSIZE_T ind = 0;
    POINTER_INFO ptrInfo = { 0 };
    
    if (FALSE == GetPointerInfo(uPointerId, &ptrInfo)) {
        return;
    }

    uEntriesCount = ptrInfo.historyCount;

    if (ptrInfo.pointerType == PT_PEN) {
        std::vector<POINTER_PEN_INFO> penHistory;
        penHistory.resize(uEntriesCount);

        if (TRUE == GetPointerPenInfoHistory(uPointerId, &uEntriesCount, penHistory.data()) && uEntriesCount > 0) {
            // need to loop in reverse because the most recent entry is at [0]
            // and we have a queue
            for (ind = static_cast<SSIZE_T>(uEntriesCount) - 1; ind >= 0; --ind) {
                auto ev = _Tseventbegin();
                _Tsprocesspen(ev, penHistory.at(static_cast<size_t>(ind)));
                _Tseventend(ev);
            }
        }
    }
    else if (ptrInfo.pointerType == PT_TOUCH) {
        std::vector<POINTER_TOUCH_INFO> touchHistory;
        touchHistory.resize(uEntriesCount);

        if (TRUE == GetPointerTouchInfoHistory(uPointerId, &uEntriesCount, touchHistory.data()) && uEntriesCount > 0) {
            // need to loop in reverse because the most recent entry is at [0]
            // and we have a queue
            for (ind = static_cast<SSIZE_T>(uEntriesCount) - 1; ind >= 0; --ind) {
                auto ev = _Tseventbegin();
                _Tsprocesstouch(ev, touchHistory.at(static_cast<size_t>(ind)));
                _Tseventend(ev);
            }
        }
    }
    else {
        std::vector<POINTER_INFO> ptrHistory;
        ptrHistory.resize(uEntriesCount);

        if (TRUE == GetPointerInfoHistory(uPointerId, &uEntriesCount, ptrHistory.data()) && uEntriesCount > 0) {
            // need to loop in reverse because the most recent entry is at [0]
            // and we have a queue
            for (ind = static_cast<SSIZE_T>(uEntriesCount) - 1; ind >= 0; --ind) {
                auto ev = _Tseventbegin();
                _Tsprocesspointergeneric(ev, ptrHistory.at(static_cast<size_t>(ind)));
                _Tseventend(ev);
            }
        }
    }
}

dlle LRESULT CALLBACK _TsWNDPROC(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (hWnd == hGameWindow) {
        switch (uMsg) {
            case WM_POINTERENTER:
            case WM_POINTERLEAVE:
            case WM_POINTERDOWN:
            case WM_POINTERUP:
            case WM_POINTERUPDATE: {
                _Tsaddextended(GET_POINTERID_WPARAM(wParam));
                break;
            }
        }
    }

    // always call the original handler after
    return fnProcOrig(hWnd, uMsg, wParam, lParam);
}

#else

//TODO: Non-Windows implementation?
//highly unlikely though...

#endif

#define _Tscase(_EnumValue) case (_EnumValue): return (#_EnumValue);
dllx const char* dllc tabletstuff_error_to_string(double tabletstuff_error_code) {
    switch ((tabletstuff_error)tabletstuff_error_code) {
        _Tscase(tabletstuff_error_none);
        _Tscase(tabletstuff_error_invalid);
        _Tscase(tabletstuff_error_window_handle_is_null);
        _Tscase(tabletstuff_error_wndproc_is_null);
        _Tscase(tabletstuff_error_already_initialized);
        _Tscase(tabletstuff_error_hook_failed);
        _Tscase(tabletstuff_error_out_of_memory);
        default: return "<unknown>";
    }
}
#undef _Tscase

dllx double dllc tabletstuff_is_present() {
    return 1;
}

dllx double dllc tabletstuff_init_new(int* window_handle_result) {
    if (fnProcOrig) {
        return tabletstuff_error_already_initialized;
    }

    hGameWindow = (HWND)window_handle_result;
    if (!hGameWindow) {
        // try to autodetect the game's window if none is supplied.
        hGameWindow = GetActiveWindow();
    }

    fnProcOrig = (WNDPROC)GetWindowLongPtrW(hGameWindow, GWLP_WNDPROC);
    if (!fnProcOrig) {
        return tabletstuff_error_wndproc_is_null;
    }

    if (0 == SetWindowLongPtrW(hGameWindow, GWLP_WNDPROC, (LONG_PTR)&_TsWNDPROC)) {
        return tabletstuff_error_hook_failed;
    }

    return tabletstuff_error_none;
}

dllx double dllc tabletstuff_init_legacy(double window_handle_result_as_real) {
    // for old GM versions where window_handle() returned a casted double
    return tabletstuff_init_new((int*)(uintptr_t)window_handle_result_as_real);
}

dllx double dllc tabletstuff_is_in_focus() {
    return GetForegroundWindow() == hGameWindow;
}

dllx double dllc tabletstuff_event_begin() {
    EntryIndex = 0;

    if (EventList.empty()) {
        return 0;
    }

    EventCurrent = EventList.front();
    EventList.pop();
    return 1;
}

dllx double dllc tabletstuff_event_length() {
    return (double)EventCurrent.size();
}

dllx double dllc tabletstuff_event_get_type() {
    // way too lazy to handle this properly
    return std::isnan(EventCurrent[EntryIndex].second.first);
}

dllx const char* dllc tabletstuff_event_get_name() {
    // the string is alive till a call to tabletstuff_event_next
    return EventCurrent[EntryIndex].first.c_str();
}

dllx double dllc tabletstuff_event_get_number() {
    // increment now
    return EventCurrent[EntryIndex++].second.first;
}

dllx const char* dllc tabletstuff_event_get_string() {
    // increment now
    return EventCurrent[EntryIndex++].second.second.c_str();
}

dllx double dllc tabletstuff_event_next() {
    // prepare for the next event, get rid of the current map
    EventCurrent = _Tseventmap();
    return 1;
}
