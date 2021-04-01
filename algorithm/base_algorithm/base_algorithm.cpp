
#include <typeinfo>
#include <type_traits>

#include "base_algorithm.h"

namespace harpocrates {
	size_t base_algorithm::signature() {
		return typeid(*this).hash_code();
	}
	int derived_algorithm::apply() {
		return 0;
	}
	size_t derived_algorithm::signature() {
		return typeid(*this).hash_code();
	}
}
