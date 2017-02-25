#include "colorset.h"

ColorSetStruct ColorSet::colorSetStruct =
{

{ 3, 15, 33, 255 },

{
	{	//	TOP LEFT
			{ 93, 172, 239, 255 },	// LEFT
			{ 127, 222, 255, 255 },	// RIGHT
			{ 88, 180, 244, 255 }, // TOP
			{ 94, 205, 244, 255 }, // BOTTOM
			{ 90, 223, 244, 255 }, // BACK
	},{	//	TOP RIGHT
			{ 93, 172, 239, 255 },	// LEFT
			{ 127, 222, 255, 255 },	// RIGHT
			{ 88, 180, 244, 255 }, // TOP
			{ 94, 205, 244, 255 }, // BOTTOM
			{ 90, 223, 244, 255 }, // BACK
	},{	//	BOTTOM LEFT
			{ 93, 172, 239, 255 },	// LEFT
			{ 127, 222, 255, 255 },	// RIGHT
			{ 88, 180, 244, 255 }, // TOP
			{ 94, 205, 244, 255 }, // BOTTOM
			{ 90, 223, 244, 255 }, // BACK
	},{	//	BOTTOM RIGHT
			{ 93, 172, 239, 255 },	// LEFT
			{ 127, 222, 255, 255 },	// RIGHT
			{ 88, 180, 244, 255 }, // TOP
			{ 94, 205, 244, 255 }, // BOTTOM
			{ 90, 223, 244, 255 }, // BACK
	}
},

{
		(Light){ // bottom left
			"Bottom left",
			0.2f, 0.2f, -0.2f,
			6, 172, 245,  709
		},
		(Light){ //top right
			"Top Right",
			0.8f, 0.8f, -0.2f,
			17, 120, 181,  690
		},
		(Light){ //top left
			"Top left",
			0.2f, 0.8f, -0.2f,
			24, 122, 198,  721
		},
		(Light){ // bottom right
			"Bottom right",
			0.8f, 0.2f, -0.2f,
			22, 145, 244,  803
		},

	(Light){ // primary
		"Primary",
		0.5f, 0.5f, 0.1f,
		7, 107, 153,  1066
	},
	(Light){ // Distant bright
		"Distant",
		0.5f, 0.5f, 0.8f,
		0, 255, 202,  1349
	},
	(Light){ // Mid-distance
		"Mid",
		0.5f, 0.5f, 0.5f,
		51, 171, 210,  1342
	},

},
	//has background
	true,

};

