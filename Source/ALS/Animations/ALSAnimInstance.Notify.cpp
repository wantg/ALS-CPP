#include "ALS/Animations/ALSAnimInstance.h"
#include "ALS/Libraries/ALSMacroLibrary.h"

bool UALSAnimInstance::HandleNotify(const FAnimNotifyEvent& AnimNotifyEvent) {
    bool b = Super::HandleNotify(AnimNotifyEvent);
    // Stop Transition/Turn Animations
    if (AnimNotifyEvent.NotifyName == "StopTransition") {
        StopSlotAnimation(0.2f, FName("Grounded Slot"));
        StopSlotAnimation(0.2f, FName("(N) Turn/Rotate"));
        StopSlotAnimation(0.2f, FName("(CLF) Turn/Rotate"));
        return b;
    }

    // Reset Grounded Entry State
    if (AnimNotifyEvent.NotifyName == "Reset-GroundedEntryState") {
        GroundedEntryState = EGroundedEntryState::EGES_None;
        return b;
    }

    // Cycle Blending Notify Events
    // Pivot
    if (AnimNotifyEvent.NotifyName == "Pivot") {
        Pivot = Speed < TriggerPivotSpeedLimit;
        AddTimer([this]() { Pivot = false; }, 0.1f, false);
        return b;
    }
    // Set Tracked Hips Direction
    if (AnimNotifyEvent.NotifyName == "Hips F") {
        TrackedHipsDirection = EHipsDirection::EHD_F;
        return b;
    }
    if (AnimNotifyEvent.NotifyName == "Hips B") {
        TrackedHipsDirection = EHipsDirection::EHD_B;
        return b;
    }
    if (AnimNotifyEvent.NotifyName == "Hips LF") {
        TrackedHipsDirection = EHipsDirection::EHD_LF;
        return b;
    }
    if (AnimNotifyEvent.NotifyName == "Hips LB") {
        TrackedHipsDirection = EHipsDirection::EHD_LB;
        return b;
    }
    if (AnimNotifyEvent.NotifyName == "Hips RF") {
        TrackedHipsDirection = EHipsDirection::EHD_RF;
        return b;
    }
    if (AnimNotifyEvent.NotifyName == "Hips RB") {
        TrackedHipsDirection = EHipsDirection::EHD_RB;
        return b;
    }

    // Anim Notify Transition Events
    // Stopping Transitions
    if (AnimNotifyEvent.NotifyName == "->N Stop R") {
        PlayTransition(FDynamicMontageParams{
            LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Stop_R_Down")),
            0.2f, 0.2f, 1.5f, 0.4f});
        return b;
    }
    if (AnimNotifyEvent.NotifyName == "->N Stop L") {
        PlayTransition(FDynamicMontageParams{
            LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Stop_L_Down")),
            0.2f, 0.2f, 1.5f, 0.4f});
        return b;
    }
    if (AnimNotifyEvent.NotifyName == "->CLF Stop") {
        PlayTransition(FDynamicMontageParams{
            LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Transition_R")),
            0.2f, 0.2f, 1.5f, 0.2f});
        return b;
    }
    if (AnimNotifyEvent.NotifyName == "->N QuickStop ") {
        PlayTransition(FDynamicMontageParams{
            LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Transition_L")),
            0.1f, 0.2f, 1.75f, 0.3f});
        return b;
    }
    // Movement State / Action Transitions
    if (AnimNotifyEvent.NotifyName == "Roll->Idle") {
        PlayTransition(FDynamicMontageParams{
            LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Transition_R")),
            0.2f, 0.2f, 1.5f, 0.2f});
        return b;
    }
    if (AnimNotifyEvent.NotifyName == "Land->Idle") {
        PlayTransition(FDynamicMontageParams{
            LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Transition_R")),
            0.1f, 0.2f, 1.4f, 0.f});
        return b;
    }
    // Overlay State Transitions
    // Bow Transitions
    if (AnimNotifyEvent.NotifyName == "Bow Relaxed->Ready") {
        if (CanOverlayTransition()) {
            PlayTransition(FDynamicMontageParams{
                LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Transition_L")),
                0.2f, 0.2f, 1.5f, 0.3f});
            return b;
        }
    }
    if (AnimNotifyEvent.NotifyName == "Bow Ready->Relaxed") {
        if (CanOverlayTransition()) {
            PlayTransition(FDynamicMontageParams{
                LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Transition_L")),
                0.2f, 0.2f, 1.5f, 0.3f});
            return b;
        }
    }
    // M4A1 Transitions
    if (AnimNotifyEvent.NotifyName == "M4A1 Relaxed->Ready") {
        if (CanOverlayTransition()) {
            PlayTransition(FDynamicMontageParams{
                LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Transition_L")),
                0.2f, 0.2f, 1.75f, 0.3f});
            return b;
        }
    }
    if (AnimNotifyEvent.NotifyName == "M4A1 Ready->Relaxed") {
        if (CanOverlayTransition()) {
            PlayTransition(FDynamicMontageParams{
                LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Transition_L")),
                0.2f, 0.2f, 1.5f, 0.3f});
            return b;
        }
    }
    // Pistol 1H Transitions
    if (AnimNotifyEvent.NotifyName == "Pistol 1H Relaxed->Ready") {
        if (CanOverlayTransition()) {
            PlayTransition(FDynamicMontageParams{
                LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Transition_R")),
                0.2f, 0.2f, 1.75f, 0.3f});
            return b;
        }
    }
    if (AnimNotifyEvent.NotifyName == "Pistol 1H Ready->Relaxed") {
        if (CanOverlayTransition()) {
            PlayTransition(FDynamicMontageParams{
                LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Transition_R")),
                0.2f, 0.2f, 1.5f, 0.3f});
            return b;
        }
    }
    // Pistol 2H Transitions
    if (AnimNotifyEvent.NotifyName == "Pistol 2H Relaxed->Ready") {
        if (CanOverlayTransition()) {
            PlayTransition(FDynamicMontageParams{
                LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Transition_R")),
                0.2f, 0.2f, 1.75f, 0.3f});
            return b;
        }
    }
    if (AnimNotifyEvent.NotifyName == "Pistol 2H Ready->Relaxed") {
        if (CanOverlayTransition()) {
            PlayTransition(FDynamicMontageParams{
                LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Base/Transitions/ALS_N_Transition_R")),
                0.2f, 0.2f, 1.5f, 0.3f});
            return b;
        }
    }

    return b;
}