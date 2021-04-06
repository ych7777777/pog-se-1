#include "runtime/runtime_process.hpp"

int main()
{
	framework::runtime::runtime_process process;

	process.run_loop();
}