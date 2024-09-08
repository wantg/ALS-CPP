#include "ALS/Animations/ALSAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "ALS/Interfaces/ALSCharacter.h"
#include "ALS/Interfaces/ALSController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "KismetAnimationLibrary.h"

UALSAnimInstance::UALSAnimInstance() {
    // ================================================================================
    // Turn In Place
    N_TurnIP_L90.Animation      = LoadObject<UAnimSequence>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/TurnInPlace/ALS_N_TurnIP_L90"));
    N_TurnIP_L90.AnimatedAngle  = -90.f;
    N_TurnIP_L90.SlotName       = "(N) Turn/Rotate";
    N_TurnIP_L90.PlayRate       = 1.2f;
    N_TurnIP_L90.ScaleTurnAngle = true;

    N_TurnIP_R90.Animation      = LoadObject<UAnimSequence>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/TurnInPlace/ALS_N_TurnIP_R90"));
    N_TurnIP_R90.AnimatedAngle  = 90.f;
    N_TurnIP_R90.SlotName       = "(N) Turn/Rotate";
    N_TurnIP_R90.PlayRate       = 1.2f;
    N_TurnIP_R90.ScaleTurnAngle = true;

    N_TurnIP_L180.Animation      = LoadObject<UAnimSequence>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/TurnInPlace/ALS_N_TurnIP_L180"));
    N_TurnIP_L180.AnimatedAngle  = -180.f;
    N_TurnIP_L180.SlotName       = "(N) Turn/Rotate";
    N_TurnIP_L180.PlayRate       = 1.2f;
    N_TurnIP_L180.ScaleTurnAngle = true;

    N_TurnIP_R180.Animation      = LoadObject<UAnimSequence>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/TurnInPlace/ALS_N_TurnIP_R180"));
    N_TurnIP_R180.AnimatedAngle  = 180.f;
    N_TurnIP_R180.SlotName       = "(N) Turn/Rotate";
    N_TurnIP_R180.PlayRate       = 1.2f;
    N_TurnIP_R180.ScaleTurnAngle = true;

    CLF_TurnIP_L90.Animation      = LoadObject<UAnimSequence>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/TurnInPlace/ALS_CLF_TurnIP_L90"));
    CLF_TurnIP_L90.AnimatedAngle  = -90.f;
    CLF_TurnIP_L90.SlotName       = "(CLF) Turn/Rotate";
    CLF_TurnIP_L90.PlayRate       = 1.2f;
    CLF_TurnIP_L90.ScaleTurnAngle = false;

    CLF_TurnIP_R90.Animation      = LoadObject<UAnimSequence>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/TurnInPlace/ALS_CLF_TurnIP_R90"));
    CLF_TurnIP_R90.AnimatedAngle  = 90.f;
    CLF_TurnIP_R90.SlotName       = "(CLF) Turn/Rotate";
    CLF_TurnIP_R90.PlayRate       = 1.2f;
    CLF_TurnIP_R90.ScaleTurnAngle = false;

    CLF_TurnIP_L180.Animation      = LoadObject<UAnimSequence>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/TurnInPlace/ALS_CLF_TurnIP_L180"));
    CLF_TurnIP_L180.AnimatedAngle  = -180.f;
    CLF_TurnIP_L180.SlotName       = "(CLF) Turn/Rotate";
    CLF_TurnIP_L180.PlayRate       = 1.2f;
    CLF_TurnIP_L180.ScaleTurnAngle = false;

    CLF_TurnIP_R180.Animation      = LoadObject<UAnimSequence>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/TurnInPlace/ALS_CLF_TurnIP_R180"));
    CLF_TurnIP_R180.AnimatedAngle  = 180.f;
    CLF_TurnIP_R180.SlotName       = "(CLF) Turn/Rotate";
    CLF_TurnIP_R180.PlayRate       = 1.2f;
    CLF_TurnIP_R180.ScaleTurnAngle = false;

    // ================================================================================
    // Blend Curves
    DiagonalScaleAmountCurve /* */ = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/AnimationBlendCurves/DiagonalScaleAmount"));
    StrideBlend_N_Walk /*       */ = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/AnimationBlendCurves/StrideBlend_N_Walk"));
    StrideBlend_N_Run /*        */ = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/AnimationBlendCurves/StrideBlend_N_Run"));
    StrideBlend_C_Walk /*       */ = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/AnimationBlendCurves/StrideBlend_N_Walk"));
    LandPredictionCurve /*      */ = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/AnimationBlendCurves/LandPredictionBlend"));
    LeanInAirCurve /*           */ = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/AnimationBlendCurves/LeanInAirAmount"));
    YawOffset_FB /*             */ = LoadObject<UCurveVector>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/AnimationBlendCurves/YawOffset_FB"));
    YawOffset_LR /*             */ = LoadObject<UCurveVector>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/AnimationBlendCurves/YawOffset_LR"));
}

/*
 * Initiaize Anim Instance
 */
void UALSAnimInstance::NativeInitializeAnimation() {
    Super::NativeInitializeAnimation();
    if (TryGetPawnOwner()) {
        Character = Cast<ACharacter>(TryGetPawnOwner());
    }
}

void UALSAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
    Super::NativeUpdateAnimation(DeltaSeconds);
    // Only update if character is valid
    DeltaTimeX = DeltaSeconds;
    if (DeltaTimeX != 0.f && Character) {
        // Do Every Frame
        UpdateCharacterInfo();
        UpdateAimingValues();
        UpdateLayerValues();
        UpdateFootIK();
        // Check Movement Mode
        bool PreviousShouldMove = ShouldMove;
        switch (MovementState) {
        case EALSMovementState::EAMS_Grounded:
            // Check If Moving Or Not
            ShouldMove = ShouldMoveCheck();
            // Do While Moving
            if (ShouldMove) {
                UpdateMovementValues();
                UpdateRotationValues();
            }
            // Do While Not Moving
            if (!ShouldMove) {
                if (CanRotateInPlace()) {
                    RotateInPlaceCheck();
                } else {
                    RotateL = false;
                    RotateR = false;
                }
                if (CanTurnInPlace()) {
                    TurnInPlaceCheck();
                } else {
                    ElapsedDelayTime = 0.f;
                }
                if (CanDynamicTransition()) {
                    DynamicTransitionCheck();
                }
            }
            // Do When Starting To Move
            if (PreviousShouldMove == false && ShouldMove) {
                ElapsedDelayTime = 0.f;
                RotateL          = false;
                RotateR          = false;
            }
            break;
        case EALSMovementState::EAMS_InAir:
            // Do While InAir
            UpdateInAirValues();
            break;
        case EALSMovementState::EAMS_Ragdoll:
            // Do While Ragdolling
            UpdateRagdollValues();
            break;
        }
    }
}

/*
 * Play Additive Transition Animation
 */
void UALSAnimInstance::PlayTransition(FDynamicMontageParams MontageParams) {
    PlaySlotAnimationAsDynamicMontage(
        MontageParams.Animation,    // UAnimSequenceBase* Asset
        FName("Grounded Slot"),     // FName SlotNodeName
        MontageParams.BlendInTime,  // float BlendInTime = 0.25f
        MontageParams.BlendOutTime, // float BlendOutTime = 0.25f
        MontageParams.PlayRate,     // float InPlayRate = 1.f
        1,                          // int32 LoopCount = 1
        0.f,                        // float BlendOutTriggerTime = -1.f
        MontageParams.StartTime     // float InTimeToStartMontageAt = 0.f
    );
}

/*
 * Play Dynamic Additive Transition Animation
 */
void UALSAnimInstance::PlayDynamicTransition(float ReTriggerDelay, FDynamicMontageParams MontageParams) {
    if (PlayDynamicTransitionEnabled) {
        PlaySlotAnimationAsDynamicMontage(
            MontageParams.Animation,    // UAnimSequenceBase* Asset
            FName("Grounded Slot"),     // FName SlotNodeName
            MontageParams.BlendInTime,  // float BlendInTime = 0.25f
            MontageParams.BlendOutTime, // float BlendOutTime = 0.25f
            MontageParams.PlayRate,     // float InPlayRate = 1.f
            1,                          // int32 LoopCount = 1
            0.f,                        // float BlendOutTriggerTime = -1.f
            MontageParams.StartTime     // float InTimeToStartMontageAt = 0.f
        );
        PlayDynamicTransitionEnabled = false;
        AddTimer([this]() { PlayDynamicTransitionEnabled = true; }, ReTriggerDelay, false);
    }
}

// ================================================================================
// Update Values
/*
 * Get Information from the Character via the Character Interface to use throughout the AnimBP and AnimGraph.
 */
void UALSAnimInstance::UpdateCharacterInfo() {
    if (IALSCharacter* ALSCharacter = Cast<IALSCharacter>(Character)) {
        ALSCharacter->GetEssentialValues(Velocity, Acceleration, MovementInput, IsMoving, HasMovementInput, Speed, MovementInputAmount, AimingRotation, AimYawRate);
        TEnumAsByte<EMovementMode> PawnMovementMode;
        ALSCharacter->GetCurrentStates(PawnMovementMode, MovementState, PrevMovementState, MovementAction, RotationMode, Gait, Stance, ViewMode, OverlayState);
    }
}

void UALSAnimInstance::UpdateAimingValues() {
    FRotator l_Rotation;

    // Interp the Aiming Rotation value to achieve smooth aiming rotation changes.
    // Interpolating the rotation before calculating the angle ensures the value is not affected by changes in actor rotation,
    // allowing slow aiming rotation changes with fast actor rotation changes.
    SmoothedAimingRotation = UKismetMathLibrary::RInterpTo(SmoothedAimingRotation, AimingRotation, DeltaTimeX, SmoothedAimingRotationInterpSpeed);

    // Calculate the Aiming angle and Smoothed Aiming Angle by getting the delta between the aiming rotation and the actor rotation.
    l_Rotation          = UKismetMathLibrary::NormalizedDeltaRotator(AimingRotation, Character->GetActorRotation());
    AimingAngle         = FVector2D(l_Rotation.Yaw, l_Rotation.Pitch);
    l_Rotation          = UKismetMathLibrary::NormalizedDeltaRotator(SmoothedAimingRotation, Character->GetActorRotation());
    SmoothedAimingAngle = FVector2D(l_Rotation.Yaw, l_Rotation.Pitch);

    // Clamp the Aiming Pitch Angle to a range of 1 to 0 for use in the vertical aim sweeps.
    if (RotationMode == EALSRotationMode::EARM_LookingDirection || RotationMode == EALSRotationMode::EARM_Aiming) {
        AimSweepTime = UKismetMathLibrary::MapRangeClamped(AimingAngle.Y, -90.f, 90.f, 1.f, 0.f);
        // Use the Aiming Yaw Angle divided by the number of spine+pelvis bones
        // to get the amount of spine rotation needed to remain facing the camera direction.
        SpineRotation.Yaw = AimingAngle.X / 4.f;
    }

    // Get the delta between the Movement Input rotation and Actor rotation and map it to a range of 0-1.
    // This value is used in the aim offset behavior to make the character look toward the Movement Input.
    if (RotationMode == EALSRotationMode::EARM_VelocityDirection) {
        if (HasMovementInput) {
            l_Rotation = UKismetMathLibrary::NormalizedDeltaRotator(
                MovementInput.ToOrientationRotator(), Character->GetActorRotation());
            InputYawOffsetTime = UKismetMathLibrary::FInterpTo(
                InputYawOffsetTime,
                UKismetMathLibrary::MapRangeClamped(l_Rotation.Yaw, -180.f, 180.f, 0.f, 1.f),
                DeltaTimeX,
                InputYawOffsetInterpSpeed);
        }
    }

    // Separate the Aiming Yaw Angle into 3 separate Yaw Times.
    // These 3 values are used in the Aim Offset behavior
    // to improve the blending of the aim offset when rotating completely around the character.
    // This allows you to keep the aiming responsive but still smoothly blend from left to right or right to left.
    float SmoothedAimingAngleX = FMath::Abs(SmoothedAimingAngle.X);
    LeftYawTime                = UKismetMathLibrary::MapRangeClamped(SmoothedAimingAngleX, 0.f, 180.f, 0.5f, 0.f);
    RightYawTime               = UKismetMathLibrary::MapRangeClamped(SmoothedAimingAngleX, 0.f, 180.f, 0.5f, 1.f);
    ForwardYawTime             = UKismetMathLibrary::MapRangeClamped(SmoothedAimingAngle.X, -180.f, 180.f, 0.f, 1.f);
}

void UALSAnimInstance::UpdateLayerValues() {
    // Get the Aim Offset weight by getting the opposite of the Aim Offset Mask.
    EnableAimOffset = UKismetMathLibrary::Lerp(1.f, 0.f, GetCurveValue(FName("Mask_AimOffset")));

    // Set the Base Pose weights
    BasePose_N   = GetCurveValue(FName("BasePose_N"));
    BasePose_CLF = GetCurveValue(FName("BasePose_CLF"));

    // Set the Additive amount weights for each body part
    SpineAdd  = GetCurveValue(FName("Layering_Spine_Add"));
    HeadAdd   = GetCurveValue(FName("Layering_Head_Add"));
    Arm_L_Add = GetCurveValue(FName("Layering_Arm_L_Add"));
    Arm_R_Add = GetCurveValue(FName("Layering_Arm_R_Add"));

    // Set the Hand Override weights
    Hand_R = GetCurveValue(FName("Layering_Hand_R"));
    Hand_L = GetCurveValue(FName("Layering_Hand_L"));

    // Blend and set the Hand IK weights to ensure they only are weighted if allowed by the Arm layers.
    EnableHandIK_L = UKismetMathLibrary::Lerp(0.f, GetCurveValue(FName("Enable_HandIK_L")), GetCurveValue(FName("Layering_Arm_L")));
    EnableHandIK_R = UKismetMathLibrary::Lerp(0.f, GetCurveValue(FName("Enable_HandIK_R")), GetCurveValue(FName("Layering_Arm_R")));

    // Set whether the arms should blend in mesh space or local space.
    // The Mesh space weight will always be 1 unless the Local Space (LS) curve is fully weighted.
    Arm_L_LS = GetCurveValue(FName("Layering_Arm_L_LS"));
    Arm_R_LS = GetCurveValue(FName("Layering_Arm_R_LS"));
    Arm_L_MS = (float)(1 - FMath::FloorToInt(Arm_L_LS));
    Arm_R_MS = (float)(1 - FMath::FloorToInt(Arm_R_LS));
}

void UALSAnimInstance::UpdateFootIK() {
    // Update Foot Locking values.
    SetFootLocking(FName("Enable_FootIK_L"), FName("FootLock_L"), FName("ik_foot_l"), FootLock_L_Alpha, FootLock_L_Location, FootLock_L_Rotation);
    SetFootLocking(FName("Enable_FootIK_R"), FName("FootLock_R"), FName("ik_foot_r"), FootLock_R_Alpha, FootLock_R_Location, FootLock_R_Rotation);

    FVector FootOffset_L_Target = FVector::ZeroVector;
    FVector FootOffset_R_Target = FVector::ZeroVector;
    switch (MovementState) {
    case EALSMovementState::EAMS_None:
    case EALSMovementState::EAMS_Grounded:
    case EALSMovementState::EAMS_Mantling:
        // Update all Foot Lock and Foot Offset values when not In Air
        SetFootOffsets(FName("Enable_FootIK_L"), FName("ik_foot_l"), FName("root"), FootOffset_L_Target, FootOffset_L_Location, FootOffset_L_Rotation);
        SetFootOffsets(FName("Enable_FootIK_R"), FName("ik_foot_r"), FName("root"), FootOffset_R_Target, FootOffset_R_Location, FootOffset_R_Rotation);
        SetPelvisIKOffset(FootOffset_L_Target, FootOffset_R_Target);
        break;
    case EALSMovementState::EAMS_InAir:
        // Reset IK Offsets if In Air
        SetPelvisIKOffset(FVector::ZeroVector, FVector::ZeroVector);
        ResetIKOffsets();
        break;
    }
}

void UALSAnimInstance::UpdateMovementValues() {
    // Interp and set the Velocity Blend.
    VelocityBlend = InterpVelocityBlend(VelocityBlend, CalculateVelocityBlend(), VelocityBlendInterpSpeed, DeltaTimeX);

    // Set the Diagnal Scale Amount.
    DiagonalScaleAmount = CalculateDiagonalScaleAmount();

    // Set the Relative Acceleration Amount and Interp the Lean Amount.
    RelativeAccelerationAmount = CalculateRelativeAccelerationAmount();
    FLeanAmount l_LeanAmount{RelativeAccelerationAmount.Y, RelativeAccelerationAmount.X};
    LeanAmount = InterpLeanAmount(LeanAmount, l_LeanAmount, GroundedLeanInterpSpeed, DeltaTimeX);

    // Set the Walk Run Blend
    WalkRunBlend = CalculateWalkRunBlend();
    // Set the Stride Blend
    StrideBlend = CalculateStrideBlend();
    // Set the Standing and Crouching Play Rates
    StandingPlayRate  = CalculateStandingPlayRate();
    CrouchingPlayRate = CalculateCrouchingPlayRate();
}

void UALSAnimInstance::UpdateRotationValues() {
    // Set the Movement Direction
    MovementDirection = CalculateMovementDirection();

    // Set the Yaw Offsets.
    // These values influence the "YawOffset" curve in the animgraph and are used to offset the characters rotation for more natural movement.
    // The curves allow for fine control over how the offset behaves for each movement direction.
    float l_Yaw           = UKismetMathLibrary::NormalizedDeltaRotator(Velocity.ToOrientationRotator(), Character->GetControlRotation()).Yaw;
    FVector l_YawOffsetFB = YawOffset_FB->GetVectorValue(l_Yaw);
    FVector l_YawOffsetLR = YawOffset_LR->GetVectorValue(l_Yaw);
    FYaw                  = l_YawOffsetFB.X;
    BYaw                  = l_YawOffsetFB.Y;
    LYaw                  = l_YawOffsetLR.X;
    RYaw                  = l_YawOffsetLR.Y;
}

void UALSAnimInstance::UpdateInAirValues() {
    // Update the fall speed.
    // Setting this value only while in the air allows you to use it within the AnimGraph for the landing strength.
    // If not, the Z velocity would return to 0 on landing.
    FallSpeed = Velocity.Z;

    // Set the Land Prediction weight.
    LandPrediction = CalculateLandPrediction();

    // Interp and set the In Air Lean Amount
    FLeanAmount l_LeanAmount = CalculateInAirLeanAmount();
    LeanAmount               = InterpLeanAmount(LeanAmount, l_LeanAmount, InAirLeanInterpSpeed, DeltaTimeX);
}

/*
 * Scale the Flail Rate by the velocity length.
 * The faster the ragdoll moves, the faster the character will flail.
 */
void UALSAnimInstance::UpdateRagdollValues() {
    float l_V = (GetOwningComponent()->GetPhysicsLinearVelocity(FName("root"))).Size();
    FlailRate = UKismetMathLibrary::MapRangeClamped(l_V, 0.f, 1000.f, 0.f, 1.f);
}

// ================================================================================
// Grounded

/*
 * Enable Movement Animations if IsMoving and HasMovementInput, or if the Speed is greater than 150.
 */
bool UALSAnimInstance::ShouldMoveCheck() {
    return (IsMoving && HasMovementInput) || Speed > 150.f;
}

/*
 * Only perform a Turn In Place check if the character is looking toward the camera in Third Person,
 * and if the "Enable Transition" curve is fully weighted.
 * The Enable_Transition curve is modified within certain states of the AnimBP
 * so that the character can only turn while in those states.
 */
bool UALSAnimInstance::CanTurnInPlace() {
    return RotationMode == EALSRotationMode::EARM_LookingDirection &&
           ViewMode == EALSViewMode::EAVM_ThirdPerson &&
           GetCurveValue(FName("Enable_Transition")) > 0.99f;
}

/*
 * Only perform a Rotate In Place Check if the character is Aiming or in First Person.
 */
bool UALSAnimInstance::CanRotateInPlace() {
    return RotationMode == EALSRotationMode::EARM_Aiming || ViewMode == EALSViewMode::EAVM_FirstPerson;
}

/*
 * Only perform a Dynamic Transition check if the "Enable Transition" curve is fully weighted.
 * The Enable_Transition curve is modified within certain states of the AnimBP
 * so that the character can only transition while in those states.
 */
bool UALSAnimInstance::CanDynamicTransition() {
    return GetCurveValue(FName("Enable_Transition")) == 1.f;
}

bool UALSAnimInstance::CanOverlayTransition() {
    return Stance == EALSStance::EAS_Standing && !ShouldMove;
}

void UALSAnimInstance::TurnInPlace(FRotator TargetRotation, float PlayRateScale, float StartTime, bool OverrideCurrent) {
    // Step 1: Set Turn Angle
    float TurnAngle = UKismetMathLibrary::NormalizedDeltaRotator(TargetRotation, Character->GetActorRotation()).Yaw;

    // Step 2: Choose Turn Asset based on the Turn Angle and Stance
    FTurnInPlaceAsset TargetTurnAsset;
    if (FMath::Abs(TurnAngle) < Turn180Threshold) {
        if (TurnAngle < 0.f) {
            if (Stance == EALSStance::EAS_Standing) {
                TargetTurnAsset = N_TurnIP_L90;
            } else if (Stance == EALSStance::EAS_Crouching) {
                TargetTurnAsset = CLF_TurnIP_L90;
            }
        } else {
            if (Stance == EALSStance::EAS_Standing) {
                TargetTurnAsset = N_TurnIP_R90;
            } else if (Stance == EALSStance::EAS_Crouching) {
                TargetTurnAsset = CLF_TurnIP_R90;
            }
        }
    } else {
        if (TurnAngle < 0.f) {
            if (Stance == EALSStance::EAS_Standing) {
                TargetTurnAsset = N_TurnIP_L180;
            } else if (Stance == EALSStance::EAS_Crouching) {
                TargetTurnAsset = CLF_TurnIP_L180;
            }
        } else {
            if (Stance == EALSStance::EAS_Standing) {
                TargetTurnAsset = N_TurnIP_R180;
            } else if (Stance == EALSStance::EAS_Crouching) {
                TargetTurnAsset = CLF_TurnIP_R180;
            }
        }
    }

    // Step 3: If the Target Turn Animation is not playing or set to be overriden,
    // play the turn animation as a dynamic montage.
    if (OverrideCurrent || !IsPlayingSlotAnimation(TargetTurnAsset.Animation, TargetTurnAsset.SlotName)) {
        PlaySlotAnimationAsDynamicMontage(
            TargetTurnAsset.Animation, TargetTurnAsset.SlotName, 0.2f, 0.2f,
            TargetTurnAsset.PlayRate * PlayRateScale, 1, 0.f, StartTime);
        // Step 4: Scale the rotation amount (gets scaled in animgraph) to compensate for turn angle (If Allowed) and play rate.
        if (TargetTurnAsset.ScaleTurnAngle) {
            RotationScale = TurnAngle / TargetTurnAsset.AnimatedAngle * TargetTurnAsset.PlayRate * PlayRateScale;
        } else {
            RotationScale = TargetTurnAsset.PlayRate * PlayRateScale;
        }
    }
}

void UALSAnimInstance::TurnInPlaceCheck() {
    // Step 1: Check if Aiming angle is outside of the Turn Check Min Angle,
    // and if the Aim Yaw Rate is below the Aim Yaw Rate Limit.
    // If so, begin counting the Elapsed Delay Time.
    // If not, reset the Elapsed Delay Time.
    // This ensures the conditions remain true for a sustained peroid of time before turning in place.
    if (FMath::Abs(AimingAngle.X) > TurnCheckMinAngle && AimYawRate < AimYawRateLimit) {
        ElapsedDelayTime = ElapsedDelayTime + DeltaTimeX;

        // Step 2: Check if the Elapsed Delay time exceeds the set delay (mapped to the turn angle range).
        // If so, trigger a Turn In Place.
        if (ElapsedDelayTime > UKismetMathLibrary::MapRangeClamped(
                                   FMath::Abs(AimingAngle.X),
                                   TurnCheckMinAngle, 180.f, MinAngleDelay, MaxAngleDelay)) {
            TurnInPlace(FRotator{0.f, AimingRotation.Yaw, 0.f}, 1.f, 0.f, false);
        }
    } else {
        ElapsedDelayTime = 0.f;
    }
}

void UALSAnimInstance::RotateInPlaceCheck() {
    // Step 1: Check if the character should rotate left or right
    // by checking if the Aiming Angle exceeds the threshold.
    RotateL = AimingAngle.X < RotateMinThreshold;
    RotateR = AimingAngle.X > RotateMaxThreshold;

    // Step 2: If the character should be rotating,
    // set the Rotate Rate to scale with the Aim Yaw Rate.
    // This makes the character rotate faster when moving the camera faster.
    if (RotateL || RotateR) {
        RotateRate = UKismetMathLibrary::MapRangeClamped(
            AimYawRate, AimYawRateMinRange, AimYawRateMaxRange,
            MinPlayRate, MaxPlayRate);
    }
}

/*
 * Check each foot to see if the location difference
 * between the IK_Foot bone and its desired / target location (determined via a virtual bone) exceeds a threshold.
 * If it does, play an additive transition animation on that foot.
 * The currently set transition plays the second half of a 2 foot transition animation,
 * so that only a single foot moves. Because only the IK_Foot bone can be locked,
 * the separate virtual bone allows the system to know its desired location when locked.
 */
void UALSAnimInstance::DynamicTransitionCheck() {
    if (UKismetAnimationLibrary::K2_DistanceBetweenTwoSocketsAndMapRange(
            GetOwningComponent(),                   // const USkeletalMeshComponent* Component
            FName("ik_foot_l"),                     // const FName SocketOrBoneNameA
            ERelativeTransformSpace::RTS_Component, // ERelativeTransformSpace SocketSpaceA
            FName("VB foot_target_l"),              // const FName SocketOrBoneNameB
            ERelativeTransformSpace::RTS_Component, // ERelativeTransformSpace SocketSpaceB
            false,                                  // bool bRemapRange
            0.f,                                    // float InRangeMin
            0.f,                                    // float InRangeMax
            0.f,                                    // float OutRangeMin
            0.f                                     // float OutRangeMax
            ) > 8.f) {
        UAnimSequence* AnimMontage = LoadObject<UAnimSequence>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Transition_R"));
        PlayDynamicTransition(0.1f, FDynamicMontageParams{AnimMontage, 0.2f, 0.2f, 1.5f, 0.8f});
    }
    if (UKismetAnimationLibrary::K2_DistanceBetweenTwoSocketsAndMapRange(
            GetOwningComponent(),                   // const USkeletalMeshComponent* Component
            FName("ik_foot_r"),                     // const FName SocketOrBoneNameA
            ERelativeTransformSpace::RTS_Component, // ERelativeTransformSpace SocketSpaceA
            FName("VB foot_target_r"),              // const FName SocketOrBoneNameB
            ERelativeTransformSpace::RTS_Component, // ERelativeTransformSpace SocketSpaceB
            false,                                  // bool bRemapRange
            0.f,                                    // float InRangeMin
            0.f,                                    // float InRangeMax
            0.f,                                    // float OutRangeMin
            0.f                                     // float OutRangeMax
            ) > 8.f) {
        UAnimSequence* AnimMontage = LoadObject<UAnimSequence>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Transition_L"));
        PlayDynamicTransition(0.1f, FDynamicMontageParams{AnimMontage, 0.2f, 0.2f, 1.5f, 0.8f});
    }
}

// ================================================================================
// Movement

/*
 * Calculate the Velocity Blend.
 * This value represents the velocity amount of the actor in each direction (normalized so that diagonals equal .5 for each direction),
 * and is used in a BlendMulti node to produce better directional blending than a standard blendspace.
 */
FVelocityBlend UALSAnimInstance::CalculateVelocityBlend() {
    FVector LocRelativeVelocityDir =
        UKismetMathLibrary::LessLess_VectorRotator(
            UKismetMathLibrary::Normal(Velocity, 0.1f), Character->GetActorRotation());

    float Sum = FMath::Abs(LocRelativeVelocityDir.X) +
                FMath::Abs(LocRelativeVelocityDir.Y) +
                FMath::Abs(LocRelativeVelocityDir.Z);

    FVector RelativeDirection = LocRelativeVelocityDir / Sum;

    return FVelocityBlend{
        (float)UKismetMathLibrary::FClamp(RelativeDirection.X, 0.f, 1.f),
        (float)FMath::Abs(UKismetMathLibrary::FClamp(RelativeDirection.X, -1.f, 0.f)),
        (float)FMath::Abs(UKismetMathLibrary::FClamp(RelativeDirection.Y, -1.f, 0.f)),
        (float)UKismetMathLibrary::FClamp(RelativeDirection.Y, 0.f, 1.f)};
}

/*
 * Calculate the Diagnal Scale Amount.
 * This value is used to scale the Foot IK Root bone to make the Foot IK bones cover more distance on the diagonal blends.
 * Without scaling, the feet would not move far enough on the diagonal direction due to the linear translational blending of the IK bones.
 * The curve is used to easily map the value.
 */
float UALSAnimInstance::CalculateDiagonalScaleAmount() {
    return DiagonalScaleAmountCurve->GetFloatValue(
        FMath::Abs(VelocityBlend.F + VelocityBlend.B));
}

/*
 * Calculate the Relative Acceleration Amount.
 * This value represents the current amount of acceleration / deceleration relative to the actor rotation.
 * It is normalized to a range of -1 to 1 so that -1 equals the Max Braking Deceleration,
 * and 1 equals the Max Acceleration of the Character Movement Component.
 */
FVector UALSAnimInstance::CalculateRelativeAccelerationAmount() {
    if (UKismetMathLibrary::Dot_VectorVector(Acceleration, Velocity) > 0.f) {
        float MaxAcceleration = Character->GetCharacterMovement()->GetMaxAcceleration();
        return UKismetMathLibrary::LessLess_VectorRotator(
            UKismetMathLibrary::Vector_ClampSizeMax(Acceleration, MaxAcceleration) / MaxAcceleration,
            Character->GetActorRotation());
    } else {
        float MaxBrakingDeceleration = Character->GetCharacterMovement()->GetMaxBrakingDeceleration();
        return UKismetMathLibrary::LessLess_VectorRotator(
            UKismetMathLibrary::Vector_ClampSizeMax(Acceleration, MaxBrakingDeceleration) / MaxBrakingDeceleration,
            Character->GetActorRotation());
    }
}

/*
 * Calculate the Walk Run Blend.
 * This value is used within the Blendspaces to blend between walking and running.
 */
float UALSAnimInstance::CalculateWalkRunBlend() {
    return Gait == EALSGait::EAG_Walking ? 0.f : 1.f;
}

/*
 * Calculate the Stride Blend.
 * This value is used within the blendspaces to scale the stride (distance feet travel)
 * so that the character can walk or run at different movement speeds.
 * It also allows the walk or run gait animations to blend independently
 * while still matching the animation speed to the movement speed,
 * preventing the character from needing to play a half walk+half run blend.
 * The curves are used to map the stride amount to the speed for maximum control.
 */
float UALSAnimInstance::CalculateStrideBlend() {
    float l_V = UKismetMathLibrary::Lerp(
        StrideBlend_N_Walk->GetFloatValue(Speed),
        StrideBlend_N_Run->GetFloatValue(Speed),
        UKismetMathLibrary::FClamp(GetCurveValue("Weight_Gait") - 1.f, 0.f, 1.f));
    return UKismetMathLibrary::Lerp(l_V, StrideBlend_C_Walk->GetFloatValue(Speed), GetCurveValue(FName("BasePose_CLF")));
}

/*
 * Calculate the Play Rate by dividing the Character's speed by the Animated Speed for each gait.
 * The lerps are determined by the "Weight_Gait" anim curve that exists on every locomotion cycle
 * so that the play rate is always in sync with the currently blended animation.
 * The value is also divided by the Stride Blend and the mesh scale
 * so that the play rate increases as the stride or scale gets smaller.
 */
float UALSAnimInstance::CalculateStandingPlayRate() {
    float l_V = UKismetMathLibrary::Lerp(
        Speed / AnimatedWalkSpeed, Speed / AnimatedRunSpeed,
        UKismetMathLibrary::FClamp(GetCurveValue("Weight_Gait") - 1.f, 0.f, 1.f));

    l_V = UKismetMathLibrary::Lerp(
        l_V, Speed / AnimatedSprintSpeed,
        UKismetMathLibrary::FClamp(GetCurveValue("Weight_Gait") - 2.f, 0.f, 1.f));

    return UKismetMathLibrary::FClamp(
        l_V / StrideBlend / GetOwningComponent()->GetComponentScale().Z, 0.f, 3.f);
}

/*
 * Calculate the Crouching Play Rate by dividing the Character's speed by the Animated Speed.
 * This value needs to be separate from the standing play rate to improve the blend from crocuh to stand while in motion.
 */
float UALSAnimInstance::CalculateCrouchingPlayRate() {
    return UKismetMathLibrary::FClamp(
        Speed / AnimatedCrouchSpeed / StrideBlend / GetOwningComponent()->GetComponentScale().Z, 0.f, 2.f);
}

// ================================================================================
// In Air

/*
 * Calculate the land prediction weight by tracing in the velocity direction to find a walkable surface the character is falling toward,
 * and getting the 'Time' (range of 0-1, 1 being maximum, 0 being about to land) till impact.
 * The Land Prediction Curve is used to control how the time affects the final weight for a smooth blend.
 */
float UALSAnimInstance::CalculateLandPrediction() {
    if (FallSpeed < 200.f) {
        UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
        FVector Start                       = CapsuleComponent->GetComponentLocation();
        FVector l_Vector                    = UKismetMathLibrary::Vector_NormalUnsafe(
            FVector{Velocity.X, Velocity.Y, UKismetMathLibrary::FClamp(Velocity.Z, -4000.f, -200.f)});
        FVector End = Start + l_Vector * UKismetMathLibrary::MapRangeClamped(Velocity.Z, 0.f, -4000.f, 50.f, 2000.f);
        const TArray<AActor*> ActorsToIgnore;
        FHitResult OutHit;
        UKismetSystemLibrary::CapsuleTraceSingleByProfile(
            this,                                                  // const UObject* WorldContextObject
            Start,                                                 // const FVector Start
            End,                                                   // const FVector End
            CapsuleComponent->GetUnscaledCapsuleRadius(),          // float Radius
            CapsuleComponent->GetUnscaledCapsuleHalfHeight(),      // float HalfHeight
            FName("ALS_Character"),                                // FName ProfileName
            false,                                                 // bool bTraceComplex
            ActorsToIgnore,                                        // const TArray<AActor*>& ActorsToIgnore
            GetDebugTraceType(EDrawDebugTrace::Type::ForOneFrame), // EDrawDebugTrace::Type DrawDebugType
            OutHit,                                                // FHitResult& OutHit
            true,                                                  // bool bIgnoreSelf
            FLinearColor::Red,                                     // FLinearColor TraceColor = FLinearColor::Red
            FLinearColor::Green,                                   // FLinearColor TraceHitColor = FLinearColor::Green
            5.f                                                    // float DrawTime = 5.0f
        );
        if (Character->GetCharacterMovement()->IsWalkable(OutHit) && OutHit.bBlockingHit) {
            return UKismetMathLibrary::Lerp(
                LandPredictionCurve->GetFloatValue(OutHit.Time), 0.f, GetCurveValue("Mask_LandPrediction"));
        }
    }
    return 0.f;
}

/*
 * Use the relative Velocity direction and amount to determine how much the character should lean while in air.
 * The Lean In Air curve gets the Fall Speed and is used as a multiplier
 * to smoothly reverse the leaning direction when transitioning from moving upwards to moving downwards.
 */
FLeanAmount UALSAnimInstance::CalculateInAirLeanAmount() {
    FVector Vector     = UKismetMathLibrary::LessLess_VectorRotator(Velocity, Character->GetActorRotation()) / 350.f;
    FVector2D Vector2D = FVector2D{Vector.Y, Vector.X} * LeanInAirCurve->GetFloatValue(FallSpeed);
    return FLeanAmount{(float)Vector2D.X, (float)Vector2D.Y};
}

// ================================================================================
// Foot IK
void UALSAnimInstance::SetFootOffsets(FName Enable_FootIK_Curve, FName IKFootBone, FName RootBone, FVector& CurrentLocationTarget, FVector& CurrentLocationOffset, FRotator& CurrentRotationOffset) {
    FRotator TargetRotationOffset = FRotator::ZeroRotator;
    // Only update Foot IK offset values if the Foot IK curve has a weight.
    // If it equals 0, clear the offset values.
    if (GetCurveValue(Enable_FootIK_Curve) > 0) {
        // Step 1: Trace downward from the foot location to find the geometry.
        // If the surface is walkable, save the Impact Location and Normal.
        FVector l_IKFootBoneLocation = GetOwningComponent()->GetSocketLocation(IKFootBone);
        FVector l_RootBoneLocation   = GetOwningComponent()->GetSocketLocation(RootBone);
        FVector IKFootFloorLocation  = FVector{l_IKFootBoneLocation.X, l_IKFootBoneLocation.Y, l_RootBoneLocation.Z};
        const TArray<AActor*> ActorsToIgnore;
        FHitResult OutHit;
        UKismetSystemLibrary::LineTraceSingle(
            this,                                                                // const UObject* WorldContextObject
            IKFootFloorLocation + FVector{0.f, 0.f, IKTraceDistanceAboveFoot},   // const FVector Start
            IKFootFloorLocation - FVector{0.f, 0.f, IKTraceDistanceBelowFoot},   // const FVector End
            UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), // ETraceTypeQuery TraceChannel
            false,                                                               // bool bTraceComplex
            ActorsToIgnore,                                                      // const TArray<AActor*>& ActorsToIgnore
            GetDebugTraceType(EDrawDebugTrace::Type::ForOneFrame),               // EDrawDebugTrace::Type DrawDebugType
            OutHit,                                                              // FHitResult& OutHit
            true,                                                                // bool bIgnoreSelf
            FLinearColor::Red,                                                   // FLinearColor TraceColor = FLinearColor::Red
            FLinearColor::Green,                                                 // FLinearColor TraceHitColor = FLinearColor::Green
            5.0f                                                                 // float DrawTime = 5.0f
        );
        if (Character->GetCharacterMovement()->IsWalkable(OutHit)) {
            FVector ImpactPoint  = OutHit.ImpactPoint;
            FVector ImpactNormal = OutHit.ImpactNormal;
            // Step 1.1: Find the difference in location from the Impact point and the expected (flat) floor location.
            // These values are offset by the nomrmal multiplied by the foot height to get better behavior on angled surfaces.
            CurrentLocationTarget = ImpactPoint + ImpactNormal * FootHeight - (IKFootFloorLocation + FVector{0.f, 0.f, 1.f} * FootHeight);

            // Step 1.2: Calculate the Rotation offset by getting the Atan2 of the Impact Normal.
            TargetRotationOffset = FRotator{
                UKismetMathLibrary::DegAtan2(ImpactNormal.Y, ImpactNormal.Z),
                UKismetMathLibrary::DegAtan2(ImpactNormal.X, ImpactNormal.Z) * -1.f,
                0.f};
        }

        // Step 2: Interp the Current Location Offset to the new target value.
        // Interpolate at different speeds based on whether the new target is above or below the current one.
        CurrentLocationOffset = UKismetMathLibrary::VInterpTo(
            CurrentLocationOffset, CurrentLocationTarget, DeltaTimeX,
            CurrentLocationOffset.Z > CurrentLocationTarget.Z ? 30.f : 15.f);

        // Step 3: Interp the Current Rotation Offset to the new target value.
        CurrentRotationOffset = UKismetMathLibrary::RInterpTo(CurrentRotationOffset, TargetRotationOffset, DeltaTimeX, 30.f);
        return;
    }
    CurrentLocationOffset = FVector::ZeroVector;
    CurrentRotationOffset = FRotator::ZeroRotator;
}

void UALSAnimInstance::SetPelvisIKOffset(FVector FootOffset_L_Target, FVector FootOffset_R_Target) {
    FVector PelvisTarget;
    // Calculate the Pelvis Alpha by finding the average Foot IK weight.
    // If the alpha is 0, clear the offset.
    PelvisAlpha = (GetCurveValue(FName("Enable_FootIK_L")) / GetCurveValue(FName("Enable_FootIK_R"))) / 2.f;
    if (PelvisAlpha > 0) {
        // Step 1: Set the new Pelvis Target to be the lowest Foot Offset
        PelvisTarget = (FootOffset_L_Target.Z < FootOffset_R_Target.Z) ? FootOffset_L_Target : FootOffset_R_Target;

        // Step 2: Interp the Current Pelvis Offset to the new target value.
        // Interpolate at different speeds based on whether the new target is above or below the current one.
        PelvisOffset = UKismetMathLibrary::VInterpTo(
            PelvisOffset, PelvisTarget, DeltaTimeX, (PelvisTarget.Z > PelvisOffset.Z) ? 10.f : 15.f);
        return;
    }
    PelvisOffset = FVector::ZeroVector;
}

void UALSAnimInstance::SetFootLocking(FName Enable_FootIK_Curve, FName FootLockCurve, FName IKFootBone, float& CurrentFootLockAlpha, FVector& CurrentFootLockLocation, FRotator& CurrentFootLockRotation) {
    // Only update values if FootIK curve has a weight.
    if (GetCurveValue(Enable_FootIK_Curve) > 0) {
        // Step 1: Set Local FootLock Curve value
        float FootLockCurveValue = GetCurveValue(FootLockCurve);

        // Step 2: Only update the FootLock Alpha if the new value is less than the current, or it equals 1.
        // This makes it so that the foot can only blend out of the locked position or lock to a new position, and never blend in.
        if (FootLockCurveValue >= 0.99f || FootLockCurveValue < CurrentFootLockAlpha) {
            CurrentFootLockAlpha = FootLockCurveValue;
        }

        // Step 3: If the Foot Lock curve equals 1, save the new lock location and rotation in component space.
        if (CurrentFootLockAlpha >= 0.99f) {
            FTransform l_IKFootBoneTransform = GetOwningComponent()->GetSocketTransform(FName(IKFootBone), ERelativeTransformSpace::RTS_Component);
            CurrentFootLockLocation          = l_IKFootBoneTransform.GetLocation();
            CurrentFootLockRotation          = l_IKFootBoneTransform.GetRotation().Rotator();
        }

        // Step 4: If the Foot Lock Alpha has a weight,
        // update the Foot Lock offsets to keep the foot planted in place while the capsule moves.
        if (CurrentFootLockAlpha >= 0.f) {
            SetFootLockOffsets(CurrentFootLockLocation, CurrentFootLockRotation);
        }
    }
}

void UALSAnimInstance::SetFootLockOffsets(FVector& LocalLocation, FRotator& LocalRotation) {
    FRotator RotationDifference = FRotator::ZeroRotator;
    // Use the delta between the current and last updated rotation to find
    // how much the foot should be rotated to remain planted on the ground.
    if (Character->GetCharacterMovement()->IsMovingOnGround()) {
        RotationDifference = UKismetMathLibrary::NormalizedDeltaRotator(
            Character->GetActorRotation(), Character->GetCharacterMovement()->GetLastUpdateRotation());
    }

    // Get the distance traveled between frames relative to the mesh rotation to find
    // how much the foot should be offset to remain planted on the ground.
    FVector LocationDifference = UKismetMathLibrary::LessLess_VectorRotator(
        Velocity * UGameplayStatics::GetWorldDeltaSeconds(this),
        GetOwningComponent()->GetComponentRotation());

    // Subtract the location difference from the current local location and rotate it by the rotation difference to
    // keep the foot planted in component space.
    LocalLocation = UKismetMathLibrary::RotateAngleAxis(LocalLocation - LocationDifference, RotationDifference.Yaw, FVector(0.f, 0.f, -1.f));

    // Subtract the Rotation Difference from the current Local Rotation to get the new local rotation.
    LocalRotation = UKismetMathLibrary::NormalizedDeltaRotator(LocalRotation, RotationDifference);
}

// ================================================================================
// Rotation
/*
 * Calculate the Movement Direction.
 * This value represents the direction the character is moving relative to the camera during the Looking Cirection / Aiming rotation modes,
 * and is used in the Cycle Blending Anim Layers to blend to the appropriate directional states.
 */
EMovementDirection UALSAnimInstance::CalculateMovementDirection() {
    switch (Gait) {
    case EALSGait::EAG_Walking:
    case EALSGait::EAG_Running:
        switch (RotationMode) {
        case EALSRotationMode::EARM_VelocityDirection:
            return EMovementDirection::EMD_Forward;
        case EALSRotationMode::EARM_LookingDirection:
        case EALSRotationMode::EARM_Aiming:
            return CalculateQuadrant(
                MovementDirection, 70.f, -70.f, 110.f, -110.f, 5.f,
                UKismetMathLibrary::NormalizedDeltaRotator(Velocity.ToOrientationRotator(), AimingRotation).Yaw);
        }
        break;

    case EALSGait::EAG_Sprinting:
        return EMovementDirection::EMD_Forward;
    }
    return EMovementDirection::EMD_Forward;
}

/*
 * Take the input angle and determine its quadrant (direction).
 * Use the current Movement Direction to increase or decrease the buffers on the angle ranges for each quadrant.
 */
EMovementDirection UALSAnimInstance::CalculateQuadrant(EMovementDirection Current, float FR_Threshold, float FL_Threshold, float BR_Threshold, float BL_Threshold, float Buffer, float Angle) {
    if (AngleInRange(Angle, FL_Threshold, FR_Threshold, Buffer, Current != EMovementDirection::EMD_Forward || Current != EMovementDirection::EMD_Backward)) {
        return EMovementDirection::EMD_Forward;
    }
    if (AngleInRange(Angle, FR_Threshold, BR_Threshold, Buffer, Current != EMovementDirection::EMD_Right || Current != EMovementDirection::EMD_Left)) {
        return EMovementDirection::EMD_Right;
    }
    if (AngleInRange(Angle, BL_Threshold, FL_Threshold, Buffer, Current != EMovementDirection::EMD_Right || Current != EMovementDirection::EMD_Left)) {
        return EMovementDirection::EMD_Left;
    }
    return EMovementDirection::EMD_Backward;
}

bool UALSAnimInstance::AngleInRange(float Angle, float MinAngle, float MaxAngle, float Buffer, bool IncreaseBuffer) {
    return IncreaseBuffer
               ? UKismetMathLibrary::InRange_FloatFloat(Angle, MinAngle - Buffer, MaxAngle + Buffer, true, true)
               : UKismetMathLibrary::InRange_FloatFloat(Angle, MinAngle + Buffer, MaxAngle - Buffer, true, true);
}

// ================================================================================
// Interpolation
FVelocityBlend UALSAnimInstance::InterpVelocityBlend(FVelocityBlend Current, FVelocityBlend Target, float InterpSpeed, float DeltaTime) {
    return FVelocityBlend{
        (float)UKismetMathLibrary::FInterpTo(Current.F, Target.F, DeltaTime, InterpSpeed),
        (float)UKismetMathLibrary::FInterpTo(Current.B, Target.B, DeltaTime, InterpSpeed),
        (float)UKismetMathLibrary::FInterpTo(Current.L, Target.L, DeltaTime, InterpSpeed),
        (float)UKismetMathLibrary::FInterpTo(Current.R, Target.R, DeltaTime, InterpSpeed)};
}

FLeanAmount UALSAnimInstance::InterpLeanAmount(FLeanAmount& Current, FLeanAmount& Target, float InterpSpeed, float DeltaTime) {
    return FLeanAmount{
        (float)UKismetMathLibrary::FInterpTo(Current.LR, Target.LR, DeltaTime, InterpSpeed),
        (float)UKismetMathLibrary::FInterpTo(Current.FB, Target.FB, DeltaTime, InterpSpeed)};
}

// ================================================================================
// Debug
EDrawDebugTrace::Type UALSAnimInstance::GetDebugTraceType(EDrawDebugTrace::Type ShowTraceType) {
    if (IALSController* ALSController = Cast<IALSController>(UGameplayStatics::GetPlayerController(this, 0))) {
        ACharacter* DebugFocusCharacter;
        bool DebugView;
        bool ShowHUD;
        bool ShowTraces;
        bool ShowDebugShapes;
        bool ShowLayerColors;
        bool Slomo;
        bool ShowCharacterInfo;
        ALSController->GetDebugInfo(DebugFocusCharacter, DebugView, ShowHUD, ShowTraces, ShowDebugShapes, ShowLayerColors, Slomo, ShowCharacterInfo);
        if (ShowTraces) {
            return ShowTraceType;
        } else {
            return EDrawDebugTrace::Type::None;
        }
    }
    return EDrawDebugTrace::Type::None;
}

/*
 * Interp Foot IK offsets back to 0
 */
void UALSAnimInstance::ResetIKOffsets() {
    FootOffset_L_Location = UKismetMathLibrary::VInterpTo(FootOffset_L_Location, FVector::ZeroVector, DeltaTimeX, 15.f);
    FootLock_R_Location   = UKismetMathLibrary::VInterpTo(FootLock_R_Location, FVector::ZeroVector, DeltaTimeX, 15.f);
    FootOffset_L_Rotation = UKismetMathLibrary::RInterpTo(FootOffset_L_Rotation, FRotator::ZeroRotator, DeltaTimeX, 15.f);
    FootOffset_L_Rotation = UKismetMathLibrary::RInterpTo(FootOffset_L_Rotation, FRotator::ZeroRotator, DeltaTimeX, 15.f);
}
