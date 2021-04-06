#pragma once

#include "../../types.hpp"

#include <nlohmann/json.hpp>

namespace framework {

	using config = nlohmann::json;
	
}

namespace framework::extensions::json {

	config read_config_from_file(const std::string& filename);
	
}
