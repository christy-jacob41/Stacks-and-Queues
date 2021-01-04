// Christy Jacob
// queue.h

#ifndef QUEUE_H_
#define QUEUE_H_

#include <cstddef>
#include <cassert>
#include <iostream>

template <class Type>
class queue;

template <class Type>
class queueEntry
{
	friend class queue<Type>;

	protected:
		Type 				dataValue;
		queueEntry<Type> 	*pNext;

		queueEntry(const Type &newData, queueEntry<Type> *newNext = nullptr)
		{
			dataValue = newData;
			pNext = newNext;
		}
		virtual ~queueEntry();
		virtual void next(queueEntry<Type> *pNext);
		virtual queueEntry<Type>* next();
		virtual Type& data();
		virtual const Type& data() const;
};

template <class Type>
class queue
{

	private:
		queueEntry<Type> *pFront;
		queueEntry<Type> *pEnd;
		std::size_t count;
	public:
		queue();
		virtual ~queue();
		queue(const queue &other);
		const queue& operator=(const queue &rhs);
		const Type& front() const;
		Type& front();
		const Type& back() const;
		Type& back();
		bool empty() const;
		std::size_t size() const;
		void push(const Type &value);
		void pop();
		void clear();
		void debug() const;
		void debug(std::ostream &out) const;
		// used by applications using the queue to work with all
		//  of the items in the queue from end to front.
		void traverse(void (*applicationFunction) (const Type &nextItem));
};


template<class Type>
void queue<Type>::debug() const
{
	debug(std::cout);
}

template<class Type>
void queue<Type>::debug(std::ostream &out) const
{

	out << "Number of items in queue: " << count << std::endl;
	out << "Front of queue is entry @" << pFront << std::endl;
	out << "End of queue is entry @" << pEnd << std::endl;
	out << std::endl;

	queueEntry<Type> *pNext = pFront;

	while (pNext != nullptr)
	{
		out << "@" << pNext << ", next @" << pNext->next() << ", data = " << pNext->data() << std::endl;
		pNext = pNext->next();
	}

}

template<class Type>
queueEntry<Type>::~queueEntry()
{
}

template<class Type>
void queueEntry<Type>::next(queueEntry<Type> *pNext)
{
	this->pNext = pNext;
}

template<class Type>
queueEntry<Type>* queueEntry<Type>::next()
{
	return pNext;
}

template<class Type>
Type& queueEntry<Type>::data()
{
	return dataValue;
}

template<class Type>
const Type& queueEntry<Type>::data() const
{
	return dataValue;
}

template<class Type>
queue<Type>::queue()
{
	pFront = nullptr;
	pEnd = nullptr;
	count = 0;
}

template<class Type>
queue<Type>::~queue()
{
	clear();
}

template<class Type>
queue<Type>::queue(const queue &other)
{
	if(other.size()>0)
	{
		pFront = new queueEntry<Type>(other.front());
		if(other.pFront->next()!=nullptr)
		{
			pFront->next(new queueEntry<Type>(other.pFront->next()->data()));
		}

		queueEntry<Type> *otherNext = other.pFront->next();
		queueEntry<Type> *thisNext = this->pFront->next();

		while (otherNext != nullptr)
		{
			if(otherNext->next()!=nullptr)
			{
				thisNext->next(new queueEntry<Type>(otherNext->next()->data()));
			}
			otherNext = otherNext->next();
			thisNext = thisNext->next();
		}
		pEnd = new queueEntry<Type>(other.back());
	}
	count = other.size();
}

template<class Type>
const queue<Type>& queue<Type>::operator=(const queue &rhs)
{
	if(this!=&rhs)
	{
		clear();
		if(rhs.size()>0)
		{
			pFront = new queueEntry<Type>(rhs.front());
			if(rhs.pFront->next()!=nullptr)
			{
				pFront->next(new queueEntry<Type>(rhs.pFront->next()->data()));
			}

			queueEntry<Type> *otherNext = rhs.pFront->next();
			queueEntry<Type> *thisNext = this->pFront->next();

			while (otherNext != nullptr)
			{
				if(otherNext->next()!=nullptr)
				{
					thisNext->next(new queueEntry<Type>(otherNext->next()->data()));
				}
				otherNext = otherNext->next();
				thisNext = thisNext->next();
			}
			pEnd = new queueEntry<Type>(rhs.back());
		}
		count = rhs.size();
	}
	return *this;
}

template<class Type>
const Type& queue<Type>::front() const
{
	return pFront->data();
}

template<class Type>
Type& queue<Type>::front()
{
	return pFront->data();
}

template<class Type>
const Type& queue<Type>::back() const
{
	return pEnd->data();
}

template<class Type>
Type& queue<Type>::back()
{
	return pEnd->data();
}

template<class Type>
bool queue<Type>::empty() const
{
	return (count==0);
}

template<class Type>
std::size_t queue<Type>::size() const
{
	return count;
}

template<class Type>
void queue<Type>::push(const Type &value)
{
	queueEntry<Type> *newBack = new queueEntry<Type>(value);
	if(count == 0)
	{
		pFront = newBack;
	}
	else
	{
		pEnd->next(newBack);
	}
	pEnd = newBack;
	count++;
}

template<class Type>
void queue<Type>::pop()
{
	if(count > 0)
	{
		queueEntry<Type> *oldFront = pFront;
		pFront = pFront->next();
		delete oldFront;
		count--;
	}
}

template<class Type>
void queue<Type>::clear()
{
	while(count>0)
	{
		pop();
	}
	pFront = nullptr;
	pEnd = nullptr;
}

// used by applications using the queue to work with all
//  of the items in the queue from end to front.
template<class Type>
void queue<Type>::traverse(void (*applicationFunction) (const Type &nextItem))
{
	queueEntry<Type> *pNext = pFront;
	while (pNext != nullptr)
	{
		applicationFunction(pNext->data());
		pNext = pNext->next();
	}
}

#endif /* QUEUE_H_ */
