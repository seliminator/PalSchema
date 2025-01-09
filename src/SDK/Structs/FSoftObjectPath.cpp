#include <SDK/Structs/FSoftObjectPath.h>
#include <SDK/Classes/KismetSystemLibrary.h>
#include <SDK/Classes/AsciiSet.h>

using namespace RC;
using namespace RC::Unreal;

namespace UECustom {
	FSoftObjectPath::FSoftObjectPath(RC::StringViewType Path)
	{
		if (Path == STR("") || Path == STR("None"))
		{
			Reset();
		}
		else
		{
			// Possibly an ExportText path. Trim the ClassName.
			// Path = FPackageName::ExportTextPathToObjectPath(Path);

			FAsciiSet Delimiters = FAsciiSet(".") + (char)SUBOBJECT_DELIMITER_CHAR;
			if (!Path.starts_with('/')  // Must start with a package path 
				|| Delimiters.Contains(Path[Path.size() - 1]) // Must not end with a trailing delimiter
				)
			{
				// Not a recognized path. No ensure/logging here because many things attempt to construct paths from user input. 
				Reset();
				return;
			}


			// Reject paths that contain two consecutive delimiters in any position 
			for (int32 i = 2; i < Path.size(); ++i) // Start by comparing index 2 and index 1 because index 0 is known to be '/'
			{
				if (Delimiters.Contains(Path[i]) && Delimiters.Contains(Path[i - 1]))
				{
					Reset();
					return;
				}
			}

			RC::StringViewType PackageNameView = FAsciiSet::FindPrefixWithout(Path, Delimiters);
			if (PackageNameView.size() == Path.size())
			{
				// No delimiter, package name only
				AssetPath = FTopLevelAssetPath(FName(RC::StringType(PackageNameView), FNAME_Add), FName());
				SubPathString.Empty();
				return;
			}

			RC::StringViewType FixedAssetName = Path.size() >= PackageNameView.size() + 1 ? Path.substr(PackageNameView.size() + 1, Path.size()) : TEXT("");
			check(FixedAssetName != "" && !Delimiters.Contains(FixedAssetName[0])); // Sanitized to avoid trailing delimiter or consecutive delimiters above

			RC::StringViewType AssetNameView = FAsciiSet::FindPrefixWithout(FixedAssetName, Delimiters);
			if (AssetNameView.size() == FixedAssetName.size())
			{
				// No subobject path
				AssetPath = FTopLevelAssetPath(FName(RC::StringType(PackageNameView), FNAME_Add), FName(AssetNameView, FNAME_Add));
				SubPathString.Empty();
				return;
			}

			auto FixedPackageName = FixedAssetName.size() >= AssetNameView.size() + 1 ? FixedAssetName.substr(0, AssetNameView.size() + 1) : TEXT("");
			check(!FixedAssetName != "" && !Delimiters.Contains(FixedAssetName[0])); // Sanitized to avoid trailing delimiter or consecutive delimiters above
			
			// Replace delimiters in subpath string with . to normalize
			AssetPath = FTopLevelAssetPath(FName(RC::StringType(PackageNameView), FNAME_Add), FName(AssetNameView, FNAME_Add));
			// SubPathString.ReplaceCharInline(SUBOBJECT_DELIMITER_CHAR, '.');
			SubPathString = FString(FixedPackageName.data());
		}
	}

	void FSoftObjectPath::SetPath(const FTopLevelAssetPath& InAssetPath, RC::Unreal::FString InSubPathString)
	{
		AssetPath = InAssetPath;
		SubPathString = MoveTemp(InSubPathString);
	}
}