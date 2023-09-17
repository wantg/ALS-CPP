#include "ALS/AnimModifiers/CopyCurves.h"
#include "AnimationBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UCopyCurves::UCopyCurves() {
    AllCurves = {"BasePose_CLF",
                 "BasePose_N",
                 "Enable_AimOffset",
                 "Enable_FootIK_L",
                 "Enable_FootIK_R",
                 "Enable_HandIK_L",
                 "Enable_HandIK_R",
                 "Enable_SpineRotation",
                 "Enable_Transition",
                 "Feet_Crossing",
                 "FootLock_L",
                 "FootLock_R",
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
                 "Layering_Spine_Add",
                 "Mask_AimOffset",
                 "Mask_FootstepSound",
                 "Mask_LandPrediction",
                 "Mask_Lean",
                 "RotationAmount",
                 "Feet_Position",
                 "Weight_Falling",
                 "Weight_Gait",
                 "YawOffset"};
}

void UCopyCurves::OnApply_Implementation(UAnimSequence* AnimationSequence) {
    AnimSequence = AnimationSequence;
    for (FName& i : CopyAllCurves ? AllCurves : CurvesToCopy) {
        CurveName = i;

        if (UAnimationBlueprintLibrary::DoesCurveExist(AnimToCopyFrom, CurveName, ERawCurveTrackTypes::RCT_Float)) {
            if (UAnimationBlueprintLibrary::DoesCurveExist(AnimSequence, CurveName, ERawCurveTrackTypes::RCT_Float)) {
                UAnimationBlueprintLibrary::RemoveCurve(AnimSequence, CurveName, false);
            }
            UAnimationBlueprintLibrary::AddCurve(AnimSequence, CurveName, ERawCurveTrackTypes::RCT_Float, false);
            TArray<float> Times;
            TArray<float> Values;
            UAnimationBlueprintLibrary::GetFloatKeys(AnimToCopyFrom, CurveName, Times, Values);
            UAnimationBlueprintLibrary::AddFloatCurveKeys(AnimSequence, CurveName, Times, Values);
        }
    }
}