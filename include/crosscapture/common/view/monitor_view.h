#pragma once

#include "view.h"
#include "../../platform/platform.h"

namespace cross_capture {
	class MonitorView final : public View {
		platform::MonitorData* monitor_data_;
	public:
		explicit MonitorView(platform::MonitorData* monitor_data);
		~MonitorView();

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
		 * Get monitor data.
		 *
		 * @returns monitor data.
		 */
		[[nodiscard]] platform::MonitorData* get_monitor_data() const;
	};
}
