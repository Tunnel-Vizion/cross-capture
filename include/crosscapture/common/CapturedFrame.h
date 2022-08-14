// Copyright (c) 2022 Tunnel Vizion. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#pragma once

#include <math>
#include <atomic>
#include <vector>

namespace crosscapture {
	struct CapturedFrame {
		size_t width;
		size_t height;
		// BGRA 8-bit format
		std::vector<uint32_t> buffer;
	};
	using AtomicCapturedFrame = std::atomic<CapturedFrame>;
}
