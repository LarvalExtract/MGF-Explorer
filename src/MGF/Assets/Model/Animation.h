#pragma once

namespace MGF {	namespace Asset { namespace Model {

	struct Animation
	{
		std::string name;
		std::string animation;
		float synch_point;
		int loop_count;
		bool primary;
	};

} } }