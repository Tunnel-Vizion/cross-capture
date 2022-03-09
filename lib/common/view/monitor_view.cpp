#include "crosscapture/common/view/monitor_view.h"

namespace cross_capture {
	MonitorView::MonitorView(platform::MonitorData* monitor_data)
		: View(view_type::monitor), monitor_data_(monitor_data) {
		
	}

	MonitorView::~MonitorView() {
		
	}
	
	size_t MonitorView::get_id() const {
		return monitor_data_->handle; // TODO: check this is sufficient
	}

	std::string MonitorView::get_name() const {
#ifdef CC_PLATFORM_WIN		
		return platform::str(monitor_data_->name);
#endif
		return monitor_data_->name;
	}

	bool MonitorView::is_valid() const {
		return true;//platform::is_monitor_handle_valid(monitor_data_->handle);
	}

	platform::MonitorData* MonitorView::get_monitor_data() const {
		return monitor_data_;
	}
}
