// Copyright (c) 2022 Tunnel Vizion. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#pragma once

#include <vector>

#if defined(WIN32)
#	include <dxgiformat.h>
#endif

#include <crosscapture/common/Display.h>

namespace crosscapture::platform {
	std::vector<Display> enumerate_displays();
	void enumerate_windows();
}
