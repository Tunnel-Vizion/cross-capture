#pragma once

#ifdef WIN32
#include <d3d11.h>

#include "device.h"

namespace cross_capture::capture_device {
	class DXGIDevice : public Device {
		const D3D_DRIVER_TYPE supported_driver_types_[1] = {
			D3D_DRIVER_TYPE_HARDWARE
		};

		const D3D_FEATURE_LEVEL supported_feature_levels_[4] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_1
		};

		D3D_FEATURE_LEVEL feature_level_;

		HRESULT create_d3d_device();
	public:
		DXGIDevice();

		bool init() override;
	};
}
#endif