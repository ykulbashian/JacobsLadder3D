
#ifndef DIMENSION_H_
#define DIMENSION_H_


class Dimension{
public:
	unsigned short X_BLOCKS, Y_BLOCKS, Z_BLOCKS, TOTAL_BLOCKS;

	GLfloat X_BLOCKS_MID;
	GLfloat Y_BLOCKS_MID;

	Dimension(){
		X_BLOCKS = 0;
		Y_BLOCKS = 0;
		Z_BLOCKS = 0;
		TOTAL_BLOCKS = 0;

		X_BLOCKS_MID = 0.0f;
		Y_BLOCKS_MID = 0.0f;
	}

	Dimension(unsigned short x, unsigned short y, unsigned short z, unsigned short total){
		X_BLOCKS = x;
		Y_BLOCKS = y;
		Z_BLOCKS = z;
		TOTAL_BLOCKS = total;

		X_BLOCKS_MID = ((GLfloat)X_BLOCKS/2.0f);
		Y_BLOCKS_MID = ((GLfloat)Y_BLOCKS/2.0f);
	}

	void setDimension(Dimension dimens){

		X_BLOCKS = dimens.X_BLOCKS;
		Y_BLOCKS = dimens.Y_BLOCKS;
		Z_BLOCKS = dimens.Z_BLOCKS;
		TOTAL_BLOCKS = dimens.TOTAL_BLOCKS;

		X_BLOCKS_MID = ((GLfloat)X_BLOCKS/2.0f);
		Y_BLOCKS_MID = ((GLfloat)Y_BLOCKS/2.0f);
	}


	bool isSimilarDimensions(Dimension dimens)
	{
		bool isSimilar = 	dimens.X_BLOCKS == X_BLOCKS &&
								dimens.Y_BLOCKS == Y_BLOCKS &&
								dimens.Z_BLOCKS == Z_BLOCKS &&
								dimens.TOTAL_BLOCKS == TOTAL_BLOCKS;

		return isSimilar;
	}
};



#endif /* DIMENSION_H_ */
