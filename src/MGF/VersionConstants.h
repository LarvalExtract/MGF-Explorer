#pragma once

namespace MGF {

	enum class Version : char
	{
		MechAssault = 2,
		MechAssault2LW = 4
	};

	inline const char* ToString(Version version)
	{
		switch (version)
		{
		case Version::MechAssault: return "MechAssault";
		case Version::MechAssault2LW: return "MechAssault 2: Lone Wolf";
		}
	}
}