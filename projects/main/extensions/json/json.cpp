#include "json.hpp"

#include <fstream>

framework::config framework::extensions::json::read_config_from_file(const std::string& filename)
{
	std::ifstream stream(filename);
	nlohmann::json json;

	stream >> json; stream.close();

	return json;
}
