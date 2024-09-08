

using UnrealBuildTool;
using System.Collections.Generic;

public class ALSEditorTarget : TargetRules
{
	public ALSEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.AddRange( new string[] { "ALS" } );
	}
}
