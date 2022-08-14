// Copyright (c) 2022 Tunnel Vizion. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

namespace crosscapture {
	enum class CaptureDeviceType {
		WinDXGIDeskDup, // Windows DXGI Desktop Duplication API Device
	};

	class CaptureDevice {
		
	};
}

/*
 * auto device = crosscapture::get_device(preference::something);
 *
 * device->capture();
 */
