#include <crosscapture/crosscapture.h>
#include "crosscapture/platform/platform.h"
#include "crosscapture/common/capture_device/device.h"
#include <iostream>

// TODO: consider caching window and display objects and then just verify.

namespace cross_capture::platform {
    std::vector<MonitorData> enumerate_monitors() {
        CGDirectDisplayID result[MAX_DISPLAYS];
        uint32_t num_monitors = 0;

        CGError err = CGGetActiveDisplayList(MAX_DISPLAYS, result, &num_monitors);
        if (err != kCGErrorSuccess) {
            throw std::runtime_error("CGGetActiveDisplayList failed");
        }

        for (auto i = 0u; i < num_monitors; ++i) {
            std::cout << "Monitor " << i << ": " << result[i] << std::endl;
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
            
            monitors.push_back(MonitorData {
                monitor_id,
                L"",
                monitor_width,
                monitor_height
            });
        }

        return monitors;
    }

    bool is_window_handle_valid(const window_handle_t window_handle) {
		return true;
	}
}