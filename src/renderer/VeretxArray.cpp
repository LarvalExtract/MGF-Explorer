#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);
}

VertexArray::VertexArray(const VertexBuffer& vb, const IndexBuffer& ib)
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
	vertexBuffer.Bind();

	int i = 0;
	for (auto& element : vb.layout.elements)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.components, element.type, element.bNormalised, vb.layout.stride, reinterpret_cast<const void*>(element.offset));
		i++;
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