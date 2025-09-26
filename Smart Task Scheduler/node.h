#pragma once

namespace scheduler {

	class node {

	private:
		node* next;
		node* data;

	public:
		virtual void link() = 0;
		virtual void setData() = 0;
		node();
		~node();

	};
}