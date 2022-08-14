// Copyright (c) 2022 Tunnel Vizion. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#pragma once

#include <windef.h>
#include <string>

namespace crosscapture {
	struct Display {
		HMONITOR handle = nullptr;
		std::string title;
	};
}