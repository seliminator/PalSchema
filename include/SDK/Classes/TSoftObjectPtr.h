#pragma once

#include "SDK/Classes/TPersistentObjectPtr.h"
#include "SDK/Structs/FSoftObjectPtr.h"

namespace UECustom {
    template<typename UEType>
    class TSoftObjectPtr
    {
    public:
        /** Construct from a soft object path */
        explicit FORCEINLINE TSoftObjectPtr(FSoftObjectPath ObjectPath) : SoftObjectPtr(ObjectPath)
        {
        }

        FSoftObjectPtr Get() const
        {
            return SoftObjectPtr;
        }
    private:
        FSoftObjectPtr SoftObjectPtr;
    };
}