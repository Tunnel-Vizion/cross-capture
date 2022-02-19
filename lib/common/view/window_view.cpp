#include "crosscapture/common/view/window_view.h"

#include <utility>

namespace cross_capture {
	WindowView::WindowView(platform::WindowData* window_data)
		: View(view_type::window), window_data_(window_data) {
		
	}

	size_t WindowView::get_id() const {
		return reinterpret_cast<size_t>(window_data_->handle);
		//return window_data_->handle; // like MonitorView, check this is suitable
	}

	platform::String WindowView::get_name() const {
		// TODO: review this method and the WindowData struct for duplicate
		// data
		return platform::get_window_title(window_data_->handle);
	}

	bool WindowView::is_valid() const {
		return platform::is_window_handle_valid(window_data_->handle);
	}

	platform::WindowData* WindowView::get_window_data() const {
		return window_data_;
	}
}
