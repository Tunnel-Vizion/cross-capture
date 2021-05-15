#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif
#include <vector>
#include <string>

namespace cross_capture::platform {
	/// window related garbage ///
#ifdef _WIN32
	using window_handle_t = HWND;
	using monitor_handle_t = HMONITOR;
#else
	using window_handle_t = size_t;
#endif

	/**
	 * Filter for window enumeration.
	 */
	struct WindowEnumerateFilter {
		// should include hidden windows
		bool include_hidden = false;

		// should include windows without titles
		bool include_empty_titles = false;
	};

	/**
	 * Container for window data.
	 */
	struct WindowData {
		// window handle
		window_handle_t handle;

		// window title
		std::wstring title;
	};

	/**
	 * Container for monitor data.
	 */
	struct MonitorData {
		// monitor handle
		monitor_handle_t handle;
	};
	
	/**
	 * Enumerates all windows on platform.
	 *
	 * @param filter enumeration filter.
	 *
	 * @returns vector of all windows found respecting filter.
	 */
	extern std::vector<WindowData> enumerate_windows(WindowEnumerateFilter filter);

	/**
	 * Enumerates all monitors connected to machine.
	 *
	 * @returns vector of all monitors found.
	 */
	extern std::vector<MonitorData> enumerate_monitors();
	
	/**
	 * Returns name of window.
	 *
	 * @param window_handle handle of window to display name of.
	 *
	 * @returns window title.
	 */
	extern std::wstring get_window_title(window_handle_t window_handle);

	/**
	 * Verifies the state of a window handle.
	 *
	 * @param window_handle handle of the window instance.
	 *
	 * @returns true if window handle is valid, otherwise false.
	 */
	extern bool is_window_handle_valid(window_handle_t window_handle);
}
