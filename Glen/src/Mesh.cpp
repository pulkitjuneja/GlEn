#include "Mesh.h"
#include "ResourceManager.h"


void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	if (hasNormals) {
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
	}
	if (hasTexCoords) {
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	}
	glBindVertexArray(0);
}

Mesh::Mesh(std::vector<Vertex>& vertices,
	std::vector<unsigned int>& indices,
	std::vector<SubMesh>& subMeshes,
	bool hasNormals,
	bool hasTextCoords)
{
	// TODO: make this parameterized. Handle how vertex attributes will be space when somehting is not present.
	this->hasNormals = hasNormals;
	this->hasTexCoords = hasTexCoords;

	this->vertices = vertices;
	this->indices = indices;
	this->subMeshes = subMeshes;
	setupMesh();
}

void Mesh::updateBuffers(std::vector<Vertex>& vertices, glm::vec3* indices) {
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size(), &vertices[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

