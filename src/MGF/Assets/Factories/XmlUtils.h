#pragma once

#include <string_view>

#include <QVector3D>
#include <QQuaternion>
#include <QColor>

namespace MA
{
	inline QVector3D StrToVector(const std::string_view str)
	{
		const auto c1 = str.find(',', 0);
		const auto c2 = str.find(',', c1 + 1);

		return QVector3D(
			std::stof(str.substr(0, c1).data()),
			std::stof(str.substr(c1 + 1, c2).data()),
			std::stof(str.substr(c2 + 1, str.size()).data())
		);
	}

	inline QColor StrToColor(const std::string_view str)
	{
		const QVector3D vec = StrToVector(str);
		return QColor::fromRgbF(vec.x() / 255.0f, vec.y() / 255.0f, vec.z() / 255.0f);
	}

	inline QQuaternion StrToQuat(const std::string_view axis, const std::string_view angle)
	{
		return QQuaternion::fromAxisAndAngle(StrToVector(axis), std::stof(angle.data()));
	}
}