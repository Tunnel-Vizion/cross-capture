#pragma once

#include <stdexcept>
#include <vector>
#include <string>
#include "crosscapture/platform/platform.h"

namespace cross_capture {
	/**
	 * Types of view.
	 */
	enum class view_type {
		none,
		window,
		monitor,
		group,
		custom
	};

	/**
	 * Abstract view class.
	 * 
	 * Represents a view that can be captured.
	 */
	class View {
		const view_type view_type_ = view_type::none;
	protected:
		/**
		 * Constructor.
		 * 
		 * @param view_type Type of view.
		 */
		explicit View(view_type v_type) : view_type_(v_type) {}
	public:
		/**
		 * Returns unique identifier of the view.
		 * 
		 * @returns unique identifier of the view.
		 */
		virtual size_t get_id() const = 0;

		/**
		 * Return name of the view.
		 * 
		 * @returns name of the view.
		 */
		virtual std::string get_name() const = 0;

		/**
		 * Returns view type.
		 *
		 * @returns view type.
		 */
		[[nodiscard]] view_type get_view_type() const { return view_type_; }

		/**
		 * Checks whether view is valid for capture.
		 * 
		 * @returns true if view is valid for capture, false otherwise.
		 */
		[[nodiscard]] virtual bool is_valid() const = 0;
	};

	/**
	 * Container for multiple views. Used to represent a group of views
	 * which are captured together.
	 */
	class ViewGroup final : public View {
		static size_t NUM_VIEWS;

		std::vector<std::shared_ptr<View>> views_;
		std::string name = std::string();
	protected:
		/**
		 * Constructs a view group.
		 *
		 * @param v_type view type.
		 * @param views vector of views.
		 */
		explicit ViewGroup(const std::vector<std::shared_ptr<View>>& views) : View(view_type::group), views_(views) {
			NUM_VIEWS++;
			name = "Group " + std::to_string(NUM_VIEWS);
		}

		~ViewGroup() {
			NUM_VIEWS--;
		}
	public:
		/**
		 * Returns unique identifier of the view.
		 * 
		 * @returns unique identifier of the view.
		 */
		[[nodiscard]] size_t get_id() const override {
			// temporary method of generating ID for view groups
			size_t id = 0;
			for (const auto& view : views_) {
				id ^= view->get_id();
			}
			return id;
		}

		/**
		 * Returns name of the view.
		 * 
		 * @returns name of the view.
		 */
		std::string get_name() const override {
			return name;
		}

		/**
		 * Set name of the view.
		 * 
		 * @param name name of the view.
		 */
		void set_name(const std::string& name) {
			this->name = name;
		}

		/**
		 * Checks whether view is valid for capture.
		 * 
		 * @returns true if view is valid for capture, false otherwise.
		 */
		bool is_valid() const override {
			for (const auto& view : views_) {
				if (!view->is_valid()) {
					return false;
				}
			}
			return true;
		}

		/**
		 * Returns views.
		 *
		 * @returns views.
		 */
		[[nodiscard]] const std::vector<std::shared_ptr<View>>& get_views() const { return views_; }

		/**
		 * Adds view to group.
		 * 
		 * @param view view to add.
		 * 
		 * @returns true if view was added, false otherwise.
		 * 
		 * @throws std::invalid_argument if view is null.
		 * @throws std::invalid_argument if view is already in group.
		 */
		bool add_view(const std::shared_ptr<View>& view) {
			if (!view) {
				throw std::invalid_argument("view is null");
			}

			// groups cannot be nested
			if (view->get_view_type() == view_type::group) {
				throw std::invalid_argument("cannot include groups in groups");
			}

			// check whether a view with the same id is already in the group
			for (const auto& v : views_) {
				if (v->get_id() == view->get_id()) {
					throw std::invalid_argument("view is already in group");
				}
			}

			views_.push_back(view);
			return true;
		}
	};
}
