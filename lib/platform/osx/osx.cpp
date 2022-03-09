#include <crosscapture/crosscapture.h>
#include "crosscapture/platform/platform.h"
#include "crosscapture/common/capture_device/device.h"
#include <CoreGraphics/CoreGraphics.h>

// TODO: consider caching window and display objects and then just verify.

namespace cross_capture::platform {
    std::vector<WindowData> enumerate_windows(WindowEnumerateFilter filter) {
        // get list of all windows
        auto window_info_list = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
        if (window_info_list == nullptr) {
            throw std::runtime_error("CGWindowListCopyWindowInfo failed");
        }
        
        const auto num_windows = CFArrayGetCount(window_info_list);
        // move values into array
        CFDictionaryRef window_info[num_windows];
        CFArrayGetValues(window_info_list, CFRangeMake(0, num_windows), reinterpret_cast<const void**>(&window_info));

        // iterate over windows
        std::vector<WindowData> windows;
        for (auto i = 0; i < num_windows; ++i) {
            auto window_dict = window_info[i];
            auto window_id = reinterpret_cast<CFNumberRef>(CFDictionaryGetValue(window_dict, kCGWindowNumber));
            auto window_title = reinterpret_cast<CFStringRef>(CFDictionaryGetValue(window_dict, kCGWindowName));

            // cast window_title to String
            std::string title;
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
            auto monitor_name = "Monitor " + std::to_string(i);
            
            monitors.emplace_back(MonitorData {
                monitor_id,
                monitor_name,
                monitor_width,
                monitor_height
            });
        }

        return monitors;
    }

    std::string get_window_title(window_handle_t window_handle) {
        // TODO: implement
        return std::string();
    }

    bool is_window_handle_valid(const window_handle_t window_handle) {
		return true;
	}

    bool debug_save_bmp(const std::string& file_name, capture_device::CapturedFrame frame) {
        // Create bitmap context
        auto bitmap_context = CGBitmapContextCreate(
            frame.data.data(),
            frame.width,
            frame.height,
            8,
            frame.width * 4,
            CGColorSpaceCreateDeviceRGB(),
            kCGImageAlphaPremultipliedLast
        );

        // Create image from bitmap context
        auto image = CGBitmapContextCreateImage(bitmap_context);

        CFStringRef file_name_ref = CFStringCreateWithCString(kCFAllocatorDefault, (file_name + ".png").c_str(), kCFStringEncodingUTF8);

        // Create image destination
        auto image_destination = CGImageDestinationCreateWithURL(
            CFURLCreateWithFileSystemPath(kCFAllocatorDefault, file_name_ref, kCFURLPOSIXPathStyle, false),
            kUTTypePNG,
            1,
            nullptr
        );

        // Add image to image destination
        CGImageDestinationAddImage(image_destination, image, nullptr);

        // Write image to file
        CGImageDestinationFinalize(image_destination);

        // Clean up
        CFRelease(image_destination);
        CFRelease(image);
        CFRelease(bitmap_context);
        CFRelease(file_name_ref);

        return true;
    }
}