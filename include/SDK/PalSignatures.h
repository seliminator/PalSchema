#pragma once

#include <filesystem>
#include <unordered_map>
#include <string>

namespace Palworld {
    class SignatureManager {
    public:
        static void Initialize();
        
        // Example of expected parameter: UObjectGlobals::StaticLoadClass
        static void* GetSignature(const std::string& ClassAndFunction);
    private:
        static inline std::unordered_map<std::string, void*> SignatureMap;

        static inline std::unordered_map<std::string, std::string> Signatures {
            { "UDataTable::HandleDataTableChanged", "48 89 54 24 10 55 53 56 48 8D 6C 24 B9" },
        };
        static inline std::unordered_map<std::string, std::string> SignaturesCallResolve {
        };
    };
}