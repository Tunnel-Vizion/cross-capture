#include "crosscapture/capture_device/device.h"

// TODO: Make this better
#ifdef _WIN32
#include "crosscapture/capture_device/dxgi_device.h"
#endif

namespace cross_capture::capture_device {
	std::unique_ptr<Device> get_suitable_capture_device() {
		return nullptr;
	}

}