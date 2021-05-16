#include "crosscapture/platform/win/dgi_device.h"
#include "crosscapture/common/view/monitor_view.h"
#include "crosscapture/common/view/window_view.h"

#include <GdiplusInit.h>
#include <stdexcept>

namespace cross_capture::capture_device {
	CapturedFrame GDIDevice::do_monitor_capture(const MonitorView* view) {
		auto* const monitor_data = view->get_monitor_data();

		hdc_ = CreateDC(nullptr, monitor_data->name.c_str(), nullptr, nullptr);

		if (!hdc_) {
			throw std::runtime_error("failed to create monitor view!");
		}

		auto* const monitor_compat_dc = CreateCompatibleDC(hdc_);
		
		SetStretchBltMode(monitor_compat_dc, HALFTONE);
		
		const auto top = monitor_data->top;
		const auto width = monitor_data->width;
		const auto height = monitor_data->height;

		BITMAPINFOHEADER  bitmap_info;

		// Windows...come on... why is all this fucking necessary...
		bitmap_info.biSize = sizeof(BITMAPINFOHEADER);
		bitmap_info.biWidth = width;
		bitmap_info.biHeight = height;
		bitmap_info.biPlanes = 1;
		bitmap_info.biBitCount = 32;
		bitmap_info.biCompression = BI_RGB;
		bitmap_info.biSizeImage = 0;
		bitmap_info.biXPelsPerMeter = 0;
		bitmap_info.biYPelsPerMeter = 0;
		bitmap_info.biClrUsed = 0;
		bitmap_info.biClrImportant = 0;

		auto* const bitmap = CreateCompatibleBitmap(hdc_, width, height);
	
		SelectObject(monitor_compat_dc, bitmap);

		const size_t bitmap_size = ((width * bitmap_info.biBitCount + 31) / 32) * 4 * height;
		
		CapturedFrame cap {
			static_cast<size_t>(width),
			static_cast<size_t>(height),
			bitmap_size,
			std::vector<unsigned int>(bitmap_size),
		};

		StretchBlt(
			monitor_compat_dc, 
			0, 
			0,
			width, 
			height, 
			hdc_,
			top,
			0, 
			width, 
			height, 
			SRCCOPY);
		
		GetDIBits(
			monitor_compat_dc, 
			bitmap, 
			0, 
			height, 
			&cap.data[0], 
			reinterpret_cast<BITMAPINFO*>(&bitmap_info), 
			DIB_RGB_COLORS);
		
		DeleteDC(monitor_compat_dc);
		DeleteDC(hdc_);
		return cap;
	}

	CapturedFrame GDIDevice::do_window_capture(const WindowView* view) {
		return CapturedFrame();
	}
	
	GDIDevice::GDIDevice()
		: Device(capture_device_type::win_gdi) {
		
	}
	
	GDIDevice::~GDIDevice() {
		cleanup();
	}
	
	bool GDIDevice::init() {
		if (Gdiplus::GdiplusStartup(&gdi_plus_token_, &input_, NULL) == Gdiplus::Status::Ok) {
			initialized_ = true;
			return true;
		}
		return false;
	}

	bool GDIDevice::cleanup() {
		Gdiplus::GdiplusShutdown(gdi_plus_token_);

		return true;
	}
}
