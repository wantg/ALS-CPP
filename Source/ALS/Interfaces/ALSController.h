#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ALS/Data/Enums.h"
#include "ALSController.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UALSController : public UInterface {
    GENERATED_BODY()
};

class ALS_API IALSController {
    GENERATED_BODY()

   public:
    UFUNCTION(BlueprintCallable, Category = "Debug")
    virtual void GetDebugInfo(ACharacter*& DebugFocusCharacter, bool& DebugView, bool& ShowHUD, bool& ShowTraces, bool& ShowDebugShapes, bool& ShowLayerColors, bool& Slomo, bool& ShowCharacterInfo) = 0;
};
