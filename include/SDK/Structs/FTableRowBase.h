#pragma once

#include <cstdint>

// Copied from UE4SS for now until the UDataTable changes are pushed over to main
namespace UECustom {
    struct FTableRowBase {
    private:
        int32_t Size;
        int32_t Alignment;

        /**
         * @brief UDataTable is a friend class in order to be able to access and set the Size and Alignment properties within UDataTable::AddRow(FName, FTableRowBase&).
        */
        friend class UDataTable;
    };
}