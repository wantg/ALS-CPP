#include "ALS/AnimModifiers/CreateLayeringCurves.h"
#include "AnimationBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UCreateLayeringCurves::UCreateLayeringCurves() {
    CurvesToCreate = {"Enable_SpineRotation",
                      "Enable_HandIK_L",
                      "Enable_HandIK_R",
                      "HipOrientation_Bias",
                      "Layering_Arm_L",
                      "Layering_Arm_L_Add",
                      "Layering_Arm_L_LS",
                      "Layering_Arm_R",
                      "Layering_Arm_R_Add",
                      "Layering_Arm_R_LS",
                      "Layering_Hand_L",
                      "Layering_Hand_R",
                      "Layering_Head",
                      "Layering_Head_Add",
                      "Layering_Legs",
                      "Layering_Pelvis",
                      "Layering_Spine",
                      "Layering_Spine_Add"};
}

void UCreateLayeringCurves::OnApply_Implementation(UAnimSequence* AnimationSequence) {
    AnimSequence = AnimationSequence;

    for (FName& i : CurvesToCreate) {
        Curve = i;

        if (UAnimationBlueprintLibrary::DoesCurveExist(AnimSequence, Curve, ERawCurveTrackTypes::RCT_Float)) {
            UAnimationBlueprintLibrary::RemoveCurve(AnimSequence, Curve, false);
        }
        UAnimationBlueprintLibrary::AddCurve(AnimSequence, Curve, ERawCurveTrackTypes::RCT_Float, false);

        if (KeyEachFrame) {
            int32 NumFrames;
            UAnimationBlueprintLibrary::GetNumFrames(AnimSequence, NumFrames);
            for (int32 NumFrameIdx = 0; NumFrameIdx < NumFrames - 2 + 1; NumFrameIdx++) {
                float TimeAtFrame;
                UAnimationBlueprintLibrary::GetTimeAtFrame(AnimSequence, NumFrameIdx, TimeAtFrame);
                UAnimationBlueprintLibrary::AddFloatCurveKey(AnimSequence, Curve, TimeAtFrame, DefaultValue);
            }
        } else {
            float TimeAtFrame;
            UAnimationBlueprintLibrary::GetTimeAtFrame(AnimSequence, 0, TimeAtFrame);
            UAnimationBlueprintLibrary::AddFloatCurveKey(AnimSequence, Curve, TimeAtFrame, DefaultValue);
        }
    }
}