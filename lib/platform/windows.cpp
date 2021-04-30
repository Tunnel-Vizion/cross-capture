#include "crosscapture/platform/platform.h"

// TODO: This is an ugly guard...we should use CMake to make sure this isn't necessary
// to do.
#ifdef _WIN32
namespace {
	struct EnumerateCallbackStruct {
		cross_capture::platform::WindowEnumerateFilter filter;
		std::vector<cross_capture::platform::WindowData> result_vec;
	};
	
	BOOL CALLBACK enumerate_windows_callback(HWND window_handle, const LPARAM result_vector) {
		constexpr auto buffer_len = MAX_PATH;
		
		auto& callback_struct = *reinterpret_cast<EnumerateCallbackStruct*>(result_vector);
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
}

namespace cross_capture::platform {
	std::vector<WindowData> enumerate_windows(const WindowEnumerateFilter filter) {
		EnumerateCallbackStruct callback_struct {
			filter,
			{}
		};

		EnumWindows(enumerate_windows_callback, reinterpret_cast<LPARAM>(&callback_struct));

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
