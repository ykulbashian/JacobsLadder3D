
#ifndef ALLITERATIVECOLLECTIONS_H_
#define ALLITERATIVECOLLECTIONS_H_

#include "Geometry/cube.h"
#include "Utils/iterativecollection.h"
#include "Stats/iterativecollectionsstats.h"

class AllIterativeCollections {

public:
	IterativeCollection<Cube*> *selectGroup;
	IterativeCollection<Cube*> *priorityGroup;
	IterativeCollection<Cube*> *movingElements;
	IterativeCollection<Cube*> *candidateGroup;

	AllIterativeCollections();
	~AllIterativeCollections();

	void initializeArrays();
	void deleteArrays();
	void revertArrays();

	bool addCubeToList(Cube *cube, const AddedToListType type);

	void getIterativeCollectionStats(IterativeCollectionStats (&stats)[4]);

	GLuint moveUnsupportedToPriority();

	GLuint moveUnsupportedToPriorityList(IterativeCollection<Cube*> *listToCheck);

private:

	bool doesListHaveRoom(const AddedToListType type);
};


#endif /* ALLITERATIVECOLLECTIONS_H_ */
