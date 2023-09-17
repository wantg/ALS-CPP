#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "CopyCurves.generated.h"

/**
 * This Anim Modifier simply copies curves from one animation to another.
 */
UCLASS()
class ALS_API UCopyCurves : public UAnimationModifier {
    GENERATED_BODY()

    UCopyCurves();

    virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;

   public:
    UAnimSequence* AnimSequence;
    UAnimSequence* AnimToCopyFrom;
    bool CopyAllCurves = false;
    TArray<FName> CurvesToCopy;
    FName CurveName = "None";
    TArray<FName> AllCurves;
};
