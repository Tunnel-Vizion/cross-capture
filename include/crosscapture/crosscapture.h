#pragma once

#ifdef WIN32
#include "crosscapture/platform/win/dxgi_device.h"
#include "crosscapture/platform/win/gdi_device.h"
#endif

#include "crosscapture/platform/platform.h"
#include "crosscapture/common/capture_device/device.h"
#include "crosscapture/common/view/monitor_view.h"
#include "crosscapture/common/view/window_view.h"

// Constants
namespace cross_capture {
    constexpr int MAX_DISPLAYS = 16;
}