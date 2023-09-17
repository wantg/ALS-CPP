#include "ALS/AnimModifiers/CreateCurves.h"
#include "AnimationBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UCreateCurves::OnApply_Implementation(UAnimSequence* AnimationSequence) {
    AnimSequence = AnimationSequence;
    for (FAnimCurveCreationParams& i : CurvesToCreate) {
        CurveParams = i;

        if (UAnimationBlueprintLibrary::DoesCurveExist(AnimSequence, CurveParams.CurveName, ERawCurveTrackTypes::RCT_Float)) {
            UAnimationBlueprintLibrary::RemoveCurve(AnimSequence, CurveParams.CurveName, false);
        }
        UAnimationBlueprintLibrary::AddCurve(AnimSequence, CurveParams.CurveName, ERawCurveTrackTypes::RCT_Float, false);

        if (CurveParams.KeyEachFrame) {
            int32 NumFrames;
            UAnimationBlueprintLibrary::GetNumFrames(AnimSequence, NumFrames);
            for (int32 NumFrameIdx = 0; NumFrameIdx < NumFrames - 2 + 1; NumFrameIdx++) {
                float TimeAtFrame;
                UAnimationBlueprintLibrary::GetTimeAtFrame(AnimSequence, NumFrameIdx, TimeAtFrame);
                UAnimationBlueprintLibrary::AddFloatCurveKey(AnimSequence, CurveParams.CurveName, TimeAtFrame, 0.f);
            }
        } else {
            for (FAnimCurveCreationData& AnimCurveCreationData : CurveParams.Keys) {
                float TimeAtFrame;
                UAnimationBlueprintLibrary::GetTimeAtFrame(AnimSequence, AnimCurveCreationData.FrameNumber, TimeAtFrame);
                UAnimationBlueprintLibrary::AddFloatCurveKey(AnimSequence, CurveParams.CurveName, TimeAtFrame, AnimCurveCreationData.CurveValue);
            }
        }
    }
}