#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "CalculateRotationAmount.generated.h"

/**
 * This Anim Modifier calculates the change in root rotation and is used to create the "RotationAmount" curves for the turn in place animations.
 * Each curve value represents the change in rotation from the current and next frame, so it can be applied to the actors rotation to mimic root motion.
 */
UCLASS()
class ALS_API UCalculateRotationAmount : public UAnimationModifier {
    GENERATED_BODY()

    virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;

   public:
    UAnimSequence* AnimSequence;
    FName CurveName = "RotationAmount";
    FName RootBoneName = "root";
};
