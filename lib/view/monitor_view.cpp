#include "crosscapture/view/monitor_view.h"

#include <stdexcept>

namespace cross_capture {
	MonitorView::MonitorView(platform::MonitorData* monitor_data)
		: View(view_type::monitor), monitor_data_(monitor_data) {

#ifdef WIN32
		hdc_ = CreateDC(nullptr, monitor_data->name.c_str(), nullptr, nullptr);

		if (!hdc_) {
			throw std::runtime_error("failed to create monitor view!");
		}
#endif
	}

	MonitorView::~MonitorView() {
		DeleteDC(hdc_);
	}
	
	platform::MonitorData* MonitorView::get_monitor_data() const {
		return monitor_data_;
	}

	HDC MonitorView::get_hdc() const {
		return hdc_;
	}

}
