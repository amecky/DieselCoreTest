#include "utils.h"

namespace utils {

	ds::World* initialize() {		
		perf::reset();
		ds::World* w = new ds::World();
		return w;
	}

	void shutdown(ds::World* w) {		
		delete w;
	}

}