#pragma once

#include <memory>
#include <vector>

namespace cross_capture {
	class View;
}

namespace cross_capture::capture_device {
	enum class capture_device_type {
		none,
		win_gdi,
		win_dxgi,
	};

	using capture = std::vector<unsigned int>;

	class Device {
		const capture_device_type device_type_;

		bool initialized_ = false;
	protected:
		explicit Device(const capture_device_type device_type)
			: device_type_(device_type) {}
	public:
		[[nodiscard]] capture_device_type get_device_type() const { return device_type_; }
		[[nodiscard]] bool is_initialized() const { return initialized_; }
		
		virtual bool init() = 0;
		
		virtual capture capture(const View* view) = 0;
	};

	std::unique_ptr<Device> get_suitable_capture_device();
}
