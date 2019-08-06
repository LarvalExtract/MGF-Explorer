#pragma once

#include "Buffers.h"

class VertexArray
{
public:
	VertexArray();
	VertexArray(const VertexBuffer& vb, const IndexBuffer& ib);
	~VertexArray();

	void SetVertexBuffer(const VertexBuffer& vb);
	void SetIndexBuffer(const IndexBuffer& ib);

	void Bind() const;
	void Unbind() const;

private:
	unsigned int vertexArrayID;

	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
};
