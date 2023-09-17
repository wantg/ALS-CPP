#include "ALS/Animations/ALSPlayerCameraBehavior.h"
#include "ALS/Interfaces/ALSCharacter.h"
#include "ALS/Interfaces/ALSCamera.h"
#include "ALS/Interfaces/ALSController.h"

void UALSPlayerCameraBehavior::NativeUpdateAnimation(float DeltaSeconds) {
    Super::NativeUpdateAnimation(DeltaSeconds);

    // Get info from the character each frame to use in the camera graph.
    UpdateCharacterInfo();
}

void UALSPlayerCameraBehavior::UpdateCharacterInfo() {
    if (IALSCharacter* ALSCharacter = Cast<IALSCharacter>(ControlledPawn)) {
        TEnumAsByte<EMovementMode> CurrentPawnMovementMode;
        EALSMovementState CurrentMovementState;
        EALSMovementState CurrentPrevMovementState;
        EALSMovementAction CurrentMovementAction;
        EALSRotationMode CurrentRotationMode;
        EALSGait CurrentActualGait;
        EALSStance CurrentActualStance;
        EALSViewMode CurrentViewMode;
        EALSOverlayState CurrentOverlayState;
        ALSCharacter->GetCurrentStates(CurrentPawnMovementMode, CurrentMovementState, CurrentPrevMovementState, CurrentMovementAction, CurrentRotationMode, CurrentActualGait, CurrentActualStance, CurrentViewMode, CurrentOverlayState);
        MovementState = CurrentMovementState;
        MovementAction = CurrentMovementAction;
        RotationMode = CurrentRotationMode;
        Gait = CurrentActualGait;
        Stance = CurrentActualStance;
        ViewMode = CurrentViewMode;
    }
    if (IALSCamera* ALSCamera = Cast<IALSCamera>(ControlledPawn)) {
        float TPFOV = 0.f;
        float FPFOV = 0.f;
        bool CurrentRightShoulder;
        ALSCamera->GetCameraParameters(TPFOV, FPFOV, CurrentRightShoulder);
        RightShoulder = CurrentRightShoulder;
    }
    if (IALSController* ALSController = Cast<IALSController>(PlayerController)) {
        ACharacter* DebugFocusCharacter;
        bool CurrentDebugView;
        bool ShowHUD;
        bool ShowTraces;
        bool ShowDebugShapes;
        bool ShowLayerColors;
        bool Slomo;
        bool ShowCharacterInfo;
        ALSController->GetDebugInfo(DebugFocusCharacter, CurrentDebugView, ShowHUD, ShowTraces, ShowDebugShapes, ShowLayerColors, Slomo, ShowCharacterInfo);
        DebugView = CurrentDebugView;
    }
}