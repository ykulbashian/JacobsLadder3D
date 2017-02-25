
#ifndef ARRAYUTILS_H_
#define ARRAYUTILS_H_

#include "Utils/iterativelisttypes.h"
#include "Stats/iterativecollectionsstats.h"
#include "Utils/linkedlistitem.h"
#include "Geometry/cube.h"
#include <pthread.h>

#define DEFAULT_ARRAY_SIZE 250
#define MID_ARRAY_SIZE 2000
#define LARGE_ARRAY_SIZE 4000


template<typename T>
class IterativeCollection {

public:

	IterativeCollection() {
		IterativeCollection(DEFAULT_ARRAY_SIZE, MOVING_LIST);
	};

	IterativeCollection(GLuint maxSize, AddedToListType type);

	virtual ~IterativeCollection();

	virtual IterativeCollectionStats getIterativeCollectionStats();

	bool addItem(T item);
	bool removeItem(T item);
	bool hasRoom();
	void resetArray(uint size);

	void lock();
	void unlock();

	bool contains(T item);

	T getRandomEntry();

	GLuint getSize();

	void resetIterator(){
		currentNode = startingNode->next();
	}

	T getNext();


	T getFirst();

	bool hasNext(){
		return currentNode != 0;
	}
	GLuint movingAverage;
	GLuint updateMovingAverage();


private:
	AddedToListType _listType;

    pthread_mutex_t _mutex_list;

    LinkedListItem<T> *startingNode;
    LinkedListItem<T> *endingNode;
    LinkedListItem<T> *currentNode;

	GLuint _pointer;

	GLuint _size;
	GLint _myId;

	void deleteAllNodes();
};


#endif /* ARRAYUTILS_H_ */
