#pragma once

#include <string>

#include "view.h"
#include "../../platform/platform.h"

namespace cross_capture {
	class WindowView : public View {
		const platform::WindowData window_data_;

		[[nodiscard]] bool is_window_destroyed() const;
	public:
		explicit WindowView(platform::WindowData window_handle);

		/**
		 * Get title of window being viewed.
		 *
		 * @returns window title.
		 */
		[[nodiscard]] std::wstring get_window_title() const;

		/**
		 * Get window data.
		 *
		 * @returns window data
		 */
		[[nodiscard]] platform::WindowData get_window_data() const;
	};
}
