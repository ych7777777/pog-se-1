#pragma once

#include "../../system.hpp"

namespace framework::systems {

	class rectangle_move_system final : public update_system {
	public:
		rectangle_move_system() = default;

		~rectangle_move_system() = default;

		void update(const runtime_service& service, const runtime_frame& frame) override;
	};
	
}
