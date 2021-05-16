#pragma once

#include <memory>
#include <vector>

namespace cross_capture {
	class MonitorView;
	class WindowView;
	class View;
	
	namespace capture_device {
		/**
		 * Capture device enums.
		 */
		enum class capture_device_type {
			none,
			win_gdi,
			win_dxgi,
		};

		/**
		 * Captured frame.
		 */
		struct CapturedFrame {
			// height of image
			size_t width;

			// width of image
			size_t height;

			// TODO: make this more detailed, stop using
			// "image size"

			// size of image including bit depth, compression, ...
			size_t image_size;

			// raw byte data
			std::vector<unsigned int> data;
		};

		class Device {
			// Device type
			const capture_device_type device_type_;
		protected:
			// Initialized boolean
			bool initialized_ = false;

			explicit Device(const capture_device_type device_type)
				: device_type_(device_type) {}

			/**
			 * Captures frame from given window view.
			 *
			 * @returns captured frame.
			 */
			virtual CapturedFrame do_window_capture(const WindowView* view) = 0;

			/**
			 * Captures frame from given monitor view.
			 *
			 * @returns captured frame.
			 */
			virtual CapturedFrame do_monitor_capture(const MonitorView* view) = 0;
		public:
			virtual ~Device() = default;

			/**
			 * Returns device type.
			 *
			 * @returns capture_device_type enum.
			 */
			[[nodiscard]] capture_device_type get_device_type() const { return device_type_; }

			/**
			 * Returns whether device is initialized or not.
			 *
			 * @returns true if initialized, otherwise false.
			 */
			[[nodiscard]] bool is_initialized() const { return initialized_; }

			/**
			 * Initialize capture device.
			 *
			 * @returns true if device is successfully initialized.
			 */
			virtual bool init() = 0;

			/**
			 * Captures single frame from given view.
			 *
			 * @param view view to capture frame from.
			 *
			 * @throws runtime_error if device is not initialized or
			 * view is unrecognized.
			 *
			 * @returns captured frame.
			 */
			CapturedFrame do_capture(const View* view);
		};

		/**
		 * Returns first valid capture device.
		 *
		 * TODO: Add preference for devices
		 *
		 * @returns capture device if one is found, otherwise nullptr.
		 */
		std::unique_ptr<Device> get_suitable_capture_device();
	}
}
