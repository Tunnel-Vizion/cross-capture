#pragma once

#if defined(_WIN32) || defined(_WIN64)
#define CC_PLATFORM_WIN
#include <Windows.h>
#elif defined(__APPLE__)
#define CC_PLATFORM_OSX
#include <ApplicationServices/ApplicationServices.h>
#endif

#include <memory>
#include <vector>
#include <string>

namespace cross_capture {
	namespace capture_device {
		struct CapturedFrame;
	}
	
	namespace platform {
		/// window related garbage ///
#ifdef CC_PLATFORM_WIN
		using window_handle_t = HWND;
		using monitor_handle_t = HMONITOR;

		using monitor_hdc = std::unique_ptr<std::remove_pointer<HDC>::type, BOOL(*)(HDC)>;

		using monitor_dim = long;
#elif defined(CC_PLATFORM_OSX)
		using window_handle_t = CGWindowID;
		using monitor_handle_t = CGDirectDisplayID;

		using monitor_dim = double;
#else
		using window_handle_t = size_t;
		using monitor_handle_t = size_t; // TODO: update

		using monitor_dim = long;
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
#ifdef CC_PLATFORM_OSX
			// monitor handle
			monitor_handle_t handle = 0;
#else
			monitor_handle_t handle = nullptr;
#endif

			// monitor title
			std::wstring name {};

			// monitor width
			monitor_dim width = 0;

			// monitor height
			monitor_dim height = 0;

			// top coord
			long top = 0;

			// left coord
			long left = 0;

			// right coord
			long right = 0;

			// bottom coord
			long bottom = 0;
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

		/**
		 * Verifies the state of a monitor handle.
		 * 
		 * @param monitor_handle handle of the monitor instance.
		 * 
		 * @returns true if monitor handle is valid, otherwise false.
		 */
		extern bool is_monitor_handle_valid(monitor_handle_t monitor_handle);

		/**
		 * **DEBUG METHOD (not standard and will be moved)**
		 */
		extern bool debug_save_bmp(std::wstring file_name, capture_device::CapturedFrame capture);
	}
}