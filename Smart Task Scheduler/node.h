#pragma once

namespace scheduler {

	class node {

	public:
		virtual void link(node* next) = 0;
		virtual void setData(node* data) = 0;
	};
}