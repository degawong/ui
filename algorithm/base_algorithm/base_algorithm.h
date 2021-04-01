

#pragma once

namespace harpocrates {
	using namespace std;
	class base_algorithm {
	public:
		base_algorithm() = default;
		virtual ~base_algorithm() = default;
	public:
		virtual int apply() = 0;
	public:
		virtual size_t signature();
	};

	class derived_algorithm final : public base_algorithm {
	public:
		virtual ~derived_algorithm() = default;
	public:
		virtual int apply() override;
	public:
		virtual size_t signature() override;
	};
}
