#pragma once

#include "extensions/spdlog/spdlog.hpp"

#include <unordered_map>

namespace framework {

	using uint64 = unsigned long long;
	using uint32 = unsigned;
	using real = float;
	using byte = unsigned char;

	template <typename Key, typename Value>
	using mapping = std::unordered_map<Key, Value>;
	
}
