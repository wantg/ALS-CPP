#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "ALS/Data/Structs.h"
#include "CreateCurves.generated.h"

/**
 * This Anim Modifier simply creates the defined curves.
 * By selecting multiple assets in the content browser and applying a modifier by right clicking,
 * you can easily create a set of curves across multiple animations.
 */
UCLASS()
class ALS_API UCreateCurves : public UAnimationModifier {
    GENERATED_BODY()

    virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;

   public:
    UAnimSequence* AnimSequence;
    FAnimCurveCreationParams CurveParams;
    TArray<FAnimCurveCreationParams> CurvesToCreate;
};
