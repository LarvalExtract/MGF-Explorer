#pragma once

#include "MGFAsset.h"

#include <QString>

class MGFPlainText : public MGFAsset
{
public:
	MGFPlainText(const MGFFile& soureFile);
	QString Text;
};
