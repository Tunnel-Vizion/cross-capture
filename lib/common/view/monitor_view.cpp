#include "crosscapture/common/view/monitor_view.h"

namespace cross_capture {
	MonitorView::MonitorView(platform::MonitorData* monitor_data)
		: View(view_type::monitor), monitor_data_(monitor_data) {
		
	}

	MonitorView::~MonitorView() {
		
	}
	
	size_t MonitorView::get_id() const {
		return reinterpret_cast<size_t>(monitor_data_->handle); // TODO: check this is sufficient
	}

	platform::String MonitorView::get_name() const {
		return monitor_data_->name;
	}

	bool MonitorView::is_valid() const {
		return platform::is_monitor_handle_valid(monitor_data_->handle);
	}

	platform::MonitorData* MonitorView::get_monitor_data() const {
		return monitor_data_;
	}
}
