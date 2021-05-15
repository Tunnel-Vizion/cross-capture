#include "crosscapture/capture_device/dxgi_device.h"

namespace cross_capture::capture_device {
	DXGIDevice::DXGIDevice()
		: Device(capture_device_type::win_dxgi) {
		
	}

	HRESULT DXGIDevice::create_d3d_device() {
		return FALSE;
	}
	
	bool DXGIDevice::init() {
		if (!create_d3d_device()) {
			return false;
		}
		
		return true;
	}
}