#pragma once

#ifndef VERTEX_H

#include <unordered_map>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normals;
	glm::vec2 texCoords;
public:
	Vertex(glm::vec3 position, glm::vec3 normals, glm::vec2 texCoords) {
		this->position = position;
		this->normals = normals;
		this->texCoords = texCoords;
	}

	Vertex() {}
};

#endif // !VERTEX_H