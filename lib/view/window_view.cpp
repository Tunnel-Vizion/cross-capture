#include <utility>

#include "crosscapture/view/window_view.h"

namespace cross_capture {
	bool WindowView::is_window_destroyed() const {
		return platform::is_window_handle_valid(window_data_.handle);
	}
	
	WindowView::WindowView(platform::WindowData window_handle)
		: View(view_type::window), window_data_(std::move(window_handle)) {
		
	}

	std::wstring WindowView::get_window_title() const {
		// TODO: review this method and the WindowData struct for duplicate
		// data
		return platform::get_window_title(window_data_.handle);
	}
}
