#include "crosscapture/platform/platform.h"
#include <iostream>


#include "crosscapture/capture_device/dgi_device.h"
#include "crosscapture/view/monitor_view.h"

#pragma comment(lib, "gdiplus.lib")

int main(int argc, char* argv[]) {
	// Example screen-capture

	auto device = cross_capture::capture_device::get_suitable_capture_device();

	auto count = 1;
	for (auto monitor : cross_capture::platform::enumerate_monitors()) {
#ifdef WIN32
		cross_capture::MonitorView view(&monitor);
		const auto screenshot = device->do_capture(&view);

		cross_capture::platform::debug_save_bmp("MONITOR_" + std::to_string(count), screenshot);
		count++;
#endif
	}
	
	return 0;
}
