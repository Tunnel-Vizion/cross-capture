#pragma once

#include "view.h"
#include "../platform/platform.h"

namespace cross_capture {
	class MonitorView :public View {
		const platform::MonitorData monitor_data_;
	public:
		explicit MonitorView(platform::MonitorData monitor_data);

		/**
		 * Get monitor data.
		 *
		 * @returns monitor data.
		 */
		[[nodiscard]] platform::MonitorData get_monitor_data() const;
	};
}
