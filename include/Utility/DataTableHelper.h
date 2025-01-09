#pragma once

#include <cstdint>
#include "nlohmann/json.hpp"

namespace RC::Unreal {
	class FProperty;
}

namespace Palworld::DataTableHelper {
	void CopyJsonValueToTableRow(void* TableRow, RC::Unreal::FProperty* Property, const nlohmann::json& Value);
}