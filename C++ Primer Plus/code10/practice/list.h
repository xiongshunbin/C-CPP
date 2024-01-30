#ifndef __LIST_H__
#define __LIST_H__

typedef unsigned long Item;

void visit_item(Item & item);

class List
{
	private:
		enum {MAX=10};
		Item items[MAX];
		int top;
	public:
		List();
		bool isempty() const;
		bool isfull() const;
		bool add(const Item & item);
		void visit(void (*pf)(Item &));
};

#endif
