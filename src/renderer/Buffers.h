#pragma once

#include <glad/glad.h>

#include <vector>
#include <string>

enum class GLSLType
{
	Bool,
	Short,
	UShort,
	Int,
	UInt,
	Float,
	Double,
	Vec2f,
	Vec3f,
	Vec4f,
	Vec2d,
	Vec3d,
	Vec4d,
	Vec2b,
	Vec3b,
	Vec4b,
	Vec2s,
	Vec3s,
	Vec4s,
	Vec2us,
	Vec3us,
	Vec4us,
	Vec2i,
	Vec3i,
	Vec4i,
	Vec2u,
	Vec3u,
	Vec4u,
};

enum class PrimitiveType
{
	Points = GL_POINTS,
	Lines = GL_LINES,
	LineStrip = GL_LINE_STRIP,
	LineLoop = GL_LINE_LOOP,
	Triangles = GL_TRIANGLES,
	TriangleStrip = GL_TRIANGLE_STRIP,
	TriangleFan = GL_TRIANGLE_FAN
};

struct BufferElement
{
	BufferElement(GLSLType type, const std::string& name, bool normalised = false) :
		attributeName(name),
		offset(0),
		size(0),
		type(0),
		bNormalised(normalised)
	{
		switch (type)
		{
		case GLSLType::Bool:	this->type = GL_BYTE;			components = 1;	size = sizeof(bool);			break;
		case GLSLType::Short:	this->type = GL_SHORT;			components = 1; size = sizeof(int16_t);			break;
		case GLSLType::UShort:	this->type = GL_UNSIGNED_SHORT;	components = 1; size = sizeof(uint16_t);		break;
		case GLSLType::Int:		this->type = GL_INT;			components = 1;	size = sizeof(int32_t);			break;
		case GLSLType::UInt:	this->type = GL_UNSIGNED_INT;	components = 1;	size = sizeof(uint32_t);		break;
		case GLSLType::Float:	this->type = GL_FLOAT;			components = 1;	size = sizeof(float);			break;
		case GLSLType::Double:	this->type = GL_DOUBLE;			components = 1;	size = sizeof(double);			break;
		case GLSLType::Vec2f:	this->type = GL_FLOAT;			components = 2;	size = sizeof(float) * 2;		break;
		case GLSLType::Vec3f:	this->type = GL_FLOAT;			components = 3;	size = sizeof(float) * 3;		break;
		case GLSLType::Vec4f:	this->type = GL_FLOAT;			components = 4;	size = sizeof(float) * 4;		break;
		case GLSLType::Vec2d:	this->type = GL_DOUBLE;			components = 2;	size = sizeof(double) * 2;		break;
		case GLSLType::Vec3d:	this->type = GL_DOUBLE;			components = 3;	size = sizeof(double) * 3;		break;
		case GLSLType::Vec4d:	this->type = GL_DOUBLE;			components = 4;	size = sizeof(double) * 4;		break;
		case GLSLType::Vec2b:	this->type = GL_BYTE;			components = 2;	size = sizeof(bool) * 2;		break;
		case GLSLType::Vec3b:	this->type = GL_BYTE;			components = 3;	size = sizeof(bool) * 3;		break;
		case GLSLType::Vec4b:	this->type = GL_BYTE;			components = 4;	size = sizeof(bool) * 4;		break;
		case GLSLType::Vec2s:	this->type = GL_SHORT;			components = 2;	size = sizeof(int16_t) * 2;		break;
		case GLSLType::Vec3s:	this->type = GL_SHORT;			components = 3;	size = sizeof(int16_t) * 3;		break;
		case GLSLType::Vec4s:	this->type = GL_SHORT;			components = 4;	size = sizeof(int16_t) * 4;		break;
		case GLSLType::Vec2us:	this->type = GL_UNSIGNED_SHORT;	components = 2;	size = sizeof(uint16_t) * 2;	break;
		case GLSLType::Vec3us:	this->type = GL_UNSIGNED_SHORT;	components = 3;	size = sizeof(uint16_t) * 3;	break;
		case GLSLType::Vec4us:	this->type = GL_UNSIGNED_SHORT;	components = 4;	size = sizeof(uint16_t) * 4;	break;
		case GLSLType::Vec2i:	this->type = GL_INT;			components = 2;	size = sizeof(int32_t) * 2;		break;
		case GLSLType::Vec3i:	this->type = GL_INT;			components = 3;	size = sizeof(int32_t) * 3;		break;
		case GLSLType::Vec4i:	this->type = GL_INT;			components = 4;	size = sizeof(int32_t) * 4;		break;
		case GLSLType::Vec2u:	this->type = GL_UNSIGNED_INT;	components = 2;	size = sizeof(uint32_t) * 2;	break;
		case GLSLType::Vec3u:	this->type = GL_UNSIGNED_INT;	components = 3;	size = sizeof(uint32_t) * 3;	break;
		case GLSLType::Vec4u:	this->type = GL_UNSIGNED_INT;	components = 4;	size = sizeof(uint32_t) * 4;	break;
		}
	}

	std::string attributeName;
	unsigned short offset;
	unsigned short size;
	unsigned char components;
	unsigned int type;
	bool bNormalised;
};

struct BufferLayout
{
	BufferLayout() : stride(0) {}
	BufferLayout(const std::initializer_list<BufferElement>& element_list) :
		stride(0),
		elements(element_list)
	{
		for (BufferElement& element : elements)
		{
			element.offset = stride;
			stride += element.size;
		}
	}

	unsigned int stride;
	std::vector<BufferElement> elements;
};

class GLBufferObject
{
public:
	~GLBufferObject();

	inline unsigned int Target() const { return bufferTarget; }
	inline unsigned int BufferID() const { return bufferID; }
	inline unsigned int Size() const { return bufferSize; }

	void Bind() const;
	void Unbind() const;

	virtual void operator=(const GLBufferObject& other);

protected:
	GLBufferObject(unsigned int target);

	unsigned int bufferTarget;
	unsigned int bufferID;
	unsigned int bufferSize;
};

class VertexBuffer : public GLBufferObject
{
public:
	VertexBuffer() : GLBufferObject(GL_ARRAY_BUFFER) {}
	VertexBuffer(const void* data, unsigned int size, const BufferLayout& layout);

	BufferLayout layout;
};

class IndexBuffer : public GLBufferObject
{
public:
	IndexBuffer() : GLBufferObject(GL_ELEMENT_ARRAY_BUFFER) {}
	IndexBuffer(unsigned char* data, unsigned int count, PrimitiveType type);
	IndexBuffer(unsigned short* data, unsigned int count, PrimitiveType type);
	IndexBuffer(unsigned int* data, unsigned int count, PrimitiveType type);
};