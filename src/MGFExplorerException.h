#pragma once

#include <exception>

#include "MGF/File.h"

class MGFExplorerException : public std::exception
{
public:
	MGFExplorerException(const const MGF::File& sourceFile) :file(sourceFile) {}

	virtual const char* what() const override { return nullptr; }

private:
	const MGF::File& file;
};