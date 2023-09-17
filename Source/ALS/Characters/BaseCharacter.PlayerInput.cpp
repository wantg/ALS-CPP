#include "ALS/Characters/BaseCharacter.h"
#include "ALS/Misc/InputMapping.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/KismetMathLibrary.h"
#include "ALS/Libraries/ALSMacroLibrary.h"

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Movement Input
    FInputAxisBinding MoveForwardBackwardsAxisBinding("MoveForward/Backwards");
    MoveForwardBackwardsAxisBinding.AxisDelegate.GetDelegateForManualSet().BindLambda([this](float Scale) {
        if (Scale) {
            PlayerMovementInput(true);
        }
    });
    PlayerInputComponent->AxisBindings.Add(MoveForwardBackwardsAxisBinding);

    FInputAxisBinding MoveRightLeftAxisBinding("MoveRight/Left");
    MoveRightLeftAxisBinding.AxisDelegate.GetDelegateForManualSet().BindLambda([this](float Scale) {
        if (Scale) {
            PlayerMovementInput(false);
        }
    });
    PlayerInputComponent->AxisBindings.Add(MoveRightLeftAxisBinding);

    // Camera Input
    FInputAxisBinding LookUpDownAxisBinding("LookUp/Down");
    LookUpDownAxisBinding.AxisDelegate.GetDelegateForManualSet().BindLambda([this](float Scale) {
        AddControllerPitchInput(LookUpDownRate * Scale);
    });
    PlayerInputComponent->AxisBindings.Add(LookUpDownAxisBinding);

    FInputAxisBinding LookLeftRightAxisBinding("LookLeft/Right");
    LookLeftRightAxisBinding.AxisDelegate.GetDelegateForManualSet().BindLambda([this](float Scale) {
        AddControllerYawInput(LookLeftRightRate * Scale);
    });
    PlayerInputComponent->AxisBindings.Add(LookLeftRightAxisBinding);

    // Jump Action: Press "Jump Action" to end the ragdoll if ragdolling,
    // check for a mantle if grounded or in air, stand up if crouching, or jump if standing.
    AddInputActionBindingLambda(PlayerInputComponent, "JumpAction", IE_Pressed, [this]() {
        if (MovementAction == EALSMovementAction::EAMA_None) {
            switch (MovementState) {
                case EALSMovementState::EAMS_Grounded:
                    if (!HasMovementInput || !MantleCheck(GroundedTraceSettings, EDrawDebugTrace::Type::ForDuration)) {
                        switch (Stance) {
                            case EALSStance::EAS_Standing:
                                Jump();
                                break;
                            case EALSStance::EAS_Crouching:
                                UnCrouch();
                                break;
                        }
                    }
                    break;
                case EALSMovementState::EAMS_InAir:
                    MantleCheck(FallingTraceSettings, EDrawDebugTrace::Type::ForDuration);
                    break;
                case EALSMovementState::EAMS_Ragdoll:
                    RagdollEnd();
                    break;
            }
        }
    });
    PlayerInputComponent->BindAction("JumpAction", IE_Released, this, &ABaseCharacter::StopJumping);

    // Stance Action: Press "Stance Action" to toggle Standing / Crouching, double tap to Roll.
    AddInputActionBindingLambda(PlayerInputComponent, "StanceAction", IE_Pressed, [this]() {
        if (MovementAction == EALSMovementAction::EAMA_None) {
            StancePressedTimes++;
            RetriggerDelay(0.3f, TEXT("ResetStancePressedTimes"));
            if (StancePressedTimes == 1) {
                switch (MovementState) {
                    case EALSMovementState::EAMS_Grounded:
                        switch (Stance) {
                            case EALSStance::EAS_Standing:
                                DesiredStance = EALSStance::EAS_Crouching;
                                Crouch();
                                break;
                            case EALSStance::EAS_Crouching:
                                DesiredStance = EALSStance::EAS_Standing;
                                UnCrouch();
                                break;
                        }
                        break;
                    case EALSMovementState::EAMS_InAir:
                        BreakFall = true;
                        RetriggerDelay(0.4f, TEXT("SetBreakFallToFlase"));
                        break;
                }
                return;
            }
            if (MainAnimInstance) {
                // Roll: Simply play a Root Motion Montage.
                MainAnimInstance->Montage_Play(
                    GetRollAnimation(),                     // UAnimMontage* MontageToPlay
                    1.15,                                   // float InPlayRate = 1.f
                    EMontagePlayReturnType::MontageLength,  // EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength
                    0.f,                                    // float InTimeToStartMontageAt=0.f
                    true                                    // bool bStopAllMontages = true
                );
                switch (Stance) {
                    case EALSStance::EAS_Standing:
                        DesiredStance = EALSStance::EAS_Crouching;
                        break;
                    case EALSStance::EAS_Crouching:
                        DesiredStance = EALSStance::EAS_Standing;
                        break;
                }
            }
        }
    });

    // Gait Actions Type 1: Press "Walk Action" to toggle walking/running, hold "Sprint Action" to sprint.
    AddInputActionBindingLambda(PlayerInputComponent, "WalkAction", IE_Pressed, [this]() {
        switch (DesiredGait) {
            case EALSGait::EAG_Walking:
                DesiredGait = EALSGait::EAG_Running;
                break;
            case EALSGait::EAG_Running:
                DesiredGait = EALSGait::EAG_Walking;
                break;
        }
    });

    AddInputActionBindingLambda(PlayerInputComponent, "SprintAction", IE_Pressed, [this]() {
        DesiredGait = EALSGait::EAG_Sprinting;
    });

    AddInputActionBindingLambda(PlayerInputComponent, "SprintAction", IE_Released, [this]() {
        DesiredGait = EALSGait::EAG_Running;
    });

    // Gait Action Type 2 (Unused): Hold "Sprint Action" to run, double tap and hold to sprint.
    /*
    AddInputActionBindingLambda(PlayerInputComponent, "SprintAction", IE_Pressed, [this]() {
        SprintHeld = true;
        SprintPressedTimes++;
        RetriggerDelay(0.3f, TEXT("ResetSprintPressedTimes"));
        if (SprintPressedTimes == 1) {
            DesiredGait = EALSGait::EAG_Running;
        } else {
            DesiredGait = EALSGait::EAG_Sprinting;
        }
    });

    AddInputActionBindingLambda(PlayerInputComponent, "SprintAction", IE_Released, [this]() {
        SprintHeld = false;
        DesiredGait = EALSGait::EAG_Running;
        RetriggerDelay(0.2f, TEXT("SetDesiredGaitToWalkingIfNotSprintHeld"));
    });
    */

    // Select Rotation Mode: Switch the desired (default) rotation mode to Velocity or Looking Direction.
    // This will be the mode the character reverts back to when un-aiming
    AddInputActionBindingLambda(PlayerInputComponent, "SelectRotationMode_1", IE_Pressed, [this]() {
        DesiredRotationMode = EALSRotationMode::EARM_VelocityDirection;
        SetRotationMode(DesiredRotationMode);
    });

    AddInputActionBindingLambda(PlayerInputComponent, "SelectRotationMode_2", IE_Pressed, [this]() {
        DesiredRotationMode = EALSRotationMode::EARM_LookingDirection;
        SetRotationMode(DesiredRotationMode);
    });

    // AimAction: Hold "AimAction" to enter the aiming mode,
    // release to revert back the desired rotation mode.
    AddInputActionBindingLambda(PlayerInputComponent, "AimAction", IE_Pressed, [this]() {
        SetRotationMode(EALSRotationMode::EARM_Aiming);
    });

    AddInputActionBindingLambda(PlayerInputComponent, "AimAction", IE_Released, [this]() {
        switch (ViewMode) {
            case EALSViewMode::EAVM_ThirdPerson:
                SetRotationMode(DesiredRotationMode);
                break;
            case EALSViewMode::EAVM_FirstPerson:
                SetRotationMode(EALSRotationMode::EARM_LookingDirection);
                break;
        }
    });

    // Camera Action: Hold "Camera Action" to toggle Third / First Person,
    // tap to swap shoulders.
    PlayerInputComponent->BindAction<FBindInputMappingWithInputEventDelegate>("CameraAction", IE_Pressed, this, &ABaseCharacter::PerformCameraAction, IE_Pressed);
    PlayerInputComponent->BindAction<FBindInputMappingWithInputEventDelegate>("CameraAction", IE_Released, this, &ABaseCharacter::PerformCameraAction, IE_Released);

    // Ragdoll Action: Press "Ragdoll Action" to toggle the ragdoll state on or off.
    AddInputActionBindingLambda(PlayerInputComponent, "RagdollAction", IE_Pressed, [this]() {
        switch (MovementState) {
            case EALSMovementState::EAMS_None:
            case EALSMovementState::EAMS_Grounded:
            case EALSMovementState::EAMS_InAir:
            case EALSMovementState::EAMS_Mantling:
                RagdollStart();
                break;
            case EALSMovementState::EAMS_Ragdoll:
                RagdollEnd();
                break;
        }
    });
}

// ================================================================================
// Input
void ABaseCharacter::PlayerMovementInput(bool IsForwardAxis) {
    FVector ControlForwardVector;
    FVector ControlRightVector;
    GetControlForwardRightVector(ControlForwardVector, ControlRightVector);
    float Y;
    float X;
    FixDiagonalGamepadValues(GetInputAxisValue(FName("MoveForward/Backwards")), GetInputAxisValue(FName("MoveRight/Left")), Y, X);
    switch (MovementState) {
        case EALSMovementState::EAMS_Grounded:
        case EALSMovementState::EAMS_InAir:
            if (IsForwardAxis) {
                AddMovementInput(ControlForwardVector, Y);
            } else {
                AddMovementInput(ControlRightVector, X);
            }
            break;
    }
}

FVector ABaseCharacter::GetPlayerMovementInput() {
    FVector ControlForwardVector;
    FVector ControlRightVector;
    GetControlForwardRightVector(ControlForwardVector, ControlRightVector);
    float MoveForwardBackwardsScale = GetInputAxisValue(FName("MoveForward/Backwards"));
    float MoveRightLeftScale = GetInputAxisValue(FName("MoveRight/Left"));
    return UKismetMathLibrary::Normal(
        MoveForwardBackwardsScale * ControlForwardVector + MoveRightLeftScale * ControlRightVector, 0.0001f);
}

void ABaseCharacter::FixDiagonalGamepadValues(float InY, float InX, float& OutY, float& OutX) {
    float RangeClampedX = UKismetMathLibrary::MapRangeClamped(FMath::Abs(InX), 0.0, 0.6, 1.0, 1.2);
    OutY = UKismetMathLibrary::FClamp(InY * RangeClampedX, -1.0, 1.0);
    float RangeClampedY = UKismetMathLibrary::MapRangeClamped(FMath::Abs(InY), 0.0, 0.6, 1.0, 1.2);
    OutX = UKismetMathLibrary::FClamp(InX * RangeClampedY, -1.0, 1.0);
}

void ABaseCharacter::PerformCameraAction(EInputEvent InputEvent) {
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController) {
        return;
    }
    TArray<FKey> Keys;
    TArray<FInputActionKeyMapping> OutMappings;
    UInputSettings::GetInputSettings()->GetActionMappingByName(FName("CameraAction"), OutMappings);
    for (FInputActionKeyMapping InputActionKeyMapping : OutMappings) {
        Keys.Add(InputActionKeyMapping.Key);
    }
    if (InputEvent == IE_Pressed) {
        FTimerHandle TimerHandle;
        auto Lambda = [this, PlayerController, Keys]() {
            bool HeldEnough = false;
            for (FKey Key : Keys) {
                if (PlayerController->GetInputKeyTimeDown(Key) >= 0.2f) {
                    HeldEnough = true;
                    break;
                }
            }
            if (HeldEnough) {
                if (ViewMode == EALSViewMode::EAVM_ThirdPerson) {
                    SetViewMode(EALSViewMode::EAVM_FirstPerson);
                } else if (ViewMode == EALSViewMode::EAVM_FirstPerson) {
                    SetViewMode(EALSViewMode::EAVM_ThirdPerson);
                }
            }
            CameraActionFinished = HeldEnough;
        };
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(Lambda), .2f, false);
    } else if (InputEvent == IE_Released) {
        if (!CameraActionFinished) {
            RightShoulder = !RightShoulder;
        }
        CameraActionFinished = false;
    }
}