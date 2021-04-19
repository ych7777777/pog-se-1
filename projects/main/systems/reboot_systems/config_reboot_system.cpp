#include "config_reboot_system.hpp"

using framework::extensions::json::read_config_from_file;

void framework::systems::config_reboot_system::resolve(const runtime_service& service)
{
	// add your config file from resources

	service.resource_system.add("config", read_config_from_file("./resources/configs/default_config.json"));
	//service.resource_system.add("config", read_config_from_file("./resources/configs/simple_config.json"));
}

void framework::systems::config_reboot_system::release(const runtime_service& service)
{
	// release your resources
}
