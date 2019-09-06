#pragma once

#include "Buffers.h"

class VertexArray
{
public:
	VertexArray();
	VertexArray(const VertexArray& other) { *this = other; }
	VertexArray(const VertexBuffer& vb, const IndexBuffer& ib);
	~VertexArray();

	void SetVertexBuffer(const VertexBuffer& vb);
	void SetIndexBuffer(const IndexBuffer& ib);

	void Bind() const;
	void Unbind() const;

	unsigned int GetIndexCount() const { return indexBuffer.GetIndexCount(); }
	PrimitiveType GetIndexMode() const { return indexBuffer.GetType(); }

	void operator=(const VertexArray& other);

private:
	unsigned int vertexArrayID;

	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
};

