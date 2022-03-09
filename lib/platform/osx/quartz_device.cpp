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
        auto bit_data = CGDataProviderCopyData(CGImageGetDataProvider(image));

        UInt8 * buf = (UInt8 *) CFDataGetBytePtr(bit_data); 
        CFIndex length = CFDataGetLength(bit_data);
        std::vector<unsigned int> bitmap(bitmap_size);

        for (int i = 0; i < length; i+=4) {
            int r = buf[i + 0];
            int g = buf[i + 1];
            int b = buf[i + 2];
            int a = buf[i + 3];

            uint32_t pixel = (a << 24) | (r << 16) | (g << 8) | b;
            bitmap[i/4] = pixel;
        }

        CapturedFrame cap {
                static_cast<size_t>(image_width),
                static_cast<size_t>(image_height),
                bitmap_size,
                std::move(bitmap),
        };

        // Release the image
        CGImageRelease(image);
        CFRelease(bit_data);
        CGDataProviderRelease(CGImageGetDataProvider(image));
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
        // This is a osx specific step. Since OSX 10.15 to have access to certain properties
        // (such as a windows title) and to be able to capture the window content, we need to
        // request access. This could perhaps be a configurable option.
#ifdef OSX_REQUEST_CAPTURE_ACCESS
        if (!CGRequestScreenCaptureAccess()) {
            error_ = "Could not initialize Quartz Device, no screen capture access.";
            return false;
        }
#endif

        initialized_ = true;
        return true;
    }

    bool QuartzDevice::cleanup() {
        return true;
    }
}