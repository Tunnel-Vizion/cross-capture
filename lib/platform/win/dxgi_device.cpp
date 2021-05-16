#include "crosscapture/platform/win/dxgi_device.h"

namespace cross_capture::capture_device {
	DXGIDevice::DXGIDevice()
		: Device(capture_device_type::win_dxgi) {
		
	}

	HRESULT DXGIDevice::create_d3d_device() {
		return FALSE;
	}
	
	bool DXGIDevice::init() {
		if (FAILED(create_d3d_device())) {
			return false;
		}
		
		return true;
	}
}
