#pragma once
#include <core\world\World.h>

namespace utils {

	ds::World* initialize();

	void shutdown(ds::World* w);

}