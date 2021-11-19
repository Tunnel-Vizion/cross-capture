#include "crosscapture/platform/osx/quartz_device.h"
#include "crosscapture/common/view/monitor_view.h"
#include "crosscapture/common/view/window_view.h"

#include <ApplicationServices/ApplicationServices.h>
#include <CoreGraphics/CoreGraphics.h>
#include <CoreGraphics/CGImage.h>

namespace cross_capture::capture_device {
    CapturedFrame QuartzDevice::do_monitor_capture(const MonitorView *view) {
        auto image = CGDisplayCreateImage(view->get_monitor_data()->handle);

        // failed to capture display, throw exception
        if (image == nullptr) {
            throw std::runtime_error("Failed to capture display");
        }

        // get image size
        auto image_width = CGImageGetWidth(image);
        auto image_height = CGImageGetHeight(image);

        // get bitmap size
        auto bitmap_size = image_width * image_height * (CGImageGetBitsPerPixel(image) / 8);

        CapturedFrame cap {
                static_cast<size_t>(image_width),
                static_cast<size_t>(image_height),
                bitmap_size,
                std::vector<unsigned int>(bitmap_size),
        };

        // Release the image
        CGImageRelease(image);
        return cap;
    }

    CapturedFrame QuartzDevice::do_window_capture(const WindowView *view) {
        return CapturedFrame {
            0, 0, 0, std::vector<unsigned int>()
        };
    }

    QuartzDevice::QuartzDevice() 
        : Device(capture_device_type::osx_quartz) {

    }

    QuartzDevice::~QuartzDevice() {

    }

    bool QuartzDevice::init() {
        return true;
    }

    bool QuartzDevice::cleanup() {
        return true;
    }
}