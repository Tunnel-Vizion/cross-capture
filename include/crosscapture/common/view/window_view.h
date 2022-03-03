#pragma once

#include "view.h"
#include "../../platform/platform.h"

namespace cross_capture {
	class WindowView final : public View {
		platform::WindowData* window_data_;
	public:
		explicit WindowView(platform::WindowData* window_handle);

		/**
		 * Returns unique identifier of the view.
		 * 
		 * @returns unique identifier of the view.
		 */
		size_t get_id() const override;

		/**
		 * Returns name of the view.
		 * 
		 * @returns name of the view.
		 */
		std::string get_name() const override;

		/**
		 * Checks whether view is valid for capture.
		 * 
		 * @returns true if view is valid for capture, false otherwise.
		 */
		bool is_valid() const override;

		/**
		 * Get window data.
		 *
		 * @returns window data
		 */
		[[nodiscard]] platform::WindowData* get_window_data() const;
	};
}
