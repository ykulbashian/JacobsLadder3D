
#ifndef ITERATIVECOLLECTIONMEMBER_H_
#define ITERATIVECOLLECTIONMEMBER_H_

#include "Utils/iterativelisttypes.h"
#include "Utils/linkedlistitem.h"
#include "Geometry/position.h"

template<typename T>
class IterativeCollectionMember : public Position{

public:
	AddedToListType addedToList;
	bool inserted;
	LinkedListItem<T> *insertionPos;

	int addedToListId;

	IterativeCollectionMember() : Position(){
		addedToList = NO_LIST;
		inserted = false;
		insertionPos = 0;
		addedToListId = 0;
	}

};


#endif /* ITERATIVECOLLECTIONMEMBER_H_ */
