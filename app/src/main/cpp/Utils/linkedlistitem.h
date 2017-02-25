
#ifndef LINKEDLISTITEM_H_
#define LINKEDLISTITEM_H_


template<class T>
class LinkedListItem {
public:
	LinkedListItem(T startItem){
		nextItem = 0;
		previousItem = 0;
		item = startItem;
	}

	bool hasNext(){
		return nextItem != 0;
	}

	LinkedListItem<T> *next(){
		return nextItem;
	}

	LinkedListItem<T> *prev(){
		return previousItem;
	}

	T getItem(){
		return item;
	}

	void putNext(LinkedListItem<T> *next){
		nextItem = next;
	}

	void putPrevious(LinkedListItem<T> *prev){
		previousItem = prev;
	}

	T item;
	LinkedListItem<T> *nextItem;
	LinkedListItem<T> *previousItem;

};

#endif /* LINKEDLISTITEM_H_ */
