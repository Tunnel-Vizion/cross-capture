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
        
        return process_image(image);
    }

    CapturedFrame QuartzDevice::do_window_capture(const WindowView *view) {
        auto image = CGWindowListCreateImage(
                CGRectNull,
                kCGWindowListOptionIncludingWindow,
                view->get_id(),
                kCGWindowImageBoundsIgnoreFraming);

        // failed to capture display, throw exception
        if (image == nullptr) {
            throw std::runtime_error("Failed to capture display");
        }

        return process_image(image);
    }

    CapturedFrame QuartzDevice::process_image(CGImageRef image) {
        // get image size
        auto image_width = CGImageGetWidth(image);
        auto image_height = CGImageGetHeight(image);

        // get bits per pixel
        auto bits_per_pixel = CGImageGetBitsPerPixel(image);

        // get bytes per row
        auto bytes_per_row = CGImageGetBytesPerRow(image);

        // get bitmap size
        auto bitmap_size = image_width * image_height * (CGImageGetBitsPerPixel(image) / 8);

        // get bitmap data
        auto bit_data = CGDataProviderCopyData(CGImageGetDataProvider(image));
        
        UInt8 * buf = (UInt8 *) CFDataGetBytePtr(bit_data);
        CFIndex length = CFDataGetLength(bit_data);
        std::vector<uint32_t> bitmap(bitmap_size);

        for (int i = 0; i < length; i+=4) {
            auto r = buf[i + 0];
            auto g = buf[i + 1];
            auto b = buf[i + 2];
            auto a = buf[i + 3];

            uint32_t pixel = (a << 24) | (r << 16) | (g << 8) | b;
            bitmap[i/4] = pixel;
        }

        CapturedFrame cap {
                static_cast<size_t>(image_width),
                static_cast<size_t>(image_height),
                bitmap_size,
                std::move(bitmap),
                bytes_per_row,
                bits_per_pixel
        };

        // Release the image
        CGImageRelease(image);
        CFRelease(bit_data);
        CGDataProviderRelease(CGImageGetDataProvider(image));
        return cap;
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
        if (CGPreflightScreenCaptureAccess() != kCGErrorSuccess) {
            if (!CGRequestScreenCaptureAccess()) {
                error_ = "Could not initialize Quartz Device, no screen capture access.";
                return false;
            }
        }
#endif

        initialized_ = true;
        return true;
    }

    bool QuartzDevice::cleanup() {
        return true;
    }
}