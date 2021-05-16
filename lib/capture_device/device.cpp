#include "crosscapture/view/window_view.h"
#include "crosscapture/view/monitor_view.h"
#include "crosscapture/capture_device/device.h"

#include <stdexcept>

#include "crosscapture/capture_device/dgi_device.h"

// TODO: Make this better
#ifdef _WIN32
#include "crosscapture/capture_device/dxgi_device.h"
#endif

namespace cross_capture::capture_device {
	CapturedFrame Device::do_capture(const View* view) {
		if (!initialized_) {
			throw std::runtime_error("device not initialized!");
		}
		
		// TODO: Check out static dispatch, https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
		
		switch (view->get_view_type()) {
		case view_type::window: return do_window_capture(static_cast<const WindowView*>(view));
		case view_type::monitor: return do_monitor_capture(static_cast<const MonitorView*>(view));
		case view_type::none: [[fallthrough]];
		default: throw std::runtime_error("TODO: better exception message...");
		}
	}


	std::unique_ptr<Device> get_suitable_capture_device() {
#ifdef WIN32
		auto gdi_device = std::make_unique<GDIDevice>();

		if (!gdi_device->init()) {
			throw std::runtime_error("could not create gdi device");
		}
		
		return gdi_device;
#endif
		return nullptr;
	}
}