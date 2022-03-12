#include "crosscapture/common/view/monitor_view.h"

namespace cross_capture {
	MonitorView::MonitorView(platform::MonitorData* monitor_data)
		: View(view_type::monitor), monitor_data_(monitor_data) {
		
	}

	MonitorView::~MonitorView() {
		
	}
	
	size_t MonitorView::get_id() const {
#ifdef CC_PLATFORM_WIN
		return reinterpret_cast<size_t>(monitor_data_->handle);
#else
		return monitor_data_->handle; // TODO: check this is sufficient
#endif
	}

	std::string MonitorView::get_name() const {
#ifdef CC_PLATFORM_WIN		
		return platform::wstr_to_str(monitor_data_->name);
#else
		return monitor_data_->name;
#endif
	}

	bool MonitorView::is_valid() const {
		return true;//platform::is_monitor_handle_valid(monitor_data_->handle);
	}

	platform::MonitorData* MonitorView::get_monitor_data() const {
		return monitor_data_;
	}
}
