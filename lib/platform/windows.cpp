#include <stdexcept>

#include "crosscapture/platform/platform.h"

// TODO: This is an ugly guard...we should use CMake to make sure this isn't necessary
// to do.
#ifdef _WIN32
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
			window_title
		});
		
		return TRUE;
	}

	BOOL CALLBACK enumerate_monitors_callback(
		HMONITOR monitor_handle, 
		[[maybe_unused]] HDC device_context, 
		[[maybe_unused]] LPRECT rect, 
		const LPARAM result) {
		auto& callback_struct = *reinterpret_cast<EnumerateMonitorsCallbackStruct*>(result);

		callback_struct.result_vec.push_back(cross_capture::platform::MonitorData{
			monitor_handle
		});
		
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
	
		return std::move(callback_struct.result_vec);
	}
	
	std::wstring get_window_title(const window_handle_t window_handle) {
		wchar_t window_title[MAX_PATH];

		GetWindowTextW(window_handle, window_title, MAX_PATH);

		return std::wstring{ window_title };
	}

	bool is_window_handle_valid(const window_handle_t window_handle) {
		return IsWindow(window_handle);
	}
}
#endif
