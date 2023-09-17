#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ALS/Interfaces/ALSAnimation.h"
#include "ALS/Data/Enums.h"
#include "ALS/Data/Structs.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ALS/Libraries/ALSMacroLibrary.h"
#include "ALSAnimInstance.generated.h"

UCLASS()
class ALS_API UALSAnimInstance : public UAnimInstance, public IALSAnimation {
    GENERATED_BODY()

   protected:
    UALSAnimInstance();
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    virtual bool HandleNotify(const FAnimNotifyEvent& AnimNotifyEvent) override;

   private:
    void PlayTransition(FDynamicMontageParams MontageParams);
    void PlayDynamicTransition(float ReTriggerDelay, FDynamicMontageParams MontageParams);

    // ================================================================================
    // IALSAnimation

    // Jumped
    virtual void Jumped() override {
        IsJumped = true;
        JumpPlayRate = UKismetMathLibrary::MapRangeClamped(Speed, 0.f, 600.f, 1.2f, 1.5f);
        AddTimer([this]() { IsJumped = false; }, 0.1f, false);
    }

    // Set Grounded Entry State
    virtual void SetGroundedEntryState(EGroundedEntryState NewGroundedEntryState) override {
        GroundedEntryState = NewGroundedEntryState;
    }

    // Set Overlay Override State
    virtual void SetOverlayOverrideState(int32 NewOverlayOverrideState) override {
        OverlayOverrideState = NewOverlayOverrideState;
    }

    // ================================================================================
    // Update Values
    void UpdateCharacterInfo();
    void UpdateAimingValues();
    void UpdateLayerValues();
    void UpdateFootIK();
    void UpdateMovementValues();
    void UpdateRotationValues();
    void UpdateInAirValues();
    void UpdateRagdollValues();

    // ================================================================================
    // Grounded
    bool ShouldMoveCheck();
    bool CanTurnInPlace();
    bool CanRotateInPlace();
    bool CanDynamicTransition();
    bool CanOverlayTransition();
    void TurnInPlace(FRotator TargetRotation, float PlayRateScale, float StartTime, bool OverrideCurrent);
    void TurnInPlaceCheck();
    void RotateInPlaceCheck();
    void DynamicTransitionCheck();

    // ================================================================================
    // Movement
    FVelocityBlend CalculateVelocityBlend();
    float CalculateDiagonalScaleAmount();
    FVector CalculateRelativeAccelerationAmount();
    float CalculateWalkRunBlend();
    float CalculateStrideBlend();
    float CalculateStandingPlayRate();
    float CalculateCrouchingPlayRate();

    // ================================================================================
    // In Air
    float CalculateLandPrediction();
    FLeanAmount CalculateInAirLeanAmount();

    // ================================================================================
    // Foot IK
    void SetFootOffsets(FName Enable_FootIK_Curve, FName IKFootBone, FName RootBone, FVector& CurrentLocationTarget, FVector& CurrentLocationOffset, FRotator& CurrentRotationOffset);
    void SetPelvisIKOffset(FVector FootOffset_L_Target, FVector FootOffset_R_Target);
    void SetFootLocking(FName Enable_FootIK_Curve, FName FootLockCurve, FName IKFootBone, float& CurrentFootLockAlpha, FVector& CurrentFootLockLocation, FRotator& CurrentFootLockRotation);
    void SetFootLockOffsets(FVector& LocalLocation, FRotator& LocalRotation);

    // ================================================================================
    // Rotation
    EMovementDirection CalculateMovementDirection();
    EMovementDirection CalculateQuadrant(EMovementDirection Current, float FR_Threshold, float FL_Threshold, float BR_Threshold, float BL_Threshold, float Buffer, float Angle);
    bool AngleInRange(float Angle, float MinAngle, float MaxAngle, float Buffer, bool IncreaseBuffer);

    // ================================================================================
    // Interpolation
    FVelocityBlend InterpVelocityBlend(FVelocityBlend Current, FVelocityBlend Target, float InterpSpeed, float DeltaTime);
    FLeanAmount InterpLeanAmount(FLeanAmount& Current, FLeanAmount& Target, float InterpSpeed, float DeltaTime);

    // ================================================================================
    // Debug
    EDrawDebugTrace::Type GetDebugTraceType(EDrawDebugTrace::Type ShowTraceType);

   private:
    void ResetIKOffsets();

   protected:
    // ================================================================================
    // References
    ACharacter* Character;
    float DeltaTimeX = 0.f;
    // ================================================================================
    // Character Information
    FRotator AimingRotation = FRotator::ZeroRotator;
    FVector Velocity = FVector::ZeroVector;
    FVector RelativeVelocityDirection = FVector::ZeroVector;
    FVector Acceleration = FVector::ZeroVector;
    FVector MovementInput = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Information")
    bool IsMoving = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Information")
    bool HasMovementInput = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Information")
    float Speed = 0.f;

    float MovementInputAmount = 0.f;
    float AimYawRate = 0.f;
    float ZoomAmount = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Information")
    EALSMovementState MovementState = EALSMovementState::EAMS_None;

    EALSMovementState PrevMovementState = EALSMovementState::EAMS_None;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Information")
    EALSMovementAction MovementAction = EALSMovementAction::EAMA_LowMantle;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Information")
    EALSRotationMode RotationMode = EALSRotationMode::EARM_VelocityDirection;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Information")
    EALSGait Gait = EALSGait::EAG_Walking;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Information")
    EALSStance Stance = EALSStance::EAS_Standing;

    EALSViewMode ViewMode = EALSViewMode::EAVM_ThirdPerson;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Information")
    EALSOverlayState OverlayState = EALSOverlayState::EAOS_Default;

    // ================================================================================
    // Anim Graph - Grounded
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    EGroundedEntryState GroundedEntryState = EGroundedEntryState::EGES_None;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    EMovementDirection MovementDirection = EMovementDirection::EMD_Forward;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    EHipsDirection TrackedHipsDirection = EHipsDirection::EHD_F;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    FVector RelativeAccelerationAmount = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    bool ShouldMove = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    bool RotateL = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    bool RotateR = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    bool Pivot = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    float RotateRate = 1.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    float RotationScale = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    float DiagonalScaleAmount = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    float WalkRunBlend = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    float StandingPlayRate = 1.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    float CrouchingPlayRate = 1.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    float StrideBlend = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    FVelocityBlend VelocityBlend = FVelocityBlend{};

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    FLeanAmount LeanAmount = FLeanAmount{};

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    float FYaw = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    float BYaw = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    float LYaw = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Grounded")
    float RYaw = 0.f;

    // ================================================================================
    // Anim Graph - In Air
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - In Air")
    bool IsJumped = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - In Air")
    float JumpPlayRate = 1.2f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - In Air")
    float FallSpeed = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - In Air")
    float LandPrediction = 1.f;

    // ================================================================================
    // Anim Graph - Aiming Values
    FRotator SmoothedAimingRotation = FRotator::ZeroRotator;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Aiming Values")
    FRotator SpineRotation = FRotator::ZeroRotator;

    FVector2D AimingAngle = FVector2D::ZeroVector;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Aiming Values")
    FVector2D SmoothedAimingAngle = FVector2D::ZeroVector;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Aiming Values")
    float AimSweepTime = 0.5f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Aiming Values")
    float InputYawOffsetTime = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Aiming Values")
    float ForwardYawTime = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Aiming Values")
    float LeftYawTime = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Aiming Values")
    float RightYawTime = 0.f;

    // ================================================================================
    // Anim Graph - Ragdoll
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Ragdoll")
    float FlailRate = 0.f;

    // ================================================================================
    // Anim Graph - Layer Blending
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    int OverlayOverrideState = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    float EnableAimOffset = 1.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    float BasePose_N = 1.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    float BasePose_CLF = 0.f;

    float Arm_L = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    float Arm_L_Add = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    float Arm_L_LS = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    float Arm_L_MS = 0.f;

    float Arm_R = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    float Arm_R_Add = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    float Arm_R_LS = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    float Arm_R_MS = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    float Hand_L = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    float Hand_R = 0.f;

    float Legs = 0.f;
    float LegsAdd = 0.f;
    float Pelvis = 0.f;
    float PelvisAdd = 0.f;
    float Spine = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    float SpineAdd = 0.f;

    float Head = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    float HeadAdd = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    float EnableHandIK_L = 1.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Layer Blending")
    float EnableHandIK_R = 1.f;

    // ================================================================================
    // Anim Graph - Foot IK
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Foot IK")
    float FootLock_L_Alpha = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Foot IK")
    float FootLock_R_Alpha = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Foot IK")
    FVector FootLock_L_Location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Foot IK")
    FVector FootLock_R_Location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Foot IK")
    FRotator FootLock_L_Rotation = FRotator::ZeroRotator;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Foot IK")
    FRotator FootLock_R_Rotation = FRotator::ZeroRotator;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Foot IK")
    FVector FootOffset_L_Location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Foot IK")
    FVector FootOffset_R_Location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Foot IK")
    FRotator FootOffset_L_Rotation = FRotator::ZeroRotator;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Foot IK")
    FRotator FootOffset_R_Rotation = FRotator::ZeroRotator;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Foot IK")
    FVector PelvisOffset = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Graph - Foot IK")
    float PelvisAlpha = 0.f;

    // ================================================================================
    // Turn In Place
    float TurnCheckMinAngle = 45.f;
    float Turn180Threshold = 130.f;
    float AimYawRateLimit = 50.f;
    float ElapsedDelayTime = 0.f;
    float MinAngleDelay = 0.75f;
    float MaxAngleDelay = 0.f;
    FTurnInPlaceAsset N_TurnIP_L90 = FTurnInPlaceAsset{};
    FTurnInPlaceAsset N_TurnIP_R90 = FTurnInPlaceAsset{};
    FTurnInPlaceAsset N_TurnIP_L180 = FTurnInPlaceAsset{};
    FTurnInPlaceAsset N_TurnIP_R180 = FTurnInPlaceAsset{};
    FTurnInPlaceAsset CLF_TurnIP_L90 = FTurnInPlaceAsset{};
    FTurnInPlaceAsset CLF_TurnIP_R90 = FTurnInPlaceAsset{};
    FTurnInPlaceAsset CLF_TurnIP_L180 = FTurnInPlaceAsset{};
    FTurnInPlaceAsset CLF_TurnIP_R180 = FTurnInPlaceAsset{};

    // ================================================================================
    // Rotate in Place
    float RotateMinThreshold = -50.f;
    float RotateMaxThreshold = 50.f;
    float AimYawRateMinRange = 90.f;
    float AimYawRateMaxRange = 270.f;
    float MinPlayRate = 1.15f;
    float MaxPlayRate = 3.f;

    // ================================================================================
    // Blend Curves
    UCurveFloat* DiagonalScaleAmountCurve;
    UCurveFloat* StrideBlend_N_Walk;
    UCurveFloat* StrideBlend_N_Run;
    UCurveFloat* StrideBlend_C_Walk;
    UCurveFloat* LandPredictionCurve;
    UCurveFloat* LeanInAirCurve;
    UCurveVector* YawOffset_FB;
    UCurveVector* YawOffset_LR;

    // ================================================================================
    // Config
    float AnimatedWalkSpeed = 150.f;
    float AnimatedRunSpeed = 350.f;
    float AnimatedSprintSpeed = 600.f;
    float AnimatedCrouchSpeed = 150.f;
    float VelocityBlendInterpSpeed = 12.f;
    float GroundedLeanInterpSpeed = 4.f;
    float InAirLeanInterpSpeed = 4.f;
    float SmoothedAimingRotationInterpSpeed = 10.f;
    float InputYawOffsetInterpSpeed = 8.f;
    float TriggerPivotSpeedLimit = 200.f;
    float FootHeight = 13.5f;
    float IKTraceDistanceAboveFoot = 50.f;
    float IKTraceDistanceBelowFoot = 45.f;

   private:
    bool PlayDynamicTransitionEnabled = true;
};
