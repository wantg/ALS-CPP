#pragma once

UENUM(BlueprintType)
enum class EALSGait : uint8 {
    EAG_Walking /*   */ UMETA(DisplayName = "Walking"),
    EAG_Running /*   */ UMETA(DisplayName = "Running"),
    EAG_Sprinting /* */ UMETA(DisplayName = "Sprinting"),
};

UENUM(BlueprintType)
enum class EALSMovementAction : uint8 {
    EAMA_None /*       */ UMETA(DisplayName = "None"),
    EAMA_LowMantle /*  */ UMETA(DisplayName = "Low Mantle"),
    EAMA_HighMantle /* */ UMETA(DisplayName = "High Mantle"),
    EAMA_Rolling /*    */ UMETA(DisplayName = "Rolling"),
    EAMA_GettingUp /*  */ UMETA(DisplayName = "Getting Up"),
};

UENUM(BlueprintType)
enum class EALSMovementState : uint8 {
    EAMS_None /*     */ UMETA(DisplayName = "None"),
    EAMS_Grounded /* */ UMETA(DisplayName = "Grounded"),
    EAMS_InAir /*    */ UMETA(DisplayName = "In Air"),
    EAMS_Mantling /* */ UMETA(DisplayName = "Mantling"),
    EAMS_Ragdoll /*  */ UMETA(DisplayName = "Ragdoll"),
};

UENUM(BlueprintType)
enum class EALSOverlayState : uint8 {
    EAOS_Default /*    */ UMETA(DisplayName = "Default"),
    EAOS_Masculine /*  */ UMETA(DisplayName = "Masculine"),
    EAOS_Feminine /*   */ UMETA(DisplayName = "Feminine"),
    EAOS_Injured /*    */ UMETA(DisplayName = "Injured"),
    EAOS_HandsTied /*  */ UMETA(DisplayName = "Hands Tied"),
    EAOS_Rifle /*      */ UMETA(DisplayName = "Rifle"),
    EAOS_Pistol1H /*   */ UMETA(DisplayName = "Pistol 1H"),
    EAOS_Pistol2H /*   */ UMETA(DisplayName = "Pistol 2H"),
    EAOS_Bow /*        */ UMETA(DisplayName = "Bow"),
    EAOS_Torch /*      */ UMETA(DisplayName = "Torch"),
    EAOS_Binoculars /* */ UMETA(DisplayName = "Binoculars"),
    EAOS_Box /*        */ UMETA(DisplayName = "Box"),
    EAOS_Barrel /*     */ UMETA(DisplayName = "Barrel"),
};

UENUM(BlueprintType)
enum class EALSRotationMode : uint8 {
    EARM_VelocityDirection /* */ UMETA(DisplayName = "Velocity Direction"),
    EARM_LookingDirection /*  */ UMETA(DisplayName = "Looking Direction"),
    EARM_Aiming /*            */ UMETA(DisplayName = "Aiming"),
};

UENUM(BlueprintType)
enum class EALSStance : uint8 {
    EAS_Standing /*  */ UMETA(DisplayName = "Standing"),
    EAS_Crouching /* */ UMETA(DisplayName = "Crouching"),
};

UENUM(BlueprintType)
enum class EALSViewMode : uint8 {
    EAVM_ThirdPerson UMETA(DisplayName = "Third Person"),
    EAVM_FirstPerson UMETA(DisplayName = "First Person"),
};

UENUM(BlueprintType)
enum class EAnimFeatureExample : uint8 {
    EAFE_StrideBlending /*  */ UMETA(DisplayName = "Stride Blending"),
    EAFE_AdditiveLeaning /* */ UMETA(DisplayName = "Additive Leaning"),
    EAFE_SprintImpulse /*   */ UMETA(DisplayName = "Sprint Impulse"),
};

UENUM(BlueprintType)
enum class EFootstepType : uint8 {
    EFT_Step /*    */ UMETA(DisplayName = "Step"),
    EFT_WalkRun /* */ UMETA(DisplayName = "Walk Run"),
    EFT_Jump /*    */ UMETA(DisplayName = "Jump"),
    EFT_Land /*    */ UMETA(DisplayName = "Land"),
};

UENUM(BlueprintType)
enum class EGroundedEntryState : uint8 {
    EGES_None UMETA(DisplayName = "None"),
    EGES_Roll UMETA(DisplayName = "Roll"),
};

UENUM(BlueprintType)
enum class EHipsDirection : uint8 {
    EHD_F /*  */ UMETA(DisplayName = "F"),
    EHD_B /*  */ UMETA(DisplayName = "B"),
    EHD_RF /* */ UMETA(DisplayName = "RF"),
    EHD_RB /* */ UMETA(DisplayName = "RB"),
    EHD_LF /* */ UMETA(DisplayName = "LF"),
    EHD_LB /* */ UMETA(DisplayName = "LB"),
};

UENUM(BlueprintType)
enum class EMantleType : uint8 {
    EMT_HighMantle /*   */ UMETA(DisplayName = "High Mantle"),
    EMT_LowMantle /*    */ UMETA(DisplayName = "Low Mantle"),
    EMT_FallingCatch /* */ UMETA(DisplayName = "Falling Catch"),
};

UENUM(BlueprintType)
enum class EMovementDirection : uint8 {
    EMD_Forward /*  */ UMETA(DisplayName = "Forward"),
    EMD_Right /*    */ UMETA(DisplayName = "Right"),
    EMD_Left /*     */ UMETA(DisplayName = "Left"),
    EMD_Backward /* */ UMETA(DisplayName = "Backward"),
};
