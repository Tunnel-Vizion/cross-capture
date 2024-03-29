#include "crosscapture/crosscapture.h"
#include <iostream>

// parse command line args
int parse_command_line_args() {
	return 0;
}

int main(int argc, char* argv[]) {
	parse_command_line_args();
	// Example screen-capture
	
	try {
		const auto device = cross_capture::capture_device::get_suitable_capture_device();

		auto count = 1;
		for (auto monitor : cross_capture::platform::enumerate_monitors()) {
			cross_capture::MonitorView view(&monitor);

			const auto screenshot = device->do_capture(&view);
			cross_capture::platform::debug_save_bmp("Monitor" + std::to_string(count), screenshot);
			count++;
		}

		auto windows = cross_capture::platform::enumerate_windows(cross_capture::platform::WindowEnumerateFilter{});
		for (auto window : windows) {
			cross_capture::WindowView view(&window);
			
			const auto screenshot = device->do_capture(&view);
			cross_capture::platform::debug_save_bmp(window.title, screenshot);
			count++;
		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
