#include "crosscapture/platform/platform.h"
#include <iostream>

int main(int argc, char* argv[]) {
	const cross_capture::platform::WindowEnumerateFilter params;
	
	for (const auto& window_title : enumerate_windows(params)) {
		std::wcout << window_title.handle << " | " << window_title.title << std::endl;
	}

	return 0;
}
