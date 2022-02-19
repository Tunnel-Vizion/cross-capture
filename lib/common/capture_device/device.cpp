#include "crosscapture/common/view/window_view.h"
#include "crosscapture/common/view/monitor_view.h"
#include "crosscapture/common/capture_device/device.h"

#ifdef CC_PLATFORM_WIN
#include "crosscapture/platform/win/dxgi_device.h"
#include "crosscapture/platform/win/gdi_device.h"
#elif defined(CC_PLATFORM_OSX)
#include "crosscapture/platform/osx/quartz_device.h"
#endif

#include <stdexcept>

namespace cross_capture::capture_device {
	CapturedFrame Device::do_capture(const View* view, const size_t num_frames, const size_t frame_delay) {
		if (num_frames < 1) {
			throw std::invalid_argument("num_frames must be >= 1");
		}

		if (!initialized_) {
			throw std::runtime_error("device not initialized!");
		}
		
		// TODO: Check out static dispatch, https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern

		switch (view->get_view_type()) {
		case view_type::window: return do_window_capture(dynamic_cast<const WindowView*>(view));
		case view_type::monitor: return do_monitor_capture(dynamic_cast<const MonitorView*>(view));
		case view_type::none: [[fallthrough]];
		default: throw std::runtime_error("TODO: better exception message...");
		}
	}
	
	std::unique_ptr<Device> get_suitable_capture_device() {
		// TODO: Make this generic...
#ifdef WIN32
		auto gdi_device = std::make_unique<GDIDevice>();

		if (!gdi_device->init()) {
			throw std::runtime_error("could not create gdi device");
		}
		
		return std::move(gdi_device);
#elif defined(CC_PLATFORM_OSX)
		auto quartz_device = std::make_unique<QuartzDevice>();

		if (!quartz_device->init()) {
			throw std::runtime_error("could not create quartz device: " + quartz_device->get_error());
		}
		
		return quartz_device;
#else
		return nullptr;
#endif
	}
}