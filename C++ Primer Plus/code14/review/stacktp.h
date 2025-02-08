// stacktp.h -- a stack template
#ifndef __STACKTP_H__
#define __STACTP_H__

template <class Type>
class Stack
{
	private:
		enum {MAX=10};// constant specific to class
		Type items[MAX];// holds stack items
		int top;// index for top stack item
	public:
		Stack();
		bool isempty() const;
		bool isfull() const;
		bool push(const Type & item);// add item to stack
		bool pop(Type & item);// pop top into item
};

template <class Type>
Stack<Type>::Stack()
{
	top=0;
}

template <class Type>
bool Stack<Type>::isempty() const
{
	return top==0;
}

template <class Type>
bool Stack<Type>::isfull() const
{
	return top==MAX;
}

template <class Type>
bool Stack<Type>::push(const Type & item)
{
	if(top<MAX)
	{
		items[top++]=item;
		return true;
	}
	else
		return false;
}

template <class Type>
bool Stack<Type>::pop(Type & item)
{
	if(top>0)
	{
		item=items[--top];
		return true;
	}
	else
		return false;
}

#endif