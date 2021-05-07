#pragma once

#include "MGF/EFileTypes.h"
#include "MGF/Assets/EAssetTypes.h"

#include <unordered_map>

namespace MGF { namespace Asset {

	const std::unordered_map<MGF::EFileType, MGF::Asset::EAssetType> sAssetMapping = {
		{ MGF::EFileType::Text,	      MGF::Asset::EAssetType::PlainText   },
		{ MGF::EFileType::Tif,	      MGF::Asset::EAssetType::Texture     },
		{ MGF::EFileType::Node,	      MGF::Asset::EAssetType::Model       },
		{ MGF::EFileType::MgModel,    MGF::Asset::EAssetType::Model       },
		{ MGF::EFileType::MgText,     MGF::Asset::EAssetType::StringTable },
		{ MGF::EFileType::Wdf,        MGF::Asset::EAssetType::Entity      },

		{ MGF::EFileType::Unassigned, MGF::Asset::EAssetType::None        }
	};

} }