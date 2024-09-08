#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ALS/Interfaces/ALSCharacter.h"
#include "ALS/Interfaces/ALSCamera.h"
#include "ALS/Data/ALSEnums.h"
#include "ALS/Data/ALSStructs.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DataTable.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ALSBaseCharacter.generated.h"

class UTimelineComponent;

/*
 * IMPORTANT!!!
 * This blueprint contains a lot of simple logic meant to be built on or replaced by more sophisticated systems on a Per Project basis.
 * How you manage Movement, Movement actions, Gameplay Events, and Character States should all be determined by the specific needs of your project.
 * The most important thing is to make sure you are updating the correct Character Information in the interface to allow the AnimBP to function correctly.
 */
UCLASS()
class ALS_API AALSBaseCharacter : public ACharacter, public IALSCharacter, public IALSCamera {
    GENERATED_BODY()

   public:
    AALSBaseCharacter();
    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

    virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
    virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
    virtual void Landed(const FHitResult& Hit) override;
    virtual void OnJumped_Implementation() override;

   public:
    // ================================================================================
    // IALSCharacter
    virtual void GetCurrentStates(TEnumAsByte<EMovementMode>& OutPawnMovementMode, EALSMovementState& OutMovementState, EALSMovementState& OutPrevMovementState, EALSMovementAction& OutMovementAction, EALSRotationMode& OutRotationMode, EALSGait& OutActualGait, EALSStance& OutActualStance, EALSViewMode& OutViewMode, EALSOverlayState& OutOverlayState) override;
    virtual void GetEssentialValues(FVector& OutVelocity, FVector& OutAcceleration, FVector& OutMovementInput, bool& OutIsMoving, bool& OutHasMovementInput, float& OutSpeed, float& OutMovementInputAmount, FRotator& OutAimingRotation, float& OutAimYawRate) override;
    virtual void SetMovementState(EALSMovementState NewMovementState) override;
    virtual void SetMovementAction(EALSMovementAction NewMovementAction) override;
    virtual void SetRotationMode(EALSRotationMode NewRotationMode) override;
    virtual void SetGait(EALSGait NewGait) override;
    virtual void SetViewMode(EALSViewMode NewViewMode) override;
    virtual void SetOverlayState(EALSOverlayState NewOverlayState) override;

    // ================================================================================
    // IALSCamera
    virtual void Get3PTraceParams(FVector& OutTraceOrigin, float& OutTraceRadius, TEnumAsByte<ETraceTypeQuery>& OutTraceChannel) override;
    virtual FTransform Get3PPivotTarget() override;
    virtual FVector GetFPCameraTarget() override;
    virtual void GetCameraParameters(float& OutTPFOV, float& OutFPFOV, bool& OutRightShoulder) override;

    // ================================================================================
    // Components
    UFUNCTION()
    void OnTimelineComponentPostUpdate();
    UFUNCTION()
    void OnTimelineComponentFinished();

    // ================================================================================
    // Utility
    void GetControlForwardRightVector(FVector& ForwardVector, FVector& RightVector);
    FVector GetCalpsuleBaseLocation(float ZOffset);
    FVector GetCapsuleLocationFromBase(FVector BaseLocation, float ZOffset);
    float GetAnimCurveValue(FName CurveName);

    // ================================================================================
    // Input
    void PlayerMovementInput(bool IsForwardAxis);
    FVector GetPlayerMovementInput();
    void FixDiagonalGamepadValues(float InY, float InX, float& OutY, float& OutX);

    // ================================================================================
    // Essential Information
    void SetEssentialValues();
    void CacheValues();
    FVector CalculateAcceleration();

    // ================================================================================
    // State Changes
    void OnBeginPlay();
    void OnCharacterMovementModeChanged(TEnumAsByte<EMovementMode> PrevMovementMode, TEnumAsByte<EMovementMode> NewMovementMode, uint8 PrevCustomMode);
    void OnMovementStateChanged(EALSMovementState NewMovementState);
    void OnMovementActionChanged(EALSMovementAction NewMovementAction);
    void OnStanceChanged(EALSStance NewStance);
    void OnRotationModeChanged(EALSRotationMode NewRotationMode);
    void OnGaitChanged(EALSGait NewActualGait);
    void OnViewModeChanged(EALSViewMode NewViewMode);
    virtual void OnOverlayStateChanged(EALSOverlayState NewOverlayState);

    // ================================================================================
    // Movement System
    void SetMovementModel();
    void UpdateCharacterMovement();
    void UpdateDynamicMovementSettings(EALSGait AllowedGait);
    FMovementSettings GetTargetMovementSettings();
    EALSGait GetAllowedGait();
    EALSGait GetActualGait(EALSGait AllowedGait);
    bool CanSprint();
    float GetMappedSpeed();
    virtual UAnimMontage* GetRollAnimation();

    // ================================================================================
    // Rotation System
    void UpdateGroudedRotation();
    void UpdateInAirRotation();
    void SmoothCharacterRotation(FRotator Target, float TargetInterpSpeedConst, float ActorInterpSpeedSmooth);
    void AddToCharacterRotation(FRotator DeltaRotation);
    void LimitRotation(float AimYawMin, float AimYawMax, float InterpSpeed);
    bool SetActorLocationAndRotationUpdateTarget(FVector NewLocation, FRotator NewRotation, bool bSweep, bool bTeleport, FHitResult& SweepHitResult);
    float CalculateGroundedRotationRate();
    bool CanUpdateMovingRotation();

    // ================================================================================
    // Mantle System
    bool MantleCheck(FMantleTraceSettings TraceSettings, TEnumAsByte<EDrawDebugTrace::Type> DebugType);
    virtual void MantleStart(float MantleHeight, FALSComponentAndTransform MantleLedgeWS, EMantleType MantleType);
    virtual void MantleEnd();
    void MantleUpdate(float BlendIn);
    bool CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset, float RadiusOffset, TEnumAsByte<EDrawDebugTrace::Type> DebugType);
    virtual FMantleAsset GetMantleAsset(EMantleType MantleType);

    // ================================================================================
    // Ragdoll System
    virtual void RagdollStart();
    virtual void RagdollEnd();
    void RagdollUpdate();
    void SetActorLocationDuringRagdoll();
    virtual UAnimMontage* GetGetUpAnimation(bool InRagdollFaceUp);

    // ================================================================================
    // Debug
    void DrawDebugShapes();
    TEnumAsByte<EDrawDebugTrace::Type> GetTraceDebugType(TEnumAsByte<EDrawDebugTrace::Type> ShowTraceType);

   public:
    // ================================================================================
    // Components
    UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "Components")
    UTimelineComponent* MantleTimeline;

    // ================================================================================
    // References
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "References")
    UAnimInstance* MainAnimInstance;

    // ================================================================================
    // Input
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
    EALSRotationMode DesiredRotationMode = EALSRotationMode::EARM_LookingDirection;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
    EALSGait DesiredGait = EALSGait::EAG_Running;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
    EALSStance DesiredStance = EALSStance::EAS_Standing;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
    float LookUpDownRate = 1.25;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
    float LookLeftRightRate = 1.25;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
    int32 TimesPressedStance = 0;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
    bool BreakFall = false;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
    bool SprintHeld = false;

    // ================================================================================
    // Essential Information
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Essential Information")
    FVector Acceleration = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Essential Information")
    bool IsMoving = false;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Essential Information")
    bool HasMovementInput = false;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Essential Information")
    FRotator LastVelocityRotation = FRotator::ZeroRotator;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Essential Information")
    FRotator LastMovementInputRotation = FRotator::ZeroRotator;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Essential Information")
    float Speed = 0;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Essential Information")
    float MovementInputAmount = 0;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Essential Information")
    float AimYawRate = 0;

    // ================================================================================
    // Camera System
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera System")
    float ThirdPersonFOV = 90;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera System")
    float FirstPersonFOV = 90;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera System")
    bool RightShoulder = false;

    // ================================================================================
    // State Values
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "State Values")
    EALSMovementState MovementState = EALSMovementState::EAMS_None;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "State Values")
    EALSMovementState PrevMovementState = EALSMovementState::EAMS_None;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "State Values")
    EALSMovementAction MovementAction = EALSMovementAction::EAMA_None;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "State Values")
    EALSRotationMode RotationMode = EALSRotationMode::EARM_LookingDirection;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "State Values")
    EALSGait Gait = EALSGait::EAG_Walking;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "State Values")
    EALSStance Stance = EALSStance::EAS_Standing;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "State Values")
    EALSViewMode ViewMode = EALSViewMode::EAVM_ThirdPerson;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State Values")
    EALSOverlayState OverlayState = EALSOverlayState::EAOS_Default;

    // ================================================================================
    // Movement System
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement System")
    FDataTableRowHandle MovementModel;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement System")
    FMovementSettingsState MovementData;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement System")
    FMovementSettings CurrentMovementSettings;

    // ================================================================================
    // Rotation System
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Rotation System")
    FRotator TargetRotation = FRotator::ZeroRotator;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Rotation System")
    FRotator InAirRotation = FRotator::ZeroRotator;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Rotation System")
    float YawOffset = 0.f;

    // ================================================================================
    // Mantle System
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mantle System")
    FMantleParams MantleParams;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mantle System")
    FALSComponentAndTransform MantleLedgeLS;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mantle System")
    FTransform MantleTarget = {FRotator::ZeroRotator, FVector::ZeroVector, FVector{1.f}};

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mantle System")
    FTransform MantleActualStartOffset = {FRotator::ZeroRotator, FVector::ZeroVector, FVector{1.f}};

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mantle System")
    FTransform MantleAnimatedStartOffset = {FRotator::ZeroRotator, FVector::ZeroVector, FVector{1.f}};

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mantle System")
    FMantleTraceSettings GroundedTraceSettings = {250.f, 50.f, 75.f, 30.f, 30.f};

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mantle System")
    FMantleTraceSettings AutomaticTraceSettings = {80.f, 40.f, 50.f, 30.f, 30.f};

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mantle System")
    FMantleTraceSettings FallingTraceSettings = {150.f, 50.f, 70.f, 30.f, 30.f};

    // ================================================================================
    // Ragdoll System
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Ragdoll System")
    bool RagdollOnGround = false;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Ragdoll System")
    bool RagdollFaceUp = false;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Ragdoll System")
    FVector LastRagdollVelocity = FVector::ZeroVector;

    // ================================================================================
    // Cached Variables
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Cached Variables")
    FVector PreviousVelocity = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Cached Variables")
    float PreviousAimYaw = 0;

   private:
    UFUNCTION()
    FORCEINLINE void SetCharacterMovementBrakingFrictionFactorToZero() { GetCharacterMovement()->BrakingFrictionFactor = 0.f; };

    UFUNCTION()
    FORCEINLINE void ResetStancePressedTimes() { StancePressedTimes = 0; };

    UFUNCTION()
    FORCEINLINE void SetBreakFallToFlase() { BreakFall = false; };

    UFUNCTION()
    FORCEINLINE void ResetSprintPressedTimes() { SprintPressedTimes = 0; }

    UFUNCTION()
    FORCEINLINE void SetDesiredGaitToWalkingIfNotSprintHeld() {
        if (!SprintHeld) {
            DesiredGait = EALSGait::EAG_Walking;
        }
    }

    void PerformCameraAction(EInputEvent InputEvent);

    int32 StancePressedTimes = 0;
    int32 SprintPressedTimes = 0;
    bool CameraActionFinished;
};
