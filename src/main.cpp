#include "crosscapture/crosscapture.h"

int main(int argc, char* argv[]) {
	// Example screen-capture
	
	auto device = cross_capture::capture_device::get_suitable_capture_device();	

	auto count = 1;
	for (auto monitor : cross_capture::platform::enumerate_monitors()) {
		cross_capture::MonitorView view(&monitor);

		const auto screenshot = device->do_capture(&view);
		cross_capture::platform::debug_save_bmp(L"Monitor" + std::to_wstring(count), screenshot);
		count++;
	}

	
	return 0;
}
