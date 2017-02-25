#include "Utils/iterativecollection.h"
#include <cstdlib>
#include "Drivers/Goals/goalobject.h"

#define LOG_TAG "IterativeArray"


template<class T>
IterativeCollection<T>::IterativeCollection(GLuint maxSize, AddedToListType type)
{
	static GLint ID_NUM = 1;

	_listType = type;
	_size = maxSize;
	_pointer = 0;

	movingAverage = 0;

	startingNode = new LinkedListItem<T>(0);
	currentNode = startingNode;
	endingNode = startingNode;

	pthread_mutex_init(&_mutex_list, 0);

	_myId = ID_NUM;
	ID_NUM++;
}

template<class T>
IterativeCollection<T>::~IterativeCollection() {

	pthread_mutex_destroy(&_mutex_list);

	deleteAllNodes();
}

template<class T>
void IterativeCollection<T>::deleteAllNodes(){

	LinkedListItem<T> *thisNode = startingNode;
	currentNode = startingNode;
	do{
		currentNode = currentNode->next();
		delete thisNode;
		thisNode = currentNode;
	}
	while(thisNode != 0);

	startingNode = new LinkedListItem<T>(0);
	currentNode = startingNode;
	endingNode = startingNode;
}

template<class T>
GLuint IterativeCollection<T>::getSize(){
	return _pointer;
}

template<class T>
T IterativeCollection<T>::getNext() {
	if(currentNode == 0)
		return 0;

	T item = currentNode->getItem();
	currentNode = currentNode->next();

	return item;
}

template<class T>
T IterativeCollection<T>::getFirst(){
	return startingNode->next() != 0 ? startingNode->next()->getItem() : 0;
}

template<class T>
bool IterativeCollection<T>::addItem(T item)
{
	if(item->addedToListId == _myId)
		return true;

	if(item->addedToList != NO_LIST)
		return false;

	if(_pointer < _size && item->insertionPos == 0){
		_pointer++;
		LinkedListItem<T> *newNode = new LinkedListItem<T>(item);
		endingNode->putNext(newNode);
		newNode->putPrevious(endingNode);
		endingNode = newNode;

		item->insertionPos = newNode;
		item->addedToList = _listType;
		item->addedToListId = _myId;
		return true;
	}
	else
		return false;
}

template<class T>
bool IterativeCollection<T>::removeItem(T item)
{
	if(item->addedToListId != _myId)
		return false;

	if(item->addedToList != _listType)
		return false;

	LinkedListItem<T> *index = item->insertionPos;

	if(index != 0){
		--_pointer;

		if(index == endingNode)
			endingNode = endingNode->prev();

		LinkedListItem<T> *nextItem = index->next();
		index->prev()->putNext(nextItem);
		if(nextItem != 0)
			nextItem->putPrevious(index->prev());

		delete index;

		item->insertionPos = 0;
		item->addedToList = NO_LIST;
		item->addedToListId = 0;
		return true;
	}
	else
		return false;
}

template<class T>
bool IterativeCollection<T>::contains(T item){
	return item->addedToListId == _myId;
}

template<class T>
bool IterativeCollection<T>::hasRoom()
{
	return _pointer < _size;
}

template<class T>
void IterativeCollection<T>::resetArray(uint size)
{
	deleteAllNodes();

	_size = size;

	_pointer = 0;
}

template<class T>
T IterativeCollection<T>::getRandomEntry()
{
	if(_pointer == 0)
		return 0;

	GLint selectionIndex = (GLint)((float)(_pointer)*((float)rand()/(float)RAND_MAX));

	LinkedListItem<T> * placeholder = startingNode;

	GLint i = 0;

	while(i++ <= selectionIndex && placeholder)
		placeholder = placeholder->next();

	if(!placeholder){
		return 0;
	}

	return placeholder->getItem();
}


template<class T>
GLuint IterativeCollection<T>::updateMovingAverage(){
	movingAverage = (GLuint)(((GLfloat)movingAverage)*(0.85f) + ((GLfloat)_pointer)*(0.15f));
	return movingAverage;
}

template<class T>
IterativeCollectionStats IterativeCollection<T>::getIterativeCollectionStats(){
	updateMovingAverage();

	IterativeCollectionStats stats = {
			_listType,
			_size,
			_pointer,
			movingAverage,
	};
	return stats;
}

template<class T>
void IterativeCollection<T>::lock(){
	pthread_mutex_lock(&_mutex_list);
}

template<class T>
void IterativeCollection<T>::unlock(){
	pthread_mutex_unlock(&_mutex_list);
}



template class IterativeCollection<Cube*>;
template class IterativeCollection<GridPosition*>;
template class IterativeCollection<GoalObject*>;
