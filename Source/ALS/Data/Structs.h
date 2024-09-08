#pragma once

#include "Curves/CurveVector.h"
#include "Engine/DataTable.h"
#include "ALS/Data/Enums.h"
#include "Structs.generated.h"

USTRUCT(BlueprintType)
struct FALSComponentAndTransform {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FTransform Transform;

    UPROPERTY(BlueprintReadWrite)
    UPrimitiveComponent* Component = nullptr;
};

USTRUCT(BlueprintType)
struct FCameraSettings {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    float TargetArmLength = 0.f;

    UPROPERTY(BlueprintReadWrite)
    FVector SocketOffset = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite)
    float LagSpeed = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float RotationLagSpeed = 0.f;

    UPROPERTY(BlueprintReadWrite)
    bool DoCollisionTest = true;
};

USTRUCT(BlueprintType)
struct FCameraSettingsGait {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FCameraSettings Walking;

    UPROPERTY(BlueprintReadWrite)
    FCameraSettings Running;

    UPROPERTY(BlueprintReadWrite)
    FCameraSettings Sprinting;

    UPROPERTY(BlueprintReadWrite)
    FCameraSettings Crouching;
};

USTRUCT(BlueprintType)
struct FCameraSettingsState {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FCameraSettingsGait VelocityDirection;

    UPROPERTY(BlueprintReadWrite)
    FCameraSettingsGait LookingDirection;

    UPROPERTY(BlueprintReadWrite)
    FCameraSettingsGait Aiming;
};

USTRUCT(BlueprintType)
struct FDynamicMontageParams {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    UAnimSequenceBase* Animation = nullptr;

    UPROPERTY(BlueprintReadWrite)
    float BlendInTime = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float BlendOutTime = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float PlayRate = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float StartTime = 0.f;
};

USTRUCT(BlueprintType)
struct FLeanAmount {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    float LR = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float FB = 0.f;
};

USTRUCT(BlueprintType)
struct FMantleAsset {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    UAnimMontage* AnimMontage = nullptr;

    UPROPERTY(BlueprintReadWrite)
    UCurveVector* PositionCorrectionCurve = nullptr;

    UPROPERTY(BlueprintReadWrite)
    FVector StartingOffset = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite)
    float LowHeight = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float LowPlayRate = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float LowStartPosition = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float HighHeight = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float HighPlayRate = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float HighStartPosition = 0.f;
};

USTRUCT(BlueprintType)
struct FMantleParams {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    UAnimMontage* AnimMontage = nullptr;

    UPROPERTY(BlueprintReadWrite)
    UCurveVector* PositionCorrectionCurve = nullptr;

    UPROPERTY(BlueprintReadWrite)
    float StartingPosition = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float PlayRate = 0.f;

    UPROPERTY(BlueprintReadWrite)
    FVector StartingOffset = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FMantleTraceSettings {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    float MaxLedgeHeight = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float MinLedgeHeight = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float ReachDistance = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float ForwardTraceRadius = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float DownwardTraceRadius = 0.f;
};

USTRUCT(BlueprintType)
struct FMovementSettings {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    float WalkSpeed = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float RunSpeed = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float SprintSpeed = 0.f;

    UPROPERTY(BlueprintReadWrite)
    UCurveVector* MovementCurve = nullptr;

    UPROPERTY(BlueprintReadWrite)
    UCurveFloat* RotationRateCurve = nullptr;
};

USTRUCT(BlueprintType)
struct FMovementSettingsStance {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FMovementSettings Standing;

    UPROPERTY(BlueprintReadWrite)
    FMovementSettings Crouching;
};

USTRUCT(BlueprintType)
struct FMovementSettingsState : public FTableRowBase {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FMovementSettingsStance VelocityDirection;

    UPROPERTY(BlueprintReadWrite)
    FMovementSettingsStance LookingDirection;

    UPROPERTY(BlueprintReadWrite)
    FMovementSettingsStance Aiming;
};

USTRUCT(BlueprintType)
struct FRotateInPlaceAsset {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    UAnimSequenceBase* Animation = nullptr;

    UPROPERTY(BlueprintReadWrite)
    FName SlotName;

    UPROPERTY(BlueprintReadWrite)
    float SlowTurnRate = 90.f;

    UPROPERTY(BlueprintReadWrite)
    float FastTurnRate = 90.f;

    UPROPERTY(BlueprintReadWrite)
    float SlowPlayRate = 1.f;

    UPROPERTY(BlueprintReadWrite)
    float FastPlayRate = 1.f;
};

USTRUCT(BlueprintType)
struct FTurnInPlaceAsset {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    UAnimSequenceBase* Animation = nullptr;

    UPROPERTY(BlueprintReadWrite)
    float AnimatedAngle = 0.f;

    UPROPERTY(BlueprintReadWrite)
    FName SlotName;

    UPROPERTY(BlueprintReadWrite)
    float PlayRate = 1.f;

    UPROPERTY(BlueprintReadWrite)
    bool ScaleTurnAngle = true;
};

USTRUCT(BlueprintType)
struct FVelocityBlend {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    float F = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float B = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float L = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float R = 0.f;
};

USTRUCT(BlueprintType)
struct FOverlayStateButtonParams {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    UUserWidget* Widget = nullptr;

    UPROPERTY(BlueprintReadWrite)
    EALSOverlayState State = EALSOverlayState::EAOS_Default;
};

USTRUCT(BlueprintType)
struct FAnimCurveCreationData {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 FrameNumber = 0;

    UPROPERTY(BlueprintReadWrite)
    float CurveValue = 0.f;
};

USTRUCT(BlueprintType)
struct FAnimCurveCreationParams {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FName CurveName = "None";

    UPROPERTY(BlueprintReadWrite)
    bool KeyEachFrame = false;

    UPROPERTY(BlueprintReadWrite, meta = (Tooltip = "X=Percentage, Y=Value"))
    TArray<FAnimCurveCreationData> Keys;
};
