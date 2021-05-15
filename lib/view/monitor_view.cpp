#include "crosscapture/view/monitor_view.h"

namespace cross_capture {
	MonitorView::MonitorView(const platform::MonitorData monitor_data)
		: View(view_type::monitor), monitor_data_(monitor_data) {
		
	}

}