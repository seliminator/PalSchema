#pragma once

#include <HAL/Platform.hpp>
#include <DynamicOutput/DynamicOutput.hpp>

namespace PS {
    template <RC::Unreal::int32 optional_arg, typename... FmtArgs>
    auto Log(RC::File::StringViewType content, FmtArgs... fmt_args) -> void
    {
        auto formatted_log = std::format(STR("[PalSchema] {}"), content);
        RC::Output::send<optional_arg>(formatted_log, fmt_args...);
    }
}