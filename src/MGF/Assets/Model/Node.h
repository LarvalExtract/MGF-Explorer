#pragma once

#include <string>
#include <vector>

namespace MGF {	namespace Asset { namespace Model {

	struct Node
	{
		std::string name;
		std::string type;
		bool bVisible = true;
		
		Node* parent = nullptr;
		std::vector<Node*> children;
		int childIndex = 0;

		~Node()
		{
			for (auto c : children) delete c;
		}
	};

} } }