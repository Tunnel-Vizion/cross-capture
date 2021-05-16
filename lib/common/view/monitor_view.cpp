#include "crosscapture/common/view/monitor_view.h"

namespace cross_capture {
	MonitorView::MonitorView(platform::MonitorData* monitor_data)
		: View(view_type::monitor), monitor_data_(monitor_data) {

#ifdef WIN32

#endif
	}

	MonitorView::~MonitorView() {
		
	}
	
	platform::MonitorData* MonitorView::get_monitor_data() const {
		return monitor_data_;
	}

}
