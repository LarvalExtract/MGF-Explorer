#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
}

VertexArray::VertexArray(const VertexBuffer& vb, const IndexBuffer& ib)
	//vertexBuffer(vb),
	//indexBuffer(ib)
{
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	SetVertexBuffer(vb);
	SetIndexBuffer(ib);
}

VertexArray::~VertexArray()
{
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void VertexArray::SetVertexBuffer(const VertexBuffer& vb)
{
	vertexBuffer = vb;

	Bind();
	glBindVertexBuffer(0, vertexBuffer.BufferID(), 0, vertexBuffer.layout.stride);

	for (auto& element : vb.layout.elements)
	{
		if (element.location != -1)
		{
			glEnableVertexAttribArray(element.location);
			glVertexAttribFormat(element.location, element.components, element.type, GL_FALSE, element.offset);
			glVertexAttribBinding(element.location, 0);
		}
	}
}

void VertexArray::SetIndexBuffer(const IndexBuffer& ib)
{
	indexBuffer = ib;
}

void VertexArray::Bind() const
{
	glBindVertexArray(vertexArrayID);
	indexBuffer.Bind();
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::operator=(const VertexArray& other)
{
	// Destroy current vertex array
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vertexArrayID);

	// Generate a new one
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	

	SetVertexBuffer(other.vertexBuffer);
	SetIndexBuffer(other.indexBuffer);
}