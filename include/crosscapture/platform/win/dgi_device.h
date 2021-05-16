#pragma once

#include "crosscapture/common/capture_device/device.h"

#include <Windows.h>
#include <gdiplus.h>

namespace cross_capture {
	class WindowView;
	class MonitorView;

	namespace capture_device {
		class GDIDevice : public Device {
			// Handle to Device Context.
			HDC hdc_ {};
			
			Gdiplus::GdiplusStartupInput input_;
			ULONG_PTR gdi_plus_token_ {};

			CapturedFrame do_monitor_capture(const MonitorView* view) override;
			CapturedFrame do_window_capture(const WindowView* view) override;
		public:
			explicit GDIDevice();
			~GDIDevice();

			bool init() override;
			bool cleanup();
		};
	}
}
