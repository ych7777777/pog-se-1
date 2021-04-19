#pragma once

#include "../../system.hpp"

namespace framework::systems {

	class rectangle_draw_system final : public update_system {
	public:
		rectangle_draw_system() = default;

		~rectangle_draw_system() = default;

		void update(const runtime_service& service, const runtime_frame& frame) override;
	private:
		float mRed = 1, mGreen = 0, mBlue = 0;
		float mSize = 100;
	};
	
}
