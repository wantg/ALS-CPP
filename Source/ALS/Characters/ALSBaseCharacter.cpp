#include "ALS/Characters/ALSBaseCharacter.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "ALS/Characters/AI/ALSAIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ALS/Interfaces/ALSAnimation.h"
#include "ALS/Libraries/ALSMacroLibrary.h"

AALSBaseCharacter::AALSBaseCharacter() {
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->SetCollisionProfileName(FName("ALS_Character"));
    GetCapsuleComponent()->SetAreaClassOverride(UNavArea_Obstacle::StaticClass());
    GetCapsuleComponent()->SetCapsuleHalfHeight(90.f);
    GetCapsuleComponent()->SetCapsuleRadius(30.f);
    GetCapsuleComponent()->ShapeColor = FColor::Black;

    GetMesh()->SetRelativeLocation(FVector{0.f, 0.f, -92.f});
    GetMesh()->SetRelativeRotation(FRotator{0.f, -90.f, 0.f});
    GetMesh()->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/Meshes/Proxy")));
    GetMesh()->SetMaterial(0, LoadObject<UMaterial>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/Materials/M_AnimMan_Default")));
    GetMesh()->SetMaterial(1, LoadObject<UMaterial>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/Materials/M_AnimMan_Eyes")));
    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Ignore);
    // GetMesh()->ClothingSimulationFactory = UClothingSimulationFactoryNv::StaticClass();
    GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;

    GetCharacterMovement()->MaxAcceleration       = 1500.f;
    GetCharacterMovement()->BrakingFrictionFactor = 0.f;
    GetCharacterMovement()->SetCrouchedHalfHeight(60.f);
    GetCharacterMovement()->bRunPhysicsWithNoController = true;
    GetCharacterMovement()->SetWalkableFloorZ(0.71f);
    GetCharacterMovement()->MinAnalogWalkSpeed             = 25.f;
    GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
    GetCharacterMovement()->PerchRadiusThreshold           = 20.f;
    GetCharacterMovement()->PerchAdditionalHeight          = 0.f;
    GetCharacterMovement()->LedgeCheckThreshold            = 0.f;
    GetCharacterMovement()->AirControl                     = 0.15f;
    GetCharacterMovement()->NavAgentProps.bCanCrouch       = true;
    GetCharacterMovement()->NavAgentProps.bCanFly          = true;

    if (FProperty* Property = GetCharacterMovement()->GetClass()->FindPropertyByName(FName("bUseAccelerationForPaths"))) {
        if (uint8* Value = Property->ContainerPtrToValuePtr<uint8>(GetCharacterMovement())) *Value += 2;
    }

    bUseControllerRotationYaw = false;
    AIControllerClass         = AALSAIController::StaticClass();
    Tags                      = {FName("ALS_Character")};

    // Mantle Timeline: This timeline is triggered from the MantleStart function and updates the function that lerps the character to the new location.
    UCurveFloat* CurveFloat = CreateDefaultSubobject<UCurveFloat>(TEXT("CurveFloat"));
    // const static FString CurveJSONString = LoadObject<UCurveFloat>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/BaseCharacterMantleTimelineBlendIn"))->ExportAsJSONString();
    // TArray<FString> OutProblems;
    // CurveFloat->ImportFromJSONString(CurveJSONString, OutProblems);
    FKeyHandle KeyHandle1 = CurveFloat->FloatCurve.UpdateOrAddKey(0.f, 0.f);
    CurveFloat->FloatCurve.SetKeyInterpMode(KeyHandle1, ERichCurveInterpMode::RCIM_Cubic);
    CurveFloat->FloatCurve.SetKeyTangentMode(KeyHandle1, ERichCurveTangentMode::RCTM_Auto);
    FKeyHandle KeyHandle2 = CurveFloat->FloatCurve.UpdateOrAddKey(0.2f, 1.f);
    CurveFloat->FloatCurve.SetKeyInterpMode(KeyHandle2, ERichCurveInterpMode::RCIM_Linear);
    CurveFloat->FloatCurve.SetKeyTangentMode(KeyHandle2, ERichCurveTangentMode::RCTM_Auto);
    CurveFloat->FloatCurve.ReadjustTimeRange(0.f, 0.2f, false, 0.f, 0.0f);

    MantleTimeline                 = CreateDefaultSubobject<UTimelineComponent>(TEXT("MantleTimeline"));
    MantleTimeline->CreationMethod = EComponentCreationMethod::Native; // Indicate it comes from a blueprint so it gets cleared when we rerun construction scripts
    this->BlueprintCreatedComponents.Add(MantleTimeline);              // Add to array so it gets saved
    MantleTimeline->SetNetAddressable();                               // This component has a stable name that can be referenced for replication
    // MantleTimeline->SetPropertySetObject(this);  // Set which object the timeline should drive properties on
    // MantleTimeline->SetDirectionPropertyName(FName("TimelineDirection"));
    MantleTimeline->SetLooping(false);
    MantleTimeline->SetTimelineLength(5.f);
    MantleTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
    MantleTimeline->SetPlaybackPosition(0.0f, false);

    FOnTimelineFloat TimelineComponentAddInterpFloatDelegate;
    MantleTimeline->AddInterpFloat(CurveFloat, TimelineComponentAddInterpFloatDelegate);
    FOnTimelineEvent TimelineComponentPostUpdateDelegate;
    TimelineComponentPostUpdateDelegate.BindDynamic(this, &AALSBaseCharacter::OnTimelineComponentPostUpdate);
    MantleTimeline->SetTimelinePostUpdateFunc(TimelineComponentPostUpdateDelegate);
    FOnTimelineEvent TimelineComponentFinishedDelegate;
    TimelineComponentFinishedDelegate.BindDynamic(this, &AALSBaseCharacter::OnTimelineComponentFinished);
    MantleTimeline->SetTimelineFinishedFunc(TimelineComponentFinishedDelegate);

    MovementModel.DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/DataTables/MovementModelTable"));
    MovementModel.RowName   = FName("Normal");

    CurrentMovementSettings.WalkSpeed         = 165.f;
    CurrentMovementSettings.RunSpeed          = 350.f;
    CurrentMovementSettings.SprintSpeed       = 600.f;
    CurrentMovementSettings.MovementCurve     = LoadObject<UCurveVector>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/CharacterMovementCurves/NormalMovement"));
    CurrentMovementSettings.RotationRateCurve = nullptr;

    MantleParams.PlayRate   = 1.f;
    MantleLedgeLS.Transform = FTransform{FRotator::ZeroRotator, FVector::ZeroVector, FVector{1.f}};
}

void AALSBaseCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    // Do Every Frame
    SetEssentialValues();
    // Check Movement Mode
    switch (MovementState) {
    case EALSMovementState::EAMS_Grounded:
        // Do While On Ground
        UpdateCharacterMovement();
        UpdateGroudedRotation();
        break;
    case EALSMovementState::EAMS_InAir:
        // Do while In Air
        UpdateInAirRotation();
        // Perform a mantle check if falling while movement input is pressed.
        if (HasMovementInput) {
            MantleCheck(FallingTraceSettings, EDrawDebugTrace::ForOneFrame);
        }
        break;
    case EALSMovementState::EAMS_Ragdoll:
        // Do while in Ragdoll
        RagdollUpdate();
    }
    CacheValues();
    DrawDebugShapes();
}

void AALSBaseCharacter::BeginPlay() {
    Super::BeginPlay();
    OnBeginPlay();
}

/*
 * On Pawn Movement Mode Changed
 */
void AALSBaseCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) {
    Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
    OnCharacterMovementModeChanged(PrevMovementMode, GetCharacterMovement()->MovementMode, PreviousCustomMode);
}

/*
 * Crouch / UnCrouch
 */
void AALSBaseCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) {
    Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
    OnStanceChanged(EALSStance::EAS_Crouching);
}
void AALSBaseCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) {
    Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
    OnStanceChanged(EALSStance::EAS_Standing);
}

/*
 * On Landed: Temporarily increase the braking friction on lands to make landings more accurate, or trigger a breakfall roll.
 */
void AALSBaseCharacter::Landed(const FHitResult& Hit) {
    Super::Landed(Hit);
    if (BreakFall) {
        if (MainAnimInstance) {
            // Breakfall: Simply play a Root Motion Montage.
            MainAnimInstance->Montage_Play(
                GetRollAnimation(),                    // UAnimMontage* MontageToPlay
                1.35,                                  // float InPlayRate = 1.f
                EMontagePlayReturnType::MontageLength, // EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength
                0.f,                                   // float InTimeToStartMontageAt=0.f
                true                                   // bool bStopAllMontages = true
            );
        }
        return;
    }
    GetCharacterMovement()->BrakingFrictionFactor = HasMovementInput ? 0.5f : 3.f;
    RetriggerDelay(0.5f, TEXT("SetCharacterMovementBrakingFrictionFactorToZero"));
}

/*
 * On Jumped: Set the new In Air Rotation to the velocity rotation if speed is greater than 100.
 */
void AALSBaseCharacter::OnJumped_Implementation() {
    Super::OnJumped_Implementation();
    InAirRotation = Speed > 100.f ? LastVelocityRotation : GetActorRotation();
    if (IALSAnimation* ALSAnimation = Cast<IALSAnimation>(MainAnimInstance)) {
        ALSAnimation->Jumped();
    }
}

// ================================================================================
// IALSCharacter

/*
 * These functions are a great place to perform logic based on the previous and new state,
 * as they should be called whenever a state changes (as long as you use the Interface Event to change the state).
 */
void AALSBaseCharacter::SetMovementState(EALSMovementState NewMovementState) {
    if (NewMovementState != MovementState) {
        OnMovementStateChanged(NewMovementState);
    }
}

void AALSBaseCharacter::SetMovementAction(EALSMovementAction NewMovementAction) {
    if (NewMovementAction != MovementAction) {
        OnMovementActionChanged(NewMovementAction);
    }
}

void AALSBaseCharacter::SetRotationMode(EALSRotationMode NewRotationMode) {
    if (NewRotationMode != RotationMode) {
        OnRotationModeChanged(NewRotationMode);
    }
}

void AALSBaseCharacter::SetGait(EALSGait NewGait) {
    if (NewGait != Gait) {
        OnGaitChanged(NewGait);
    }
}

void AALSBaseCharacter::SetViewMode(EALSViewMode NewViewMode) {
    if (NewViewMode != ViewMode) {
        OnViewModeChanged(NewViewMode);
    }
}

void AALSBaseCharacter::SetOverlayState(EALSOverlayState NewOverlayState) {
    if (NewOverlayState != OverlayState) {
        OnOverlayStateChanged(NewOverlayState);
    }
}

void AALSBaseCharacter::GetEssentialValues(FVector& OutVelocity, FVector& OutAcceleration, FVector& OutMovementInput, bool& OutIsMoving, bool& OutHasMovementInput, float& OutSpeed, float& OutMovementInputAmount, FRotator& OutAimingRotation, float& OutAimYawRate) {
    OutVelocity            = GetVelocity();
    OutAcceleration        = Acceleration;
    OutMovementInput       = GetCharacterMovement()->GetCurrentAcceleration();
    OutIsMoving            = IsMoving;
    OutHasMovementInput    = HasMovementInput;
    OutSpeed               = Speed;
    OutMovementInputAmount = MovementInputAmount;
    OutAimingRotation      = GetControlRotation();
    OutAimYawRate          = AimYawRate;
}

void AALSBaseCharacter::GetCurrentStates(TEnumAsByte<EMovementMode>& OutPawnMovementMode, EALSMovementState& OutMovementState, EALSMovementState& OutPrevMovementState, EALSMovementAction& OutMovementAction, EALSRotationMode& OutRotationMode, EALSGait& OutActualGait, EALSStance& OutActualStance, EALSViewMode& OutViewMode, EALSOverlayState& OutOverlayState) {
    OutPawnMovementMode  = GetCharacterMovement()->MovementMode;
    OutMovementState     = MovementState;
    OutPrevMovementState = PrevMovementState;
    OutMovementAction    = MovementAction;
    OutRotationMode      = RotationMode;
    OutActualGait        = Gait;
    OutActualStance      = Stance;
    OutViewMode          = ViewMode;
    OutOverlayState      = OverlayState;
}

// ================================================================================
// IALSCamera
void AALSBaseCharacter::Get3PTraceParams(FVector& OutTraceOrigin, float& OutTraceRadius, TEnumAsByte<ETraceTypeQuery>& OutTraceChannel) {
    OutTraceOrigin  = GetActorLocation();
    OutTraceRadius  = 10.f;
    OutTraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
}

FTransform AALSBaseCharacter::Get3PPivotTarget() {
    return GetActorTransform();
}

FVector AALSBaseCharacter::GetFPCameraTarget() {
    return GetMesh()->GetSocketLocation(FName("FP_Camera"));
}

void AALSBaseCharacter::GetCameraParameters(float& OutTPFOV, float& OutFPFOV, bool& OutRightShoulder) {
    OutTPFOV         = ThirdPersonFOV;
    OutFPFOV         = FirstPersonFOV;
    OutRightShoulder = RightShoulder;
}

// ================================================================================
// Components
void AALSBaseCharacter::OnTimelineComponentPostUpdate() {
    TSet<UCurveBase*> OutCurves;
    MantleTimeline->GetAllCurves(OutCurves);
    MantleUpdate(Cast<UCurveFloat>(OutCurves.Array()[0])->GetFloatValue(MantleTimeline->GetPlaybackPosition()));
}

void AALSBaseCharacter::OnTimelineComponentFinished() {
    MantleEnd();
}

// ================================================================================
// Utility
void AALSBaseCharacter::GetControlForwardRightVector(FVector& ForwardVector, FVector& RightVector) {
    ForwardVector = UKismetMathLibrary::GetForwardVector(FRotator{0.f, GetControlRotation().Yaw, 0.f});
    RightVector   = UKismetMathLibrary::GetRightVector(FRotator{0.f, GetControlRotation().Yaw, 0.f});
}

FVector AALSBaseCharacter::GetCalpsuleBaseLocation(float ZOffset) {
    UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
    return (CapsuleComp->GetComponentLocation() - CapsuleComp->GetUpVector() * (CapsuleComp->GetScaledCapsuleHalfHeight() + ZOffset));
}

FVector AALSBaseCharacter::GetCapsuleLocationFromBase(FVector BaseLocation, float ZOffset) {
    return BaseLocation + FVector{0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + ZOffset};
}

float AALSBaseCharacter::GetAnimCurveValue(FName CurveName) {
    return MainAnimInstance ? MainAnimInstance->GetCurveValue(CurveName) : 0.f;
}

// ================================================================================
// Essential Information

/*
 * These values represent how the capsule is moving as well as how it wants to move,
 * and therefore are essential for any data driven animation system.
 * They are also used throughout the system for various functions,
 * so I found it is easiest to manage them all in one place.
 */
void AALSBaseCharacter::SetEssentialValues() {
    // Set the amount of Acceleration.
    Acceleration = CalculateAcceleration();
    // Determine if the character is moving by getting it's speed.
    // The Speed equals the length of the horizontal (x y) velocity,
    // so it does not take vertical movement into account.
    // If the character is moving, update the last velocity rotation.
    // This value is saved because it might be useful to know the last orientation of movement even after the character has stopped.
    Speed    = FVector{GetVelocity().X, GetVelocity().Y, 0.f}.Size();
    IsMoving = Speed > 1.f;
    if (IsMoving) {
        LastVelocityRotation = GetVelocity().ToOrientationRotator();
    }
    // Determine if the character has movement input by getting its movement input amount.
    // The Movement Input Amount is equal to the current acceleration divided by the max acceleration so that it has a range of 0-1,
    // 1 being the maximum possible amount of input, and 0 beiung none.
    // If the character has movement input, update the Last Movement Input Rotation.
    MovementInputAmount = (GetCharacterMovement()->GetCurrentAcceleration()).Size() / GetCharacterMovement()->GetMaxAcceleration();
    HasMovementInput    = MovementInputAmount > 0.f;
    if (HasMovementInput) {
        LastMovementInputRotation = (GetCharacterMovement()->GetCurrentAcceleration()).ToOrientationRotator();
    }
    // Set the Aim Yaw rate by comparing the current and previous Aim Yaw value, divided by Delta Seconds.
    // This represents the speed the camera is rotating left to right.
    AimYawRate = FMath::Abs((GetControlRotation().Yaw - PreviousAimYaw) / UGameplayStatics::GetWorldDeltaSeconds(this));
}

/*
 * Cache certain values to be used in calculations on the next frame
 */
void AALSBaseCharacter::CacheValues() {
    PreviousVelocity = GetVelocity();
    PreviousAimYaw   = GetControlRotation().Yaw;
}

/*
 * Calculate the Acceleration by comparing the current and previous velocity.
 * The Current Acceleration returned by the movement component equals the input acceleration,
 * and does not represent the actual physical accelration of the character.
 */
FVector AALSBaseCharacter::CalculateAcceleration() {
    return (GetVelocity() - PreviousVelocity) / UGameplayStatics::GetWorldDeltaSeconds(this);
}

// ================================================================================
// State Changes
void AALSBaseCharacter::OnBeginPlay() {
    // Make sure the mesh and animbp update after the CharacterBP to ensure it gets the most recent values.
    GetMesh()->AddTickPrerequisiteActor(this);
    // Set Reference to the Main Anim Instance.
    if (GetMesh()->GetAnimInstance()) {
        MainAnimInstance = GetMesh()->GetAnimInstance();
    }
    // Set the Movement Model
    SetMovementModel();
    // Update states to use the initial desired values.
    OnGaitChanged(DesiredGait);
    OnRotationModeChanged(DesiredRotationMode);
    OnViewModeChanged(ViewMode);
    OnOverlayStateChanged(OverlayState);
    switch (DesiredStance) {
    case EALSStance::EAS_Standing:
        UnCrouch();
        break;
    case EALSStance::EAS_Crouching:
        Crouch();
        break;
    }
    // Set default rotation values.
    TargetRotation            = GetActorRotation();
    LastVelocityRotation      = GetActorRotation();
    LastMovementInputRotation = GetActorRotation();
}

/*
 * Use the Character Movement Mode changes to set the Movement States to the right values.
 * This allows you to have a custom set of movement states but still use the functionality of the default character movement component.
 */
void AALSBaseCharacter::OnCharacterMovementModeChanged(TEnumAsByte<EMovementMode> PrevMovementMode, TEnumAsByte<EMovementMode> NewMovementMode, uint8 PrevCustomMode) {
    switch (NewMovementMode) {
    case EMovementMode::MOVE_Walking:
    case EMovementMode::MOVE_NavWalking:
        SetMovementState(EALSMovementState::EAMS_Grounded);
        break;
    case EMovementMode::MOVE_Falling:
        SetMovementState(EALSMovementState::EAMS_InAir);
        break;
    }
}

void AALSBaseCharacter::OnMovementStateChanged(EALSMovementState NewMovementState) {
    EALSMovementState PreviousMovementState = EALSMovementState::EAMS_None;
    SetPreviousAndNewValue(NewMovementState, MovementState, PreviousMovementState);
    switch (MovementState) {
    case EALSMovementState::EAMS_InAir:
        // If the character enters the air, set the In Air Rotation and uncrouch if crouched.
        // If the character is currently rolling, enable the ragdoll.
        switch (MovementAction) {
        case EALSMovementAction::EAMA_None:
            InAirRotation = GetActorRotation();
            if (Stance == EALSStance::EAS_Crouching) {
                UnCrouch();
            }
            break;
        case EALSMovementAction::EAMA_Rolling:
            RagdollStart();
            break;
        }
    case EALSMovementState::EAMS_Ragdoll:
        // Stop the Mantle Timeline if transitioning to the ragdoll state while mantling.
        if (PreviousMovementState == EALSMovementState::EAMS_Mantling) {
            MantleTimeline->Stop();
        }
        break;
    }
}

void AALSBaseCharacter::OnMovementActionChanged(EALSMovementAction NewMovementAction) {
    EALSMovementAction PreviousMovementAction = EALSMovementAction::EAMA_None;
    SetPreviousAndNewValue(NewMovementAction, MovementAction, PreviousMovementAction);
    // Make the character crouch if performing a roll.
    if (MovementAction == EALSMovementAction::EAMA_Rolling) {
        UnCrouch();
    }
    // Upon ending a roll, reset the stance back to its desired value.
    if (PreviousMovementAction == EALSMovementAction::EAMA_Rolling) {
        switch (DesiredStance) {
        case EALSStance::EAS_Standing:
            UnCrouch();
            break;
        case EALSStance::EAS_Crouching:
            Crouch();
            break;
        }
    }
}

void AALSBaseCharacter::OnStanceChanged(EALSStance NewStance) {
    EALSStance PreviousStance = EALSStance::EAS_Standing;
    SetPreviousAndNewValue(NewStance, Stance, PreviousStance);
}

void AALSBaseCharacter::OnRotationModeChanged(EALSRotationMode NewRotationMode) {
    EALSRotationMode PreviousRotationMode = EALSRotationMode::EARM_VelocityDirection;
    SetPreviousAndNewValue(NewRotationMode, RotationMode, PreviousRotationMode);
    // If the new rotation mode is Velocity Direction and the character is in First Person, set the viewmode to Third Person.
    if (RotationMode == EALSRotationMode::EARM_VelocityDirection) {
        if (ViewMode == EALSViewMode::EAVM_FirstPerson) {
            SetViewMode(EALSViewMode::EAVM_ThirdPerson);
        }
    }
}

void AALSBaseCharacter::OnGaitChanged(EALSGait NewActualGait) {
    EALSGait PreviousActualGait = EALSGait::EAG_Walking;
    SetPreviousAndNewValue(NewActualGait, Gait, PreviousActualGait);
}

void AALSBaseCharacter::OnViewModeChanged(EALSViewMode NewViewMode) {
    EALSViewMode PreviousViewMode = EALSViewMode::EAVM_ThirdPerson;
    SetPreviousAndNewValue(NewViewMode, ViewMode, PreviousViewMode);

    switch (ViewMode) {
    case EALSViewMode::EAVM_ThirdPerson:
        // If Third Person, set the rotation mode back to the desired mode.
        if (RotationMode == EALSRotationMode::EARM_VelocityDirection || RotationMode == EALSRotationMode::EARM_LookingDirection) {
            SetRotationMode(DesiredRotationMode);
        }
        break;
    case EALSViewMode::EAVM_FirstPerson:
        // If First Person, set the rotation mode to looking direction if currently in the velocity direction mode.
        if (RotationMode == EALSRotationMode::EARM_VelocityDirection) {
            SetRotationMode(EALSRotationMode::EARM_LookingDirection);
        }
        break;
    }
}

void AALSBaseCharacter::OnOverlayStateChanged(EALSOverlayState NewOverlayState) {
    EALSOverlayState PreviousOverlayState = EALSOverlayState::EAOS_Default;
    SetPreviousAndNewValue(NewOverlayState, OverlayState, PreviousOverlayState);
}

// ================================================================================
// Movement System
void AALSBaseCharacter::SetMovementModel() {
    FMovementSettingsState* MovementDataPtr = MovementModel.DataTable->FindRow<FMovementSettingsState>(MovementModel.RowName, "");
    MovementData                            = *MovementDataPtr;
}

void AALSBaseCharacter::UpdateCharacterMovement() {
    // Set the Allowed Gait
    EALSGait AllowedGait = GetAllowedGait();
    // Determine the Actual Gait. If it is different from the current Gait,
    // Set the new Gait Event.
    EALSGait ActualGait = GetActualGait(AllowedGait);
    if (ActualGait != Gait) {
        SetGait(ActualGait);
    }
    // Use the allowed gait to update the movement settings.
    UpdateDynamicMovementSettings(AllowedGait);
}

void AALSBaseCharacter::UpdateDynamicMovementSettings(EALSGait AllowedGait) {
    // Step 1: Get the Current Movement Settings.
    CurrentMovementSettings = GetTargetMovementSettings();
    // Update the Character Max Walk Speed to the configured speeds based on the currently Allowed Gait.
    switch (AllowedGait) {
    case EALSGait::EAG_Walking:
        GetCharacterMovement()->MaxWalkSpeed         = CurrentMovementSettings.WalkSpeed;
        GetCharacterMovement()->MaxWalkSpeedCrouched = CurrentMovementSettings.WalkSpeed;
        break;
    case EALSGait::EAG_Running:
        GetCharacterMovement()->MaxWalkSpeed         = CurrentMovementSettings.RunSpeed;
        GetCharacterMovement()->MaxWalkSpeedCrouched = CurrentMovementSettings.RunSpeed;
        break;
    case EALSGait::EAG_Sprinting:
        GetCharacterMovement()->MaxWalkSpeed         = CurrentMovementSettings.SprintSpeed;
        GetCharacterMovement()->MaxWalkSpeedCrouched = CurrentMovementSettings.SprintSpeed;
        break;
    }
    // Update the Acceleration, Deceleration, and Ground Friction using the Movement Curve.
    // This allows for fine control over movement behavior at each speed (May not be suitable for replication).
    FVector V                                          = CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed());
    GetCharacterMovement()->MaxAcceleration            = V.X;
    GetCharacterMovement()->BrakingDecelerationWalking = V.Y;
    GetCharacterMovement()->GroundFriction             = V.Z;
}

FMovementSettings AALSBaseCharacter::GetTargetMovementSettings() {
    switch (RotationMode) {
    case EALSRotationMode::EARM_VelocityDirection:
        switch (Stance) {
        case EALSStance::EAS_Standing:
            return MovementData.VelocityDirection.Standing;
        case EALSStance::EAS_Crouching:
            return MovementData.VelocityDirection.Crouching;
        }
        break;
    case EALSRotationMode::EARM_LookingDirection:
        switch (Stance) {
        case EALSStance::EAS_Standing:
            return MovementData.LookingDirection.Standing;
        case EALSStance::EAS_Crouching:
            return MovementData.LookingDirection.Crouching;
        }
        break;
    case EALSRotationMode::EARM_Aiming:
        switch (Stance) {
        case EALSStance::EAS_Standing:
            return MovementData.Aiming.Standing;
        case EALSStance::EAS_Crouching:
            return MovementData.Aiming.Crouching;
        }
        break;
    }
    return FMovementSettings{};
}

/*
 * Calculate the Allowed Gait.
 * This represents the maximum Gait the character is currently allowed to be in,
 * and can be determined by the desired gait, the rotation mode, the stance, etc.
 * For example, if you wanted to force the character into a walking state while indoors, this could be done here.
 */
EALSGait AALSBaseCharacter::GetAllowedGait() {
    if (Stance == EALSStance::EAS_Crouching || RotationMode == EALSRotationMode::EARM_Aiming) {
        switch (DesiredGait) {
        case EALSGait::EAG_Walking:
            return EALSGait::EAG_Walking;
        case EALSGait::EAG_Running:
        case EALSGait::EAG_Sprinting:
            return EALSGait::EAG_Running;
        }
    } else {
        switch (DesiredGait) {
        case EALSGait::EAG_Walking:
            return EALSGait::EAG_Walking;
        case EALSGait::EAG_Running:
            return EALSGait::EAG_Running;
        case EALSGait::EAG_Sprinting:
            return CanSprint() ? EALSGait::EAG_Sprinting : EALSGait::EAG_Running;
        }
    }
    return EALSGait::EAG_Walking;
}

/*
 * Get the Actual Gait. This is calculated by the actual movement of the character,
 * and so it can be different from the desired gait or allowed gait.
 * For instance, if the Allowed Gait becomes walking,
 * the Actual gait will still be running untill the character decelerates to the walking speed.
 */
EALSGait AALSBaseCharacter::GetActualGait(EALSGait AllowedGait) {
    float LocalWalkSpeed   = CurrentMovementSettings.WalkSpeed;
    float LocalRunSpeed    = CurrentMovementSettings.RunSpeed;
    float LocalSprintSpeed = CurrentMovementSettings.SprintSpeed;
    if (Speed >= LocalRunSpeed + 10.f) {
        switch (AllowedGait) {
        case EALSGait::EAG_Walking:
        case EALSGait::EAG_Running:
            return EALSGait::EAG_Running;
        case EALSGait::EAG_Sprinting:
            return EALSGait::EAG_Sprinting;
        }
    } else if (Speed >= LocalWalkSpeed + 10.f) {
        return EALSGait::EAG_Running;
    } else {
        return EALSGait::EAG_Walking;
    }
    return EALSGait::EAG_Running;
}

/*
 * Determine if the character is currently able to sprint based on the Rotation mode
 * and current acceleration (input) rotation.
 * If the character is in the Looking Rotation mode,
 * only allow sprinting if there is full movement input
 * and it is faced forward relative to the camera + or - 50 degrees.
 */
bool AALSBaseCharacter::CanSprint() {
    if (!HasMovementInput) {
        return false;
    }
    FRotator Rotator = (GetCharacterMovement()->GetCurrentAcceleration()).ToOrientationRotator();
    float Yaw        = UKismetMathLibrary::NormalizedDeltaRotator(Rotator, GetControlRotation()).Yaw;
    switch (RotationMode) {
    case EALSRotationMode::EARM_VelocityDirection:
        return MovementInputAmount > 0.9f;
    case EALSRotationMode::EARM_LookingDirection:
        return MovementInputAmount > 0.9f && FMath::Abs(Yaw) < 50.f;
    case EALSRotationMode::EARM_Aiming:
        return false;
    }
    return false;
}

/*
 * Map the character's current speed to the configured movement speeds with a range of 0-3,
 * with 0 = stopped, 1 = the Walk Speed, 2 = the Run Speed, and 3 = the Sprint Speed.
 * This allows you to vary the movement speeds but still use the mapped range in calculations for consistent results.
 */
float AALSBaseCharacter::GetMappedSpeed() {
    float LocalWalkSpeed   = CurrentMovementSettings.WalkSpeed;
    float LocalRunSpeed    = CurrentMovementSettings.RunSpeed;
    float LocalSprintSpeed = CurrentMovementSettings.SprintSpeed;
    if (Speed > LocalRunSpeed) {
        return UKismetMathLibrary::MapRangeClamped(Speed, LocalRunSpeed, LocalSprintSpeed, 2.f, 3.f);
    }
    if (Speed > LocalWalkSpeed) {
        return UKismetMathLibrary::MapRangeClamped(Speed, LocalWalkSpeed, LocalRunSpeed, 1.f, 2.f);
    }
    return UKismetMathLibrary::MapRangeClamped(Speed, 0.f, LocalWalkSpeed, 0.f, 1.f);
}

/*
 * This gets overriden in the AnimMan Child character to select the appropriate animation based on the overlay state.
 */
UAnimMontage* AALSBaseCharacter::GetRollAnimation() {
    return nullptr;
}

// ================================================================================
// Rotation System
void AALSBaseCharacter::UpdateGroudedRotation() {
    switch (MovementAction) {
    case EALSMovementAction::EAMA_None:
        if (CanUpdateMovingRotation()) {
            switch (RotationMode) {
            case EALSRotationMode::EARM_VelocityDirection:
                // Velocity Direction Rotation
                SmoothCharacterRotation(FRotator{0.f, LastVelocityRotation.Yaw, 0.f}, 800.f, CalculateGroundedRotationRate());
                break;
            case EALSRotationMode::EARM_LookingDirection:
                // Looking Direction Rotation
                if (Gait == EALSGait::EAG_Sprinting) {
                    SmoothCharacterRotation(FRotator{0.f, LastVelocityRotation.Yaw, 0.f}, 500.f, CalculateGroundedRotationRate());
                } else {
                    SmoothCharacterRotation(FRotator{0.f, GetControlRotation().Yaw + GetAnimCurveValue(FName("YawOffset")), 0.f}, 500.f, CalculateGroundedRotationRate());
                }
                break;
            case EALSRotationMode::EARM_Aiming:
                // Aiming Rotation
                SmoothCharacterRotation(FRotator{0.f, GetControlRotation().Yaw, 0.f}, 1000.f, 20.f);
                break;
            }
        } else {
            // Not Moving
            if (ViewMode == EALSViewMode::EAVM_FirstPerson || RotationMode == EALSRotationMode::EARM_Aiming) {
                LimitRotation(-100.f, 100.f, 20.f);
            }
            // Apply the RotationAmount curve from Turn In Place Animations.
            // The Rotation Amount curve defines how much rotation should be applied each frame,
            // and is calculated for animations that are animated at 30fps.
            float RotationAmount = GetAnimCurveValue(FName("RotationAmount"));
            if (FMath::Abs(RotationAmount) > 0.001f) {
                AddActorWorldRotation(FRotator{0.f, RotationAmount * UGameplayStatics::GetWorldDeltaSeconds(this) / (1.f / 30.f), 0.f});
                TargetRotation = GetActorRotation();
            }
        }
        break;
    case EALSMovementAction::EAMA_Rolling:
        if (HasMovementInput) {
            // Rolling Rotation
            SmoothCharacterRotation(FRotator{0.f, LastMovementInputRotation.Yaw, 0.f}, 0.f, 2.f);
        }
        break;
    }
}

void AALSBaseCharacter::UpdateInAirRotation() {
    switch (RotationMode) {
    case EALSRotationMode::EARM_VelocityDirection:
    case EALSRotationMode::EARM_LookingDirection:
        // Velocity / Looking Direction Rotation
        SmoothCharacterRotation(FRotator{0.f, InAirRotation.Yaw, 0.f}, 0.f, 5.f);
        break;
    case EALSRotationMode::EARM_Aiming:
        // Aiming Rotation
        SmoothCharacterRotation(FRotator{0.f, GetControlRotation().Yaw, 0.f}, 0.f, 15.f);
        InAirRotation = GetActorRotation();
        break;
    }
}

/*
 * Interpolate the Target Rotation for extra smooth rotation behavior
 */
void AALSBaseCharacter::SmoothCharacterRotation(FRotator Target, float TargetInterpSpeedConst, float ActorInterpSpeedSmooth) {
    TargetRotation = UKismetMathLibrary::RInterpTo_Constant(TargetRotation, Target, UGameplayStatics::GetWorldDeltaSeconds(this), TargetInterpSpeedConst);
    SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRotation, UGameplayStatics::GetWorldDeltaSeconds(this), ActorInterpSpeedSmooth));
}

void AALSBaseCharacter::AddToCharacterRotation(FRotator DeltaRotation) {
    TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, DeltaRotation);
    AddActorWorldRotation(DeltaRotation);
};

/*
 * Prevent the character from rotating past a certain angle.
 */
void AALSBaseCharacter::LimitRotation(float AimYawMin, float AimYawMax, float InterpSpeed) {
    float Yaw = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation()).Yaw;
    if (!UKismetMathLibrary::InRange_FloatFloat(Yaw, AimYawMin, AimYawMax, true, true)) {
        float RotationYaw = GetControlRotation().Yaw + Yaw > 0.f ? AimYawMin : AimYawMax;
        SmoothCharacterRotation(FRotator{0.f, RotationYaw, 0.f}, 0.f, InterpSpeed);
    }
}

/*
 * Update the Actors Location and Rotation as well as the Target Rotation variable to keep everything in sync.
 */
bool AALSBaseCharacter::SetActorLocationAndRotationUpdateTarget(FVector NewLocation, FRotator NewRotation, bool bSweep, bool bTeleport, FHitResult& SweepHitResult) {
    TargetRotation = NewRotation;
    return SetActorLocationAndRotation(NewLocation, TargetRotation, bSweep, &SweepHitResult);
};

/*
 * Calculate the rotation rate by using the current Rotation Rate Curve in the Movement Settings.
 * Using the curve in conjunction with the mapped speed
 * gives you a high level of control over the rotation rates for each speed.
 * Increase the speed if the camera is rotating quickly for more responsive rotation.
 */
float AALSBaseCharacter::CalculateGroundedRotationRate() {
    return CurrentMovementSettings.RotationRateCurve->GetFloatValue(GetMappedSpeed()) * UKismetMathLibrary::MapRangeClamped(AimYawRate, 0.f, 300.f, 1.f, 3.f);
}

bool AALSBaseCharacter::CanUpdateMovingRotation() {
    return ((IsMoving && HasMovementInput) || Speed > 150.f) && !HasAnyRootMotion();
}

// ================================================================================
// Mantle System
bool AALSBaseCharacter::MantleCheck(FMantleTraceSettings TraceSettings, TEnumAsByte<EDrawDebugTrace::Type> DebugType) {
    FVector InitialTraceImpactPoint   = FVector::ZeroVector;
    FVector InitialTraceNormal        = FVector::ZeroVector;
    FVector DownTraceLocation         = FVector::ZeroVector;
    UPrimitiveComponent* HitComponent = nullptr;
    FTransform TargetTransform        = {FRotator::ZeroRotator, FVector::ZeroVector, FVector{1.f}};
    float MantleHeight                = 0.f;
    EMantleType MantleType            = EMantleType::EMT_HighMantle;

    // Step 1: Trace forward to find a wall / object the character cannot walk on.
    FVector Start = GetCalpsuleBaseLocation(2.f) + (GetPlayerMovementInput() * -30.f) +
                    FVector{0.f, 0.f, (TraceSettings.MaxLedgeHeight + TraceSettings.MinLedgeHeight) / 2.f};
    FVector End                  = Start + GetPlayerMovementInput() * TraceSettings.ReachDistance;
    float HalfHeight             = (TraceSettings.MaxLedgeHeight - TraceSettings.MinLedgeHeight) / 2.f + 1.f;
    ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2);
    const TArray<AActor*> ActorsToIgnore;
    FHitResult CapsuleTraceHit;
    bool CapsuleTraceSingle = UKismetSystemLibrary::CapsuleTraceSingle(
        this,                             // const UObject* WorldContextObject
        Start,                            // const FVector Start
        End,                              // const FVector End
        TraceSettings.ForwardTraceRadius, // float Radius
        HalfHeight,                       // float HalfHeight
        TraceChannel,                     // ETraceTypeQuery TraceChannel
        false,                            // bool bTraceComplex
        ActorsToIgnore,                   // const TArray<AActor*>& ActorsToIgnore
        GetTraceDebugType(DebugType),     // EDrawDebugTrace::Type DrawDebugType
        CapsuleTraceHit,                  // FHitResult& OutHit
        true,                             // bool bIgnoreSelf
        FLinearColor::Black,              // FLinearColor TraceColor = FLinearColor::Red
        FLinearColor::Black,              // FLinearColor TraceHitColor = FLinearColor::Green
        1.f                               // float DrawTime = 5.0f
    );
    if (!GetCharacterMovement()->IsWalkable(CapsuleTraceHit) && CapsuleTraceHit.bBlockingHit && !CapsuleTraceHit.bStartPenetrating) {
        InitialTraceImpactPoint = CapsuleTraceHit.ImpactPoint;
        InitialTraceNormal      = CapsuleTraceHit.ImpactNormal;
    } else {
        return false;
    }

    // Step 2: Trace downward from the first trace's Impact Point and determine if the hit location is walkable.
    End = FVector{InitialTraceImpactPoint.X, InitialTraceImpactPoint.Y, GetCalpsuleBaseLocation(2.f).Z} +
          InitialTraceNormal * -15.f;
    Start = End + FVector{0.f, 0.f, TraceSettings.MaxLedgeHeight + TraceSettings.DownwardTraceRadius + 1.f};
    FHitResult SphereTraceHit;
    bool SphereTraceSingle = UKismetSystemLibrary::SphereTraceSingle(
        this,                                         // const UObject* WorldContextObject
        Start,                                        // const FVector Start
        End,                                          // const FVector End
        TraceSettings.DownwardTraceRadius,            // float Radius
        TraceChannel,                                 // ETraceTypeQuery TraceChannel
        false,                                        // bool bTraceComplex
        ActorsToIgnore,                               // const TArray<AActor*>& ActorsToIgnore
        GetTraceDebugType(DebugType),                 // EDrawDebugTrace::Type DrawDebugType
        SphereTraceHit,                               // FHitResult& OutHit
        true,                                         // bool bIgnoreSelf
        FLinearColor{0.896269f, 0.388383f, 0.f, 1.f}, // FLinearColor TraceColor = FLinearColor::Red
        FLinearColor{1.f, 0.f, 0.049349f, 1.f},       // FLinearColor TraceHitColor = FLinearColor::Green
        1.f                                           // float DrawTime = 5.0f
    );
    if (GetCharacterMovement()->IsWalkable(SphereTraceHit) && SphereTraceHit.bBlockingHit) {
        DownTraceLocation = FVector{SphereTraceHit.Location.X, SphereTraceHit.Location.Y, SphereTraceHit.ImpactPoint.Z};
        HitComponent      = SphereTraceHit.GetComponent();
    } else {
        return false;
    }

    // Step 3: Check if the capsule has room to stand at the downward trace's location.
    // If so, set that location as the Target Transform and calculate the mantle height.
    FVector CapsuleLocationFromBase = GetCapsuleLocationFromBase(DownTraceLocation, 2.f);
    bool HasRoom                    = CapsuleHasRoomCheck(GetCapsuleComponent(), CapsuleLocationFromBase, 0.f, 0.f, GetTraceDebugType(DebugType));
    if (HasRoom) {
        FRotator Rotator = (InitialTraceNormal * FVector{-1.f, -1.f, 0.f}).ToOrientationRotator();
        TargetTransform  = FTransform{Rotator, CapsuleLocationFromBase, FVector{1.f}};
        MantleHeight     = (TargetTransform.GetLocation() - GetActorLocation()).Z;
    } else {
        return false;
    }

    // Step 4: Determine the Mantle Type by checking the movement mode and Mantle Height.
    if (MovementState == EALSMovementState::EAMS_InAir) {
        MantleType = EMantleType::EMT_FallingCatch;
    } else {
        if (MantleHeight > 125.f) {
            MantleType = EMantleType::EMT_HighMantle;
        } else {
            MantleType = EMantleType::EMT_LowMantle;
        }
    }

    // Step 5: If everything checks out, start the Mantle
    MantleStart(MantleHeight, FALSComponentAndTransform{TargetTransform, HitComponent}, MantleType);
    return true;
}

void AALSBaseCharacter::MantleStart(float MantleHeight, FALSComponentAndTransform MantleLedgeWS, EMantleType MantleType) {
    // Step 1: Convert the world space target to the mantle component's local space for use in moving objects.
    MantleLedgeLS = ComponentWorldToLocal(MantleLedgeWS);

    // Step 2: Get the Mantle Asset and use it to set the new Mantle Params.
    FMantleAsset MantleAsset = GetMantleAsset(MantleType);
    MantleParams             = {
        MantleAsset.AnimMontage,
        MantleAsset.PositionCorrectionCurve,
        (float)UKismetMathLibrary::MapRangeClamped(MantleHeight, MantleAsset.LowHeight, MantleAsset.HighHeight, MantleAsset.LowStartPosition, MantleAsset.HighStartPosition),
        (float)UKismetMathLibrary::MapRangeClamped(MantleHeight, MantleAsset.LowHeight, MantleAsset.HighHeight, MantleAsset.LowPlayRate, MantleAsset.HighPlayRate),
        MantleAsset.StartingOffset};

    // Step 3: Set the Mantle Target and calculate the Starting Offset (offset amount between the actor and target transform).
    MantleTarget            = MantleLedgeWS.Transform;
    MantleActualStartOffset = TransformSubtractionTransform(GetActorTransform(), MantleTarget);

    // Step 4: Calculate the Animated Start Offset from the Target Location.
    // This would be the location the actual animation starts at relative to the Target Transform.
    FVector V    = (MantleTarget.GetRotation().Rotator()).Vector() * MantleParams.StartingOffset.Y;
    FTransform T = FTransform{
        MantleTarget.GetRotation().Rotator(),
        MantleTarget.GetLocation() - FVector{V.X, V.Y, MantleParams.StartingOffset.Z},
        FVector{1.f}};
    MantleAnimatedStartOffset = TransformSubtractionTransform(T, MantleTarget);

    // Step 5: Clear the Character Movement Mode and set the Movement State to Mantling
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None, 0);
    SetMovementState(EALSMovementState::EAMS_Mantling);

    // Step 6: Configure the Mantle Timeline so that it is the same length as the Lerp/Correction curve minus the starting position,
    // and plays at the same speed as the animation. Then start the timeline.
    float MinTime;
    float MaxTime;
    MantleParams.PositionCorrectionCurve->GetTimeRange(MinTime, MaxTime);
    MantleTimeline->SetTimelineLength(MaxTime - MantleParams.StartingPosition);
    MantleTimeline->SetPlayRate(MantleParams.PlayRate);
    MantleTimeline->PlayFromStart();

    // Step 7: Play the Anim Montaget if valid.
    if (MantleParams.AnimMontage && MainAnimInstance) {
        MainAnimInstance->Montage_Play(
            MantleParams.AnimMontage,              // UAnimMontage* MontageToPlay
            MantleParams.PlayRate,                 // float InPlayRate = 1.f
            EMontagePlayReturnType::MontageLength, // EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength
            MantleParams.StartingPosition,         // float InTimeToStartMontageAt=0.f
            false                                  // bool bStopAllMontages = true
        );
    }
};

void AALSBaseCharacter::MantleEnd() {
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking, 0);
}

void AALSBaseCharacter::MantleUpdate(float BlendIn) {
    // Step 1: Continually update the mantle target from the stored local transform to follow along with moving objects.
    FALSComponentAndTransform CT = ComponentLocalToWorld(MantleLedgeLS);
    MantleTarget                 = CT.Transform;

    // Step 2: Update the Position and Correction Alphas using the Position/Correction curve set for each Mantle.
    FVector V               = MantleParams.PositionCorrectionCurve->GetVectorValue(MantleParams.StartingPosition + MantleTimeline->GetPlaybackPosition());
    float PositionAlpha     = V.X;
    float XYCorrectionAlpha = V.Y;
    float ZCorrectionAlpha  = V.Z;

    // Step 3: Lerp multiple transforms together for independent control over the horizontal and vertical blend to the animated start position, as well as the target position.
    // Blend into the animated horizontal and rotation offset using the Y value of the Position/Correction Curve.
    FTransform T1{
        MantleAnimatedStartOffset.GetRotation().Rotator(),
        FVector{MantleAnimatedStartOffset.GetLocation().X, MantleAnimatedStartOffset.GetLocation().Y, MantleActualStartOffset.GetLocation().Z},
        FVector{1.f}};
    FTransform TA = UKismetMathLibrary::TLerp(MantleActualStartOffset, T1, XYCorrectionAlpha);
    // Blend into the animated vertical offset using the Z value of the Position/Correction Curve.
    FTransform T2{
        MantleActualStartOffset.GetRotation().Rotator(),
        FVector{MantleActualStartOffset.GetLocation().X, MantleActualStartOffset.GetLocation().Y, MantleAnimatedStartOffset.GetLocation().Z},
        FVector{1.f}};
    FTransform TB = UKismetMathLibrary::TLerp(MantleActualStartOffset, T2, ZCorrectionAlpha);
    FTransform TC = FTransform{TA.GetRotation().Rotator(), FVector{TA.GetLocation().X, TA.GetLocation().Y, TB.GetLocation().Z}, FVector{1.f}};
    // Blend from the currently blending transforms into the final mantle target using the X value of the Position/Correction Curve.
    FTransform T = UKismetMathLibrary::TLerp(TransformAdditionTransform(MantleTarget, TC), MantleTarget, PositionAlpha);
    // Initial Blend In (controlled in the timeline curve) to allow the actor to blend into the Position/Correction curve at the midoint.
    // This prevents pops when mantling an object lower than the animated mantle.
    FTransform LerpedTarget = UKismetMathLibrary::TLerp(TransformAdditionTransform(MantleTarget, MantleActualStartOffset), T, BlendIn);

    // Step 4: Set the actors location and rotation to the Lerped Target.
    FHitResult SweepHitResult;
    SetActorLocationAndRotationUpdateTarget(LerpedTarget.GetLocation(), LerpedTarget.GetRotation().Rotator(), false, false, SweepHitResult);
}

/*
 * Perform a trace to see if the capsule has room to be at the target location.
 */
bool AALSBaseCharacter::CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset, float RadiusOffset, TEnumAsByte<EDrawDebugTrace::Type> DebugType) {
    float Z = Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere() + RadiusOffset * -1.f + HeightOffset;
    const TArray<AActor*> ActorsToIgnore;
    FHitResult SphereTraceHit;
    UKismetSystemLibrary::SphereTraceSingleByProfile(
        this,                                               // const UObject* WorldContextObject
        TargetLocation + FVector(0.f, 0.f, Z),              // const FVector Start
        TargetLocation - FVector(0.f, 0.f, Z),              // const FVector End
        Capsule->GetUnscaledCapsuleRadius() + RadiusOffset, // float Radius
        FName("ALS_Character"),                             // FName ProfileName
        false,                                              // bool bTraceComplex
        ActorsToIgnore,                                     // const TArray<AActor*>& ActorsToIgnore
        GetTraceDebugType(DebugType),                       // EDrawDebugTrace::Type DrawDebugType
        SphereTraceHit,                                     // FHitResult& OutHit
        true,                                               // bool bIgnoreSelf
        FLinearColor{0.130706f, 0.896269f, 0.144582f, 1.f}, // FLinearColor TraceColor = FLinearColor::Red
        FLinearColor{0.932733f, 0.29136f, 1.f, 1.f},        // FLinearColor TraceHitColor = FLinearColor::Green
        1.f                                                 // float DrawTime = 5.0f
    );
    return !(SphereTraceHit.bBlockingHit || SphereTraceHit.bStartPenetrating);
};

/*
 * Get the Default Mantle Asset values. These will be overriden in the AnimMan Child Character
 */
FMantleAsset AALSBaseCharacter::GetMantleAsset(EMantleType MantleType) {
    switch (MantleType) {
    case EMantleType::EMT_HighMantle:
        return FMantleAsset{
            nullptr,
            LoadObject<UCurveVector>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/MantleCurves/Mantle_1m")),
            FVector{0.f, 65.f, 200.f},
            50.f, 1.f, 0.5f,
            100.f, 1.f, 0.f};
    case EMantleType::EMT_LowMantle:
        return FMantleAsset{
            nullptr,
            LoadObject<UCurveVector>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/MantleCurves/Mantle_2m")),
            FVector{0.f, 65.f, 200.f},
            125.f, 1.2f, 0.6f,
            200.f, 1.2f, 0.f};
    case EMantleType::EMT_FallingCatch:
        return FMantleAsset{
            nullptr,
            LoadObject<UCurveVector>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/MantleCurves/Mantle_2m")),
            FVector{0.f, 65.f, 200.f},
            125.f, 1.2f, 0.6f,
            200.f, 1.2f, 0.f};
    }
    return FMantleAsset{};
};

// ================================================================================
// Ragdoll System
void AALSBaseCharacter::RagdollStart() {
    // Step 1: Clear the Character Movement Mode and set teh Movement State to Ragdoll
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None, 0);
    SetMovementState(EALSMovementState::EAMS_Ragdoll);

    // Step 2: Disable capsule collision and enable mesh physics simulation starting from the pelvis.
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("pelvis"), true, true);

    // Step 3: Stop any active montages.
    MainAnimInstance->Montage_Stop(0.2f);
}

void AALSBaseCharacter::RagdollEnd() {
    // Step 1: Save a snapshot of the current Ragdoll Pose for use in AnimGraph to blend out of the ragdoll
    if (MainAnimInstance) {
        MainAnimInstance->SavePoseSnapshot(FName("RagdollPose"));
    }

    // Step 2: If the ragdoll is on the ground, set the movement mode to walking and play a Get Up animation.
    // If not, set the movement mode to falling and update teh character movement velocity to match the last ragdoll velocity.
    if (RagdollOnGround) {
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking, 0);
        MainAnimInstance->Montage_Play(
            GetGetUpAnimation(RagdollFaceUp),      // UAnimMontage* MontageToPlay
            1.f,                                   // float InPlayRate = 1.f
            EMontagePlayReturnType::MontageLength, // EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength
            0.f,                                   // float InTimeToStartMontageAt=0.f
            true                                   // bool bStopAllMontages = true
        );
    } else {
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling, 0);
        GetCharacterMovement()->Velocity = LastRagdollVelocity;
    }

    // Step 3: Re-Enable capsule collision, and disable physics simulation on the mesh.
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    GetMesh()->SetAllBodiesSimulatePhysics(false);
}

void AALSBaseCharacter::RagdollUpdate() {
    // Set the Last Ragdoll Velocity.
    LastRagdollVelocity = GetMesh()->GetPhysicsLinearVelocity(FName("root"));

    // Use the Ragdoll Velocity to scale the ragdoll's joint strength for physical animation.
    GetMesh()->SetAllMotorsAngularDriveParams(
        UKismetMathLibrary::MapRangeClamped(LastRagdollVelocity.Size(), 0.f, 1000.f, 0.f, 25000.f),
        0.f, 0.f, false);

    // Disable Gravity if falling faster than -4000 to prevent continual acceleration.
    // This also prevents the ragdoll from going through the floor.
    GetMesh()->SetEnableGravity(LastRagdollVelocity.Z > -4000.f);

    // Update the Actor location to follow the ragdoll.
    SetActorLocationDuringRagdoll();
}

void AALSBaseCharacter::SetActorLocationDuringRagdoll() {
    // Set the pelvis as the target location.
    FVector TargetRagdollLocation = GetMesh()->GetSocketLocation(FName("pelvis"));

    // Determine wether the ragdoll is facing up or down and set the target rotation accordingly.
    FRotator PelvisRotation = GetMesh()->GetSocketRotation(FName("pelvis"));
    RagdollFaceUp           = PelvisRotation.Roll < 0.f;
    FRotator TargetRagdollRotation{0.f, RagdollFaceUp ? PelvisRotation.Yaw - 180.f : PelvisRotation.Yaw, 0.f};

    // Trace downward from the target location to offset the target location,
    // preventing the lower half of the capsule from going through the floor when the ragdoll is laying on the ground.
    FVector End{TargetRagdollLocation.X, TargetRagdollLocation.Y, TargetRagdollLocation.Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight()};
    const TArray<AActor*> ActorsToIgnore;
    FHitResult OutHit;
    UKismetSystemLibrary::LineTraceSingle(
        this,                                                                // const UObject* WorldContextObject
        TargetRagdollLocation,                                               // const FVector Start
        End,                                                                 // const FVector End
        UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), // ETraceTypeQuery TraceChannel
        false,                                                               // bool bTraceComplex
        ActorsToIgnore,                                                      // const TArray<AActor*>& ActorsToIgnore
        EDrawDebugTrace::None,                                               // EDrawDebugTrace::Type DrawDebugType
        OutHit,                                                              // FHitResult& OutHit
        true                                                                 // bool bIgnoreSelf
        // FLinearColor TraceColor = FLinearColor::Red
        // FLinearColor TraceHitColor = FLinearColor::Green
        // float DrawTime = 5.0f
    );
    RagdollOnGround = OutHit.bBlockingHit;
    if (RagdollOnGround) {
        FHitResult SweepHitResult;
        float Z = TargetRagdollLocation.Z +
                  GetCapsuleComponent()->GetScaledCapsuleHalfHeight() -
                  FMath::Abs(OutHit.ImpactPoint.Z - OutHit.TraceStart.Z) + 2.f;
        SetActorLocationAndRotationUpdateTarget(FVector{TargetRagdollLocation.X, TargetRagdollLocation.Y, Z},
            TargetRagdollRotation, false, false, SweepHitResult);
    } else {
        FHitResult SweepHitResult;
        SetActorLocationAndRotationUpdateTarget(TargetRagdollLocation, TargetRagdollRotation, false, false, SweepHitResult);
    }
}

UAnimMontage* AALSBaseCharacter::GetGetUpAnimation(bool InRagdollFaceUp) { return nullptr; };
