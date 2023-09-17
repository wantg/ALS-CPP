#include "ALS/AnimModifiers/CalculateRotationAmount.h"
#include "AnimationBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UCalculateRotationAmount::OnApply_Implementation(UAnimSequence* AnimationSequence) {
    AnimSequence = AnimationSequence;
    if (UAnimationBlueprintLibrary::DoesCurveExist(AnimSequence, CurveName, ERawCurveTrackTypes::RCT_Float)) {
        UAnimationBlueprintLibrary::RemoveCurve(AnimSequence, CurveName, false);
    }
    UAnimationBlueprintLibrary::AddCurve(AnimSequence, CurveName, ERawCurveTrackTypes::RCT_Float, false);

    int32 NumFrames;
    UAnimationBlueprintLibrary::GetNumFrames(AnimSequence, NumFrames);
    for (int32 i = 0; i < NumFrames + 1; i++) {
        float TimeAtFrame;
        UAnimationBlueprintLibrary::GetTimeAtFrame(AnimSequence, i, TimeAtFrame);

        FTransform CurrentPose;
        float RateScale;
        UAnimationBlueprintLibrary::GetRateScale(AnimSequence, RateScale);
        int32 Frame = i + (RateScale < 0.f ? -1 : 1);
        UAnimationBlueprintLibrary::GetBonePoseForFrame(AnimSequence, RootBoneName, Frame, false, CurrentPose);

        FTransform NextPose;
        UAnimationBlueprintLibrary::GetBonePoseForFrame(AnimSequence, RootBoneName, i, false, NextPose);

        float value = (CurrentPose.GetRotation().Z - NextPose.GetRotation().Z) * FMath::Abs(RateScale);

        UAnimationBlueprintLibrary::AddFloatCurveKey(AnimSequence, CurveName, TimeAtFrame, value);
    }

    UAnimationBlueprintLibrary::FinalizeBoneAnimation(AnimSequence);
}