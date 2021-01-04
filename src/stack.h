// Christy Jacob
// stack.h

#ifndef STACK_H_
#define STACK_H_

#include <cstddef>
#include <cassert>
#include <iostream>

template <class Type>
class stack;

template <class Type>
class stackEntry
{
	friend class stack<Type>;
protected:
	Type              dataValue;
	stackEntry<Type>* pNext;
	stackEntry(const Type &newData, stackEntry<Type> *newNext = nullptr)
	{
		dataValue = newData;
		pNext = newNext;
	}
	virtual ~stackEntry();
	virtual void next(stackEntry<Type> *pNext);
	virtual Type& data();
	virtual const Type& data() const;
	virtual stackEntry<Type>* next();
};

template <class Type>
class stack
{
private:
	stackEntry<Type>* topStack;
	std::size_t entryCount;
public:
	stack();
	stack(const stack &other);
	const stack& operator=(const stack &rhs);
	virtual ~stack();
	const Type& top() const;
	Type& top();
	bool empty() const;
	std::size_t size() const;
	void push(const Type &value);
	void pop();
	void clear();
	void debug() const;
	void debug(std::ostream &out) const;
};

template <class Type>
void stack<Type>::debug() const
{
	debug(std::cout);
}

template <class Type>
void stack<Type>::debug(std::ostream &out) const
{
	out << "Number of items in stack: " << entryCount << std::endl;
	out << "Top of stack is entry @" << topStack << std::endl;

	stackEntry<Type> *pNext = topStack;

	while (pNext != nullptr)
	{
		out << "@" << pNext << ", next @" << pNext->next() << ", data = " << pNext->data() << std::endl;
		pNext = pNext->next();
	}
}

template <class Type>
stackEntry<Type>::~stackEntry()
{
}

template <class Type>
void stackEntry<Type>::next(stackEntry<Type> *pNext)
{
	this->pNext = pNext;
}

template <class Type>
Type& stackEntry<Type>::data()
{
	return dataValue;
}

template <class Type>
const Type& stackEntry<Type>::data() const
{
	return dataValue;
}

template <class Type>
stackEntry<Type>* stackEntry<Type>::next()
{
	return pNext;
}

template <class Type>
stack<Type>::stack()
{
	topStack = nullptr;
	entryCount = 0;
}

template <class Type>
stack<Type>::stack(const stack &other)
{
	if(other.size()>0)
	{
		topStack = new stackEntry<Type>(other.top());
		if(other.topStack->next()!=nullptr)
		{
			topStack->next(new stackEntry<Type>(other.topStack->next()->data()));
		}

		stackEntry<Type> *otherNext = other.topStack->next();
		stackEntry<Type> *thisNext = this->topStack->next();

		while (otherNext != nullptr)
		{
			if(otherNext->next()!=nullptr)
			{
				thisNext->next(new stackEntry<Type>(otherNext->next()->data()));
			}
			otherNext = otherNext->next();
			thisNext = thisNext->next();
		}
	}
	entryCount = other.size();

}

template <class Type>
const stack<Type>& stack<Type>::operator=(const stack &rhs)
{
	if(this!=&rhs)
	{
		clear();
		if(rhs.size()>0)
		{
			topStack = new stackEntry<Type>(rhs.top());
			if(rhs.topStack->next()!=nullptr)
			{
				topStack->next(new stackEntry<Type>(rhs.topStack->next()->data()));
			}

			stackEntry<Type> *otherNext = rhs.topStack->next();
			stackEntry<Type> *thisNext = this->topStack->next();

			while (otherNext != nullptr)
			{
				if(otherNext->next()!=nullptr)
				{
					thisNext->next(new stackEntry<Type>(otherNext->next()->data()));
				}
				otherNext = otherNext->next();
				thisNext = thisNext->next();
			}
		}
		entryCount = rhs.size();
	}
	return *this;
}

template <class Type>
stack<Type>::~stack()
{
	clear();
}

template <class Type>
const Type& stack<Type>::top() const
{
	return topStack->data();
}

template <class Type>
Type& stack<Type>::top()
{
	return topStack->data();
}

template <class Type>
bool stack<Type>::empty() const
{
	return (entryCount==0);
}

template <class Type>
std::size_t stack<Type>::size() const
{
	return entryCount;
}

template <class Type>
void stack<Type>::push(const Type &value)
{
	stackEntry<Type> *newTop = new stackEntry<Type>(value, topStack);
	topStack = newTop;
	entryCount++;
}

template <class Type>
void stack<Type>::pop()
{
	if(entryCount>0)
	{
		stackEntry<Type> *oldTop = topStack;
		topStack = topStack->next();
		delete oldTop;
		entryCount--;
	}
}

template <class Type>
void stack<Type>::clear()
{
	while(entryCount>0)
	{
		pop();
	}
	topStack = nullptr;
}

#endif /* STACK_H_ */

