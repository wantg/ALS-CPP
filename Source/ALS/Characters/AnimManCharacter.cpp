#include "ALS/Characters/AnimManCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerInput.h"
#include "ALS/Interfaces/ALSController.h"
#include "Kismet/GameplayStatics.h"
#include "ALS/Animations/BowAnim.h"
#include "Kismet/KismetMathLibrary.h"

AAnimManCharacter::AAnimManCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    // Components
    GetCapsuleComponent()->SetCapsuleRadius(35.f);
    GetMesh()->SetRelativeRotation(FRotator{0.000041f, -89.999901f, 0.f});
    GetMesh()->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/Meshes/AnimMan")));
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    GetMesh()->SetAnimInstanceClass(LoadClass<UAnimInstance>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/ALS_AnimBP.ALS_AnimBP_C")));
    for (int32 i = 0; i < 15; i++) {
        const TCHAR* s = i == 10 ? TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/Materials/M_AnimMan_Eyes") : TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/Materials/M_AnimMan_Default");
        GetMesh()->SetMaterial(i, LoadObject<UMaterial>(nullptr, s));
    }
    GetMesh()->OverrideMaterials = {nullptr};
    GetMesh()->bUpdateJointsFromAnimation = true;
    GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

    RightShoulder = true;

    VisualMeshes = CreateDefaultSubobject<USceneComponent>(TEXT("VisualMeshes"));
    VisualMeshes->SetupAttachment(GetMesh());

    BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
    BodyMesh->SetupAttachment(VisualMeshes);

    HeldObjectRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HeldObjectRoot"));
    HeldObjectRoot->SetupAttachment(GetMesh());

    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    SkeletalMesh->SetupAttachment(HeldObjectRoot);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(HeldObjectRoot);
    StaticMesh->SetCollisionProfileName(FName("ALS_Prop"));

    // Mantle Animations
    Mantle_2m_Default_0 = FMantleAsset{
        LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_N_Mantle_2m_Montage_Default")),
        LoadObject<UCurveVector>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/MantleCurves/Mantle_2m")),
        FVector{0.f, 65.f, 200.f},
        125.f, 1.2f, 0.6f,
        200.f, 1.2f, 0.f};
    Mantle_1m_Default_0 = FMantleAsset{
        LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_N_Mantle_1m_Montage_Default")),
        LoadObject<UCurveVector>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/MantleCurves/Mantle_1m")),
        FVector{0.f, 65.f, 100.f},
        50.f, 1.f, 0.5f,
        100.f, 1.f, 0.f};
    Mantle_1m_RH_0 = FMantleAsset{
        LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_N_Mantle_1m_Montage_RH")),
        LoadObject<UCurveVector>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/MantleCurves/Mantle_1m")),
        FVector{0.f, 65.f, 100.f},
        50.f, 1.f, 0.5f,
        100.f, 1.f, 0.f};
    Mantle_1m_LH_0 = FMantleAsset{
        LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_N_Mantle_1m_Montage_LH")),
        LoadObject<UCurveVector>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/MantleCurves/Mantle_1m")),
        FVector{0.f, 65.f, 100.f},
        50.f, 1.f, 0.5f,
        100.f, 1.f, 0.f};
    Mantle_1m_2H_0 = FMantleAsset{
        LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_N_Mantle_1m_Montage_2H")),
        LoadObject<UCurveVector>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/MantleCurves/Mantle_1m")),
        FVector{0.f, 65.f, 100.f},
        50.f, 1.f, 0.5f,
        100.f, 1.f, 0.f};
    Mantle_1m_Box_0 = FMantleAsset{
        LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_N_Mantle_1m_Montage_Box")),
        LoadObject<UCurveVector>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Data/Curves/MantleCurves/Mantle_1m")),
        FVector{0.f, 65.f, 100.f},
        50.f, 1.f, 0.5f,
        100.f, 1.f, 0.f};

    // Roll Animations
    LandRoll_Default = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_N_LandRoll_F_Montage_Default"));
    LandRoll_RH = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_N_LandRoll_F_Montage_RH"));
    LandRoll_LH = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_N_LandRoll_F_Montage_LH"));
    LandRoll_2H = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_N_LandRoll_F_Montage_2H"));

    // Get Up Animations
    GetUpFront_Default = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_CLF_GetUp_Front_Montage_Default"));
    GetUpFront_RH = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_CLF_GetUp_Front_Montage_RH"));
    GetUpFront_LH = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_CLF_GetUp_Front_Montage_LH"));
    GetUpFront_2H = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_CLF_GetUp_Front_Montage_2H"));
    GetUpBack_Default = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_CLF_GetUp_Back_Montage_Default"));
    GetUpBack_RH = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_CLF_GetUp_Back_Montage_RH"));
    GetUpBack_LH = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_CLF_GetUp_Back_Montage_LH"));
    GetUpBack_2H = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/AnimationExamples/Actions/ALS_CLF_GetUp_Back_Montage_2H"));
}

void AAnimManCharacter::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
    UpdateColoringSystem();
    UpdateHeldObjectAnimations();
}

void AAnimManCharacter::OnConstruction(const FTransform& Transform) {
    Super::OnConstruction(Transform);
    BodyMesh->SetLeaderPoseComponent(GetMesh(), false, false);
    SetDynamicMaterials();
    SetResetColors();
}

void AAnimManCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    Cast<APlayerController>(GetController())->PlayerInput->AddActionMapping(FInputActionKeyMapping{FName("MKeyPressed"), EKeys::M});
    PlayerInputComponent->BindAction("MKeyPressed", IE_Pressed, this, &AAnimManCharacter::MKeyPressed);
}

// ================================================================================
// IALSCamera
void AAnimManCharacter::Get3PTraceParams(FVector& TraceOrigin, float& TraceRadius, TEnumAsByte<ETraceTypeQuery>& TraceChannel) {
    if (RightShoulder) {
        TraceOrigin = GetMesh()->GetSocketLocation("TP_CameraTrace_R");
    } else {
        TraceOrigin = GetMesh()->GetSocketLocation("TP_CameraTrace_L");
    }
    TraceRadius = 15.f;
    TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera);
}

FTransform AAnimManCharacter::Get3PPivotTarget() {
    return FTransform{
        GetActorRotation(),
        UKismetMathLibrary::GetVectorArrayAverage(
            {GetMesh()->GetSocketLocation("head"),
             GetMesh()->GetSocketLocation("root")}),
        FVector{1.f}};
}

FVector AAnimManCharacter::GetFPCameraTarget() {
    return GetMesh()->GetSocketLocation("FP_Camera");
}

// ================================================================================
// Coloring System
void AAnimManCharacter::SetDynamicMaterials() {
    Head = GetMesh()->CreateDynamicMaterialInstance(2, nullptr, FName("None"));
    Torso = GetMesh()->CreateDynamicMaterialInstance(1, nullptr, FName("None"));
    Pelvis = GetMesh()->CreateDynamicMaterialInstance(0, nullptr, FName("None"));

    UpperLegs = GetMesh()->CreateDynamicMaterialInstance(6, nullptr, FName("None"));
    LowerLegs = GetMesh()->CreateDynamicMaterialInstance(7, nullptr, FName("None"));
    Feet = GetMesh()->CreateDynamicMaterialInstance(8, nullptr, FName("None"));

    Shoulder_L = GetMesh()->CreateDynamicMaterialInstance(4, nullptr, FName("None"));
    UpperArm_L = GetMesh()->CreateDynamicMaterialInstance(3, nullptr, FName("None"));
    LowerArm_L = GetMesh()->CreateDynamicMaterialInstance(5, nullptr, FName("None"));
    Hand_L = GetMesh()->CreateDynamicMaterialInstance(9, nullptr, FName("None"));

    Shoulder_R = GetMesh()->CreateDynamicMaterialInstance(12, nullptr, FName("None"));
    UpperArm_R = GetMesh()->CreateDynamicMaterialInstance(11, nullptr, FName("None"));
    LowerArm_R = GetMesh()->CreateDynamicMaterialInstance(13, nullptr, FName("None"));
    Hand_R = GetMesh()->CreateDynamicMaterialInstance(14, nullptr, FName("None"));
}

void AAnimManCharacter::SetResetColors() {
    if (SolidColor) {
        Head->SetVectorParameterValue(FName("BaseColor"), DefaultColor);
        Torso->SetVectorParameterValue(FName("BaseColor"), DefaultColor);
        Pelvis->SetVectorParameterValue(FName("BaseColor"), DefaultColor);
        Shoulder_L->SetVectorParameterValue(FName("BaseColor"), DefaultColor);
        UpperArm_L->SetVectorParameterValue(FName("BaseColor"), DefaultColor);
        LowerArm_L->SetVectorParameterValue(FName("BaseColor"), DefaultColor);
        Hand_L->SetVectorParameterValue(FName("BaseColor"), DefaultColor);
        Shoulder_R->SetVectorParameterValue(FName("BaseColor"), DefaultColor);
        UpperArm_R->SetVectorParameterValue(FName("BaseColor"), DefaultColor);
        LowerArm_R->SetVectorParameterValue(FName("BaseColor"), DefaultColor);
        Hand_R->SetVectorParameterValue(FName("BaseColor"), DefaultColor);
        UpperLegs->SetVectorParameterValue(FName("BaseColor"), DefaultColor);
        LowerLegs->SetVectorParameterValue(FName("BaseColor"), DefaultColor);
        Feet->SetVectorParameterValue(FName("BaseColor"), DefaultColor);
        return;
    }
    Head->SetVectorParameterValue(FName("BaseColor"), SkinColor);
    switch (ShirtType) {
        case 0:
            Torso->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            Shoulder_L->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            Shoulder_R->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            UpperArm_L->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            UpperArm_R->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            LowerArm_L->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            LowerArm_R->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            break;
        case 1:
            Torso->SetVectorParameterValue(FName("BaseColor"), ShirtColor);
            Shoulder_L->SetVectorParameterValue(FName("BaseColor"), ShirtColor);
            Shoulder_R->SetVectorParameterValue(FName("BaseColor"), ShirtColor);
            UpperArm_L->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            UpperArm_R->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            LowerArm_L->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            LowerArm_R->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            break;
        case 2:
            Torso->SetVectorParameterValue(FName("BaseColor"), ShirtColor);
            Shoulder_L->SetVectorParameterValue(FName("BaseColor"), ShirtColor);
            Shoulder_R->SetVectorParameterValue(FName("BaseColor"), ShirtColor);
            UpperArm_L->SetVectorParameterValue(FName("BaseColor"), ShirtColor);
            UpperArm_R->SetVectorParameterValue(FName("BaseColor"), ShirtColor);
            LowerArm_L->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            LowerArm_R->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            break;
        case 3:
            Torso->SetVectorParameterValue(FName("BaseColor"), ShirtColor);
            Shoulder_L->SetVectorParameterValue(FName("BaseColor"), ShirtColor);
            Shoulder_R->SetVectorParameterValue(FName("BaseColor"), ShirtColor);
            UpperArm_L->SetVectorParameterValue(FName("BaseColor"), ShirtColor);
            UpperArm_R->SetVectorParameterValue(FName("BaseColor"), ShirtColor);
            LowerArm_L->SetVectorParameterValue(FName("BaseColor"), ShirtColor);
            LowerArm_R->SetVectorParameterValue(FName("BaseColor"), ShirtColor);
            break;
    }
    switch (PantsType) {
        case 0:
            Pelvis->SetVectorParameterValue(FName("BaseColor"), PantsColor);
            UpperLegs->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            LowerLegs->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            break;
        case 1:
            Pelvis->SetVectorParameterValue(FName("BaseColor"), PantsColor);
            UpperLegs->SetVectorParameterValue(FName("BaseColor"), PantsColor);
            LowerLegs->SetVectorParameterValue(FName("BaseColor"), SkinColor);
            break;
        case 2:
            Pelvis->SetVectorParameterValue(FName("BaseColor"), PantsColor);
            UpperLegs->SetVectorParameterValue(FName("BaseColor"), PantsColor);
            LowerLegs->SetVectorParameterValue(FName("BaseColor"), PantsColor);
            break;
    }
    Feet->SetVectorParameterValue(FName("BaseColor"), Shoes ? ShoesColor : SkinColor);
    Hand_L->SetVectorParameterValue(FName("BaseColor"), Gloves ? GlovesColor : SkinColor);
    Hand_R->SetVectorParameterValue(FName("BaseColor"), Gloves ? GlovesColor : SkinColor);
}

void AAnimManCharacter::UpdateColoringSystem() {
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
        if (GetMesh()->IsVisible() && ShowLayerColors) {
            UpdateLayeringColors();
            return;
        }
        SetResetColors();
    }
}

void AAnimManCharacter::UpdateLayeringColors() {
    FLinearColor l_LinearColor;

    l_LinearColor = UKismetMathLibrary::LinearColorLerp(OverlayLayerColor, AdditiveAmount_Color, GetAnimCurveValue(FName("Layering_Head_Add")));
    Head->SetVectorParameterValue(FName("BaseColor"), UKismetMathLibrary::LinearColorLerp(BaseLayerColor, l_LinearColor, GetAnimCurveValue(FName("Layering_Head"))));

    l_LinearColor = UKismetMathLibrary::LinearColorLerp(OverlayLayerColor, AdditiveAmount_Color, GetAnimCurveValue(FName("Layering_Spine_Add")));
    Torso->SetVectorParameterValue(FName("BaseColor"), UKismetMathLibrary::LinearColorLerp(BaseLayerColor, l_LinearColor, GetAnimCurveValue(FName("Layering_Spine"))));

    l_LinearColor = UKismetMathLibrary::LinearColorLerp(BaseLayerColor, AdditiveAmount_Color, GetAnimCurveValue(FName("Layering_Pelvis")));
    Pelvis->SetVectorParameterValue(FName("BaseColor"), l_LinearColor);

    l_LinearColor = UKismetMathLibrary::LinearColorLerp(BaseLayerColor, AdditiveAmount_Color, GetAnimCurveValue(FName("Layering_Legs")));
    UpperLegs->SetVectorParameterValue(FName("BaseColor"), l_LinearColor);
    LowerLegs->SetVectorParameterValue(FName("BaseColor"), l_LinearColor);
    Feet->SetVectorParameterValue(FName("BaseColor"), l_LinearColor);

    l_LinearColor = UKismetMathLibrary::LinearColorLerp(OverlayLayerColor, AdditiveAmount_Color, GetAnimCurveValue(FName("Layering_Arm_L_Add")));
    l_LinearColor = UKismetMathLibrary::LinearColorLerp(BaseLayerColor, l_LinearColor, GetAnimCurveValue(FName("Layering_Arm_L")));
    Shoulder_L->SetVectorParameterValue(FName("BaseColor"), l_LinearColor);
    UpperArm_L->SetVectorParameterValue(FName("BaseColor"), l_LinearColor);
    LowerArm_L->SetVectorParameterValue(FName("BaseColor"), l_LinearColor);

    LowerArm_L->GetVectorParameterValue(FName("BaseColor"), l_LinearColor);
    l_LinearColor = UKismetMathLibrary::LinearColorLerp(l_LinearColor, Hand_Color, GetAnimCurveValue(FName("Layering_Hand_L")));
    Hand_L->SetVectorParameterValue(FName("BaseColor"), UKismetMathLibrary::LinearColorLerp(l_LinearColor, HandIK_Color, GetAnimCurveValue(FName("Enable_HandIK_L"))));

    l_LinearColor = UKismetMathLibrary::LinearColorLerp(OverlayLayerColor, AdditiveAmount_Color, GetAnimCurveValue(FName("Layering_Arm_R_Add")));
    l_LinearColor = UKismetMathLibrary::LinearColorLerp(BaseLayerColor, l_LinearColor, GetAnimCurveValue(FName("Layering_Arm_R")));
    Shoulder_R->SetVectorParameterValue(FName("BaseColor"), l_LinearColor);
    UpperArm_R->SetVectorParameterValue(FName("BaseColor"), l_LinearColor);
    LowerArm_R->SetVectorParameterValue(FName("BaseColor"), l_LinearColor);

    LowerArm_R->GetVectorParameterValue(FName("BaseColor"), l_LinearColor);
    l_LinearColor = UKismetMathLibrary::LinearColorLerp(l_LinearColor, Hand_Color, GetAnimCurveValue(FName("Layering_Hand_R")));
    Hand_R->SetVectorParameterValue(FName("BaseColor"), UKismetMathLibrary::LinearColorLerp(l_LinearColor, HandIK_Color, GetAnimCurveValue(FName("Enable_HandIK_R"))));
}

// ================================================================================
// Held Object
void AAnimManCharacter::UpdateHeldObject() {
    switch (OverlayState) {
        case EALSOverlayState::EAOS_Default:
        case EALSOverlayState::EAOS_Masculine:
        case EALSOverlayState::EAOS_Feminine:
        case EALSOverlayState::EAOS_Injured:
        case EALSOverlayState::EAOS_HandsTied:
            ClearHeldObject();
            break;
        case EALSOverlayState::EAOS_Rifle:
            AttachToHand(nullptr, LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Props/Meshes/M4A1")), nullptr, false, FVector::ZeroVector);
            break;
        case EALSOverlayState::EAOS_Pistol1H:
            AttachToHand(nullptr, LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Props/Meshes/M9")), nullptr, false, FVector::ZeroVector);
            break;
        case EALSOverlayState::EAOS_Pistol2H:
            AttachToHand(nullptr, LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Props/Meshes/M9")), nullptr, false, FVector::ZeroVector);
            break;
        case EALSOverlayState::EAOS_Bow:
            AttachToHand(nullptr, LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Props/Meshes/Bow")),
                         LoadClass<UAnimInstance>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Props/Meshes/Bow_AnimBP.Bow_AnimBP_C")), true, FVector::ZeroVector);
            break;
        case EALSOverlayState::EAOS_Torch:
            AttachToHand(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Props/Meshes/Torch")), nullptr, nullptr, true, FVector::ZeroVector);
            break;
        case EALSOverlayState::EAOS_Binoculars:
            AttachToHand(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Props/Meshes/Binoculars")), nullptr, nullptr, false, FVector::ZeroVector);
            break;
        case EALSOverlayState::EAOS_Box:
            AttachToHand(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Props/Meshes/Box")), nullptr, nullptr, false, FVector::ZeroVector);
            break;
        case EALSOverlayState::EAOS_Barrel:
            AttachToHand(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Props/Meshes/Barrel")), nullptr, nullptr, true, FVector::ZeroVector);
            break;
    }
}

void AAnimManCharacter::ClearHeldObject() {
    StaticMesh->SetStaticMesh(nullptr);
    SkeletalMesh->SetSkinnedAssetAndUpdate(nullptr, false);
    SkeletalMesh->SetAnimInstanceClass(nullptr);
}

void AAnimManCharacter::AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh, UClass* NewAnimClass, bool LeftHand, FVector Offset) {
    ClearHeldObject();
    if (NewStaticMesh) {
        StaticMesh->SetStaticMesh(NewStaticMesh);
    }
    if (NewSkeletalMesh) {
        SkeletalMesh->SetSkinnedAssetAndUpdate(NewSkeletalMesh, true);
    }
    if (NewAnimClass) {
        SkeletalMesh->SetAnimInstanceClass(NewAnimClass);
    }
    HeldObjectRoot->AttachToComponent(GetMesh(), FAttachmentTransformRules{EAttachmentRule::SnapToTarget, true}, LeftHand ? FName("VB LHS_ik_hand_gun") : FName("VB RHS_ik_hand_gun"));
    HeldObjectRoot->SetRelativeLocation(Offset, false, false);
}

void AAnimManCharacter::UpdateHeldObjectAnimations() {
    if (OverlayState == EALSOverlayState::EAOS_Bow) {
        if (UBowAnim* BowAnim = Cast<UBowAnim>(SkeletalMesh->GetAnimInstance())) {
            BowAnim->Draw = GetAnimCurveValue(FName("Enable_SpineRotation"));
        }
    }
}

// ================================================================================
// State Changes
void AAnimManCharacter::OnOverlayStateChanged(EALSOverlayState NewOverlayState) {
    Super::OnOverlayStateChanged(NewOverlayState);
    UpdateHeldObject();
}

// ================================================================================
// Movement System
UAnimMontage* AAnimManCharacter::GetRollAnimation() {
    switch (OverlayState) {
        case EALSOverlayState::EAOS_Default:
        case EALSOverlayState::EAOS_Masculine:
        case EALSOverlayState::EAOS_Feminine:
            return LandRoll_Default;
        case EALSOverlayState::EAOS_Injured:
            return LandRoll_LH;
        case EALSOverlayState::EAOS_HandsTied:
            return LandRoll_2H;
        case EALSOverlayState::EAOS_Rifle:
            return LandRoll_2H;
        case EALSOverlayState::EAOS_Pistol1H:
            return LandRoll_RH;
        case EALSOverlayState::EAOS_Pistol2H:
            return LandRoll_LH;
        case EALSOverlayState::EAOS_Bow:
            return LandRoll_LH;
        case EALSOverlayState::EAOS_Torch:
            return LandRoll_2H;
        case EALSOverlayState::EAOS_Binoculars:
            return LandRoll_2H;
        case EALSOverlayState::EAOS_Box:
            return LandRoll_LH;
        case EALSOverlayState::EAOS_Barrel:
            return LandRoll_RH;
    }
    return nullptr;
}

// ================================================================================
// Mantle System
void AAnimManCharacter::MantleStart(float MantleHeight, FALSComponentAndTransform MantleLedgeWS, EMantleType MantleType) {
    Super::MantleStart(MantleHeight, MantleLedgeWS, MantleType);
    if (MantleType != EMantleType::EMT_LowMantle) {
        ClearHeldObject();
    }
}

void AAnimManCharacter::MantleEnd() {
    Super::MantleEnd();
    UpdateHeldObject();
}

FMantleAsset AAnimManCharacter::GetMantleAsset(EMantleType MantleType) {
    if (MantleType == EMantleType::EMT_LowMantle) {
        switch (OverlayState) {
            case EALSOverlayState::EAOS_Default:
            case EALSOverlayState::EAOS_Masculine:
            case EALSOverlayState::EAOS_Feminine:
                return Mantle_1m_Default_0;
            case EALSOverlayState::EAOS_Injured:
                return Mantle_1m_LH_0;
            case EALSOverlayState::EAOS_HandsTied:
                return Mantle_1m_2H_0;
            case EALSOverlayState::EAOS_Rifle:
            case EALSOverlayState::EAOS_Pistol1H:
            case EALSOverlayState::EAOS_Pistol2H:
                return Mantle_1m_RH_0;
            case EALSOverlayState::EAOS_Bow:
                return Mantle_1m_LH_0;
            case EALSOverlayState::EAOS_Torch:
                return Mantle_1m_LH_0;
            case EALSOverlayState::EAOS_Binoculars:
                return Mantle_1m_RH_0;
            case EALSOverlayState::EAOS_Box:
                return Mantle_1m_Box_0;
            case EALSOverlayState::EAOS_Barrel:
                return Mantle_1m_LH_0;
        }
    } else {
        return Mantle_2m_Default_0;
    }
    return FMantleAsset{};
}

// ================================================================================
// Ragdoll System
void AAnimManCharacter::RagdollStart() {
    ClearHeldObject();
    Super::RagdollStart();
}

void AAnimManCharacter::RagdollEnd() {
    Super::RagdollEnd();
    UpdateHeldObject();
}

UAnimMontage* AAnimManCharacter::GetGetUpAnimation(bool InRagdollFaceUp) {
    switch (OverlayState) {
        case EALSOverlayState::EAOS_Default:
        case EALSOverlayState::EAOS_Masculine:
        case EALSOverlayState::EAOS_Feminine:
            return InRagdollFaceUp ? GetUpBack_Default : GetUpFront_Default;
        case EALSOverlayState::EAOS_Injured:
            return InRagdollFaceUp ? GetUpBack_LH : GetUpFront_LH;
        case EALSOverlayState::EAOS_HandsTied:
            return InRagdollFaceUp ? GetUpBack_2H : GetUpFront_2H;
        case EALSOverlayState::EAOS_Rifle:
        case EALSOverlayState::EAOS_Pistol1H:
        case EALSOverlayState::EAOS_Pistol2H:
            return InRagdollFaceUp ? GetUpBack_RH : GetUpFront_RH;
        case EALSOverlayState::EAOS_Bow:
        case EALSOverlayState::EAOS_Torch:
            return InRagdollFaceUp ? GetUpBack_LH : GetUpFront_LH;
        case EALSOverlayState::EAOS_Binoculars:
            return InRagdollFaceUp ? GetUpBack_RH : GetUpFront_RH;
        case EALSOverlayState::EAOS_Box:
            return InRagdollFaceUp ? GetUpBack_2H : GetUpFront_2H;
        case EALSOverlayState::EAOS_Barrel:
            return InRagdollFaceUp ? GetUpBack_LH : GetUpFront_LH;
    }
    return nullptr;
}

void AAnimManCharacter::MKeyPressed() {
    MKeyFlipFlop = !MKeyFlipFlop;
    if (MKeyFlipFlop) {
        BodyMesh->SetSkinnedAssetAndUpdate(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AdvancedLocomotionV4/CharacterAssets/MannequinSkeleton/Meshes/Mannequin")), true);
        GetMesh()->SetVisibility(false, false);
    } else {
        BodyMesh->SetSkinnedAssetAndUpdate(nullptr, true);
        GetMesh()->SetVisibility(true, false);
    }
}