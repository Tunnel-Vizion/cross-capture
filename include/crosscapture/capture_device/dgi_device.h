#pragma once

#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>

#include "device.h"

#ifdef WIN32
namespace cross_capture::capture_device {
	class GDIDevice : public Device {
		Gdiplus::GdiplusStartupInput input_;
		ULONG_PTR gdi_plus_token_;

		CapturedFrame do_monitor_capture(const MonitorView* view) override;
		CapturedFrame do_window_capture(const WindowView* view) override;
	public:
		explicit GDIDevice();
		~GDIDevice();

		bool init() override;
		bool cleanup();
	};
}
#endif