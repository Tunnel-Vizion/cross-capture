#include <stdexcept>

#include "crosscapture/platform/platform.h"
#include "crosscapture/common/capture_device/device.h"

namespace {
	struct EnumerateWindowsCallbackStruct {
		cross_capture::platform::WindowEnumerateFilter filter;
		std::vector<cross_capture::platform::WindowData> result_vec;
	};

	struct EnumerateMonitorsCallbackStruct {
		std::vector<cross_capture::platform::MonitorData> result_vec;
	};
	
	BOOL CALLBACK enumerate_windows_callback(HWND window_handle, const LPARAM result) {
		constexpr auto buffer_len = MAX_PATH;
		
		auto& callback_struct = *reinterpret_cast<EnumerateWindowsCallbackStruct*>(result);
		wchar_t window_title[buffer_len];

		if (!callback_struct.filter.include_hidden && !IsWindowVisible(window_handle)) {
			return TRUE;
		}
		
		GetWindowTextW(window_handle, window_title, buffer_len);

		if (!callback_struct.filter.include_empty_titles && wcslen(window_title) == 0) {
			return TRUE;
		}
		
		callback_struct.result_vec.emplace_back(cross_capture::platform::WindowData {
			window_handle,
			window_title,
		});
		
		return TRUE;
	}

	BOOL CALLBACK enumerate_monitors_callback(
		HMONITOR monitor_handle, 
		[[maybe_unused]] HDC device_context, 
		[[maybe_unused]] LPRECT rect, 
		const LPARAM result) {
		auto& callback_struct = *reinterpret_cast<EnumerateMonitorsCallbackStruct*>(result);

		MONITORINFOEX monitor_info = { 0 };
		monitor_info.cbSize = sizeof(monitor_info);

		if (GetMonitorInfo(monitor_handle, &monitor_info)) {
			auto* dc = CreateDC(nullptr, monitor_info.szDevice, nullptr, nullptr);
			if (dc) {
				const auto rc_monitor = monitor_info.rcMonitor;

				callback_struct.result_vec.emplace_back(cross_capture::platform::MonitorData {
					monitor_handle,
					monitor_info.szDevice,
					rc_monitor.right - rc_monitor.left,
					rc_monitor.bottom - rc_monitor.top,
					rc_monitor.top,
					rc_monitor.left,
					rc_monitor.right,
					rc_monitor.bottom,
				});
				
				DeleteDC(dc);
			}
		}
		
		return TRUE;
	}
}

namespace cross_capture::platform {
	std::vector<WindowData> enumerate_windows(const WindowEnumerateFilter filter) {
		EnumerateWindowsCallbackStruct callback_struct {
			filter,
			{}
		};

		if (!EnumWindows(enumerate_windows_callback, reinterpret_cast<LPARAM>(&callback_struct))) {
			throw std::runtime_error("unable to enumerate windows!");
		}

		return std::move(callback_struct.result_vec);
	}

	// TODO: Important for windows, check desktop events such as WM_DISPLAYCHANGE
	std::vector<MonitorData> enumerate_monitors() {
		EnumerateMonitorsCallbackStruct callback_struct {
			{}
		};

		if (!EnumDisplayMonitors(
			nullptr,
			nullptr,
			enumerate_monitors_callback,
			reinterpret_cast<LPARAM>(&callback_struct))) {
			throw std::runtime_error("unable to enumerate monitors");
		}
	
		return callback_struct.result_vec;
	}
	
	platform::String get_window_title(const window_handle_t window_handle) {
		wchar_t window_title[MAX_PATH];

		GetWindowTextW(window_handle, window_title, MAX_PATH);

		return String{ window_title };
	}

	bool is_window_handle_valid(const window_handle_t window_handle) {
		return IsWindow(window_handle);
	}

	bool is_monitor_handle_valid(const monitor_handle_t monitor_handle) {
		return false; // TODO: finish
	}

	bool debug_save_bmp(const platform::String& file_name, capture_device::CapturedFrame capture) {
		BITMAPFILEHEADER bmf_header;
		BITMAPINFOHEADER bi;

		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = static_cast<long>(capture.width);
		bi.biHeight = static_cast<long>(capture.height);
		bi.biPlanes = 1;
		bi.biBitCount = 32;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		auto* const file = CreateFile(String(file_name + L".bmp").c_str(),
			GENERIC_WRITE,
			0,
			nullptr,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, 
			nullptr);

		DWORD dw_bytes_written = 0;
		const DWORD dw_bmp_size = static_cast<DWORD>(capture.image_size);
		const DWORD dw_sizeof_dib = dw_bmp_size + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		bmf_header.bfOffBits = static_cast<DWORD>(sizeof(BITMAPFILEHEADER)) + static_cast<DWORD>(sizeof(BITMAPINFOHEADER));
		bmf_header.bfSize = dw_sizeof_dib;
		bmf_header.bfType = 0x4D42;

		WriteFile(file, LPWSTR(&bmf_header), sizeof(BITMAPFILEHEADER), &dw_bytes_written, nullptr);
		WriteFile(file, LPWSTR(&bi), sizeof(BITMAPINFOHEADER), &dw_bytes_written, nullptr);
		WriteFile(file, LPWSTR(&capture.data[0]), dw_bmp_size, &dw_bytes_written, nullptr);

		CloseHandle(file);

		return true;
	}
}