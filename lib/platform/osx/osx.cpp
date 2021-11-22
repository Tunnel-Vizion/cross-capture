#include <crosscapture/crosscapture.h>
#include "crosscapture/platform/platform.h"
#include "crosscapture/common/capture_device/device.h"
#include <CoreGraphics/CoreGraphics.h>

// TODO: consider caching window and display objects and then just verify.

namespace cross_capture::platform {
    std::vector<WindowData> enumerate_windows(WindowEnumerateFilter filter) {
        // get list of all windows
        auto result = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
        if (result == nullptr) {
            throw std::runtime_error("CGWindowListCopyWindowInfo failed");
        }
        
        const auto num_windows = CFArrayGetCount(result);
        // move values into array
        CFDictionaryRef window_info[num_windows];
        CFArrayGetValues(result, CFRangeMake(0, num_windows), reinterpret_cast<const void**>(&window_info));

        // iterate over windows
        std::vector<WindowData> windows;
        for (auto i = 0; i < num_windows; ++i) {
            auto window_dict = window_info[i];
            auto window_id = reinterpret_cast<CFNumberRef>(CFDictionaryGetValue(window_dict, kCGWindowNumber));
            auto window_title = reinterpret_cast<CFStringRef>(CFDictionaryGetValue(window_dict, kCGWindowName));

            // cast window_title to String
            platform::String title;
            if (window_title != nullptr) {
                title = CFStringGetCStringPtr(window_title, kCFStringEncodingUTF8);

                if (filter.include_empty_titles) {
                    continue;
                }
            }

            unsigned int window_handle;
            CFNumberGetValue(window_id, kCFNumberIntType, &window_handle);

            windows.emplace_back(WindowData {
                window_handle,
                title,
            });
        }

        return windows;
    }

    std::vector<MonitorData> enumerate_monitors() {
        CGDirectDisplayID result[MAX_DISPLAYS];
        uint32_t num_monitors = 0;

        CGError err = CGGetActiveDisplayList(MAX_DISPLAYS, result, &num_monitors);
        if (err != kCGErrorSuccess) {
            throw std::runtime_error("CGGetActiveDisplayList failed");
        }

        std::vector<MonitorData> monitors;
        for (int i = 0; i < num_monitors; i++) {
            auto monitor_id = result[i];

            // get height and width of monitor
            auto monitor_rect = CGDisplayBounds(monitor_id);
            auto monitor_width = CGRectGetWidth(monitor_rect);
            auto monitor_height = CGRectGetHeight(monitor_rect);

            // osx doesn't have a way to get the monitor name, so we'll just use the index
            auto monitor_name = L"Monitor " + std::to_wstring(i);
            
            monitors.emplace_back(MonitorData {
                monitor_id,
                String(),
                monitor_width,
                monitor_height
            });
        }

        return monitors;
    }

    platform::String get_window_title(window_handle_t window_handle) {
        // TODO: implement
        return platform::String();
    }

    bool is_window_handle_valid(const window_handle_t window_handle) {
		return true;
	}
}