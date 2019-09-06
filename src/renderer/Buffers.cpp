#include "Buffers.h"

GLBufferObject::GLBufferObject(unsigned int target) :
	bufferTarget(target),
	bufferID(0),
	bufferSize(0)
{
	glGenBuffers(1, &bufferID);
	glBindBuffer(target, bufferID);
}

GLBufferObject::~GLBufferObject()
{
	glBindBuffer(bufferTarget, 0);
	glDeleteBuffers(1, &bufferID);
}

void GLBufferObject::Bind() const
{
	glBindBuffer(bufferTarget, bufferID);
}

void GLBufferObject::Unbind() const
{
	glBindBuffer(bufferTarget, 0);
}

void GLBufferObject::operator=(const GLBufferObject& other)
{
	glBindBuffer(GL_COPY_WRITE_BUFFER, this->bufferID);
	glBindBuffer(GL_COPY_READ_BUFFER, other.BufferID());

	glBufferData(GL_COPY_WRITE_BUFFER, other.Size(), nullptr, GL_STATIC_DRAW);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, other.Size());

	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(GL_COPY_READ_BUFFER, 0);

	glBindBuffer(bufferTarget, this->bufferID);
	this->bufferSize = other.Size();
}


VertexBuffer::VertexBuffer(const void* data, unsigned int size, const BufferLayout& layout) :
	GLBufferObject(GL_ARRAY_BUFFER),
	layout(layout)
{
	bufferSize = size;
	glBufferData(bufferTarget, bufferSize, data, GL_STATIC_DRAW);
}

//void VertexBuffer::operator=(const VertexBuffer& other)
//{
//	*static_cast<GLBufferObject*>(this) = other;
//	this->layout = other.layout;
//}

IndexBuffer::IndexBuffer(unsigned char* data, unsigned int count, PrimitiveType type) :
	GLBufferObject(GL_ELEMENT_ARRAY_BUFFER),
	count(count),
	type(type)
{
	bufferSize = sizeof(unsigned char) * count;
	glBufferData(bufferTarget, bufferSize, data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(unsigned short* data, unsigned int count, PrimitiveType type) :
	GLBufferObject(GL_ELEMENT_ARRAY_BUFFER),
	count(count),
	type(type)
{
	bufferSize = sizeof(unsigned short) * count;
	glBufferData(bufferTarget, bufferSize, data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count, PrimitiveType type) :
	GLBufferObject(GL_ELEMENT_ARRAY_BUFFER),
	count(count),
	type(type)
{
	bufferSize = sizeof(unsigned int) * count;
	glBufferData(bufferTarget, bufferSize, data, GL_STATIC_DRAW);
}
