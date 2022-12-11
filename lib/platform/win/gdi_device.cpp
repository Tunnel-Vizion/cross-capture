#include "crosscapture/platform/win/gdi_device.h"
#include "crosscapture/common/view/monitor_view.h"
#include "crosscapture/common/view/window_view.h"

#include <gdiplusinit.h>
#include <stdexcept>

namespace cross_capture::capture_device {
	CapturedFrame GDIDevice::do_monitor_capture(const MonitorView* view) {
		const auto* const monitor_data = view->get_monitor_data();
		const HDC hdc = CreateDC(monitor_data->name.c_str(), nullptr, nullptr, nullptr);

		if (!hdc) {
			throw std::runtime_error("failed to create monitor view!");
		}

		return process_image(hdc, monitor_data->top, monitor_data->width, monitor_data->height);
	}

	CapturedFrame GDIDevice::do_window_capture(const WindowView* view) {
		const auto* const window_data = view->get_window_data();
		const HDC hdc = GetDC(window_data->handle);

		if (!hdc) {
			throw std::runtime_error("failed to create window view!");
		}

		RECT rc_client;
		GetClientRect(window_data->handle, &rc_client);

		const auto width = rc_client.right - rc_client.left;
		const auto height = rc_client.bottom - rc_client.top;

		return process_image(hdc, 0, width, height);
	}

	CapturedFrame GDIDevice::process_image(const HDC hdc, const long top, const long width, const long height) {
		auto* const window_compat_dc = CreateCompatibleDC(hdc);

		SetStretchBltMode(window_compat_dc, HALFTONE);		

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

		auto* const bitmap = CreateCompatibleBitmap(hdc, width, height);

		SelectObject(window_compat_dc, bitmap);

		const size_t bitmap_size = ((width * bitmap_info.biBitCount + 31) / 32) * 4 * height;

		if (bitmap_size < 0) {
			throw std::runtime_error("tried to process empty bitmap!");
		}

		CapturedFrame cap{
			static_cast<size_t>(width),
			static_cast<size_t>(height),
			bitmap_size,
			std::vector<unsigned int>(bitmap_size),
			0,
			0
		};

		StretchBlt(
			window_compat_dc,
			0,
			0,
			width,
			height,
			hdc,
			top,
			0,
			width,
			height,
			SRCCOPY);

		GetDIBits(
			window_compat_dc,
			bitmap,
			0,
			height,
			&cap.data[0],
			reinterpret_cast<BITMAPINFO*>(&bitmap_info),
			DIB_RGB_COLORS);

		if (!DeleteDC(window_compat_dc)) {
			throw std::runtime_error("failed to delete window dc!");
		}
		if (!DeleteDC(hdc)) {
			throw std::runtime_error("failed to delete hdc!");
		}

		return cap;
	}

	GDIDevice::GDIDevice()
		: Device(capture_device_type::win_gdi) {

	}

	GDIDevice::~GDIDevice() {
		cleanup();
	}

	bool GDIDevice::init() {
		if (Gdiplus::GdiplusStartup(&gdi_plus_token_, &input_, nullptr) == Gdiplus::Status::Ok) {
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
