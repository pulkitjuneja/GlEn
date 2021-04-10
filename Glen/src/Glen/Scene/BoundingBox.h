#pragma once 

#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "Glen/Globals.h"
#include <limits>

class BoundingBox {

	glm::vec3 center;
	glm::vec3 extents;

	// temp variables used in calculation of bounding box
	glm::vec3 min;
	glm::vec3 max;

public: 
	BoundingBox();
	void processVertexPoint(glm::vec3 vertex);
	void FinalizeData();
	glm::vec3 getExtents();
	glm::vec3 getCenter();
	glm::vec3* getCornerPoints();
};
#endif // !BOUNDING_BOX_H
