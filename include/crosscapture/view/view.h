#pragma once

namespace cross_capture {
	/**
	 * Types of view.
	 */
	enum class view_type {
		none,
		window,
		monitor,
		custom
	};

	class View {
		const view_type view_type_ = view_type::none;
	protected:
		explicit View(view_type v_type);
	public:
		/**
		 * Returns view type.
		 *
		 * @returns view type.
		 */
		[[nodiscard]] view_type get_view_type() const { return view_type_; }
	};
}
