#pragma once

#include "crosscapture/common/capture_device/device.h"

namespace cross_capture {
    class WindowView;
    class MonitorView;

    namespace capture_device {
        class QuartzDevice : public Device {
            CapturedFrame do_monitor_capture(const MonitorView* view) override;
            CapturedFrame do_window_capture(const WindowView* view) override;
            CapturedFrame process_image(CGImageRef image);
        public:
            explicit QuartzDevice();
            ~QuartzDevice();

            bool init() override;
            bool cleanup();
        };
    }
}