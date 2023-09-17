#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ALS/Data/Enums.h"
#include "ALSAnimation.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UALSAnimation : public UInterface {
    GENERATED_BODY()
};

class ALS_API IALSAnimation {
    GENERATED_BODY()

   public:
    virtual void Jumped() = 0;
    virtual void SetGroundedEntryState(EGroundedEntryState GroundedEntryState) = 0;
    virtual void SetOverlayOverrideState(int32 OverlayOverrideState) = 0;
};
