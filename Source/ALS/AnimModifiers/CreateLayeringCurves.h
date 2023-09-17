#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "ALS/Data/Structs.h"
#include "CreateLayeringCurves.generated.h"

/**
 * This Anim Modifier automatically creates all Layering Curves
 * and is a great way to quickly add the necessary curves to the overlay animations.
 */
UCLASS()
class ALS_API UCreateLayeringCurves : public UAnimationModifier {
    GENERATED_BODY()

    UCreateLayeringCurves();

    virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;

   public:
    UAnimSequence* AnimSequence;
    FName Curve = "None";
    TArray<FName> CurvesToCreate;
    float DefaultValue = 1.f;
    bool KeyEachFrame = true;
};
