#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
	min = glm::vec3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	max = glm::vec3(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
}

void BoundingBox::processVertexPoint(glm::vec3 vertex)
{
	if (vertex.x > max.x) max.x = vertex.x;
	if (vertex.x < min.x) min.x = vertex.x;
	if (vertex.y > max.y) max.y = vertex.y;
	if (vertex.y < min.y) min.y = vertex.y;
	if (vertex.z > max.z) max.z = vertex.z;
	if (vertex.z < min.z) min.z = vertex.z;
}

void BoundingBox::FinalizeData()
{
	extents = glm::vec3((max.x - min.x), (max.y - min.y), (max.z - min.z));
	center = glm::vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
}

glm::vec3 BoundingBox::getExtents()
{
	return extents;
}

glm::vec3 BoundingBox::getCenter()
{
	return center;
}

glm::vec3* BoundingBox::getCornerPoints()
{
	glm::vec3 corners[8];
	corners[0] = center + glm::vec3(extents.x, extents.y, extents.z);
	corners[1] = center + glm::vec3(extents.x, extents.y, -extents.z);
	corners[2] = center + glm::vec3(extents.x, -extents.y, extents.z);
	corners[3] = center + glm::vec3(extents.x, -extents.y, -extents.z);
	corners[4] = center + glm::vec3(-extents.x, extents.y, extents.z);
	corners[5] = center + glm::vec3(-extents.x, extents.y, -extents.z);
	corners[6] = center + glm::vec3(-extents.x, -extents.y, extents.z);
	corners[7] = center + glm::vec3(-extents.x, -extents.y, -extents.z);

	return corners;
}
