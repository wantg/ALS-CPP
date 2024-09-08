#include "ALS/CameraSystem/ALSPlayerCameraManager.h"
#include "ALS/Animations/ALSPlayerCameraBehavior.h"
#include "ALS/Interfaces/ALSCamera.h"
#include "ALS/Interfaces/ALSController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AALSPlayerCameraManager::AALSPlayerCameraManager() {
    AutoReceiveInput = EAutoReceiveInput::Type::Player0;
    CameraBehavior   = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CameraBehavior"));
    CameraBehavior->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Blueprints/CameraSystem/Camera")));
    CameraBehavior->SetMaterial(0, LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial")));
    CameraBehavior->SetMaterial(1, LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial")));
    // CameraBehavior->ClothingSimulationFactory = UClothingSimulationFactoryNv::StaticClass();
    CameraBehavior->SetHiddenInGame(true);
    CameraBehavior->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    CameraBehavior->SetAnimInstanceClass(LoadObject<UAnimBlueprint>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Blueprints/CameraSystem/ALS_PlayerCameraBehavior.ALS_PlayerCameraBehavior"))->GeneratedClass);
    CameraBehavior->SetupAttachment(RootComponent);
}

void AALSPlayerCameraManager::OnPossess(APawn* NewPawn) {
    // Set "Controlled Pawn" when Player Controller Possesses new character. (called from Player Controller)
    ControlledPawn = NewPawn;
    // Updated references in the Camera Behavior AnimBP.
    if (UALSPlayerCameraBehavior* ALSPlayerCameraBehavior = Cast<UALSPlayerCameraBehavior>(CameraBehavior->GetAnimInstance())) {
        ALSPlayerCameraBehavior->PlayerController = GetOwningPlayerController();
        ALSPlayerCameraBehavior->ControlledPawn   = ControlledPawn;
    }
}

void AALSPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) {
    // Check the Camera Target actor for the "ALS_Character" tag.
    // This will let the camera manager know if it should use the custom camera behavior
    if (OutVT.Target->ActorHasTag(FName("ALS_Character"))) {
        // Calculate custom camera parameters
        CustomCameraBehavior(OutVT.POV.Location, OutVT.POV.Rotation, OutVT.POV.FOV);
    } else {
        // Run Parent Function to return the default camera parameters
        Super::UpdateViewTargetInternal(OutVT, DeltaTime);
    }
}

void AALSPlayerCameraManager::CustomCameraBehavior(FVector& OutLocation, FRotator& OutRotation, float& OutFOV) {
    FTransform PivotTarget;
    FVector FPTarget;
    float TPFOV = 0.f;
    float FPFOV = 0.f;

    // Step 1: Get Camera Parameters from CharacterBP via the Camera Interface
    if (IALSCamera* ALSCamera = Cast<IALSCamera>(ControlledPawn)) {
        PivotTarget = ALSCamera->Get3PPivotTarget();
        FPTarget    = ALSCamera->GetFPCameraTarget();
        bool RightShoulder;
        ALSCamera->GetCameraParameters(TPFOV, FPFOV, RightShoulder);
    }

    // Step 2: Calculate Target Camera Rotation.
    // Use the Control Rotation and interpolate for smooth camera rotation.
    FRotator Interp = UKismetMathLibrary::RInterpTo(
        GetCameraRotation(),
        GetOwningPlayerController()->GetControlRotation(),
        GetCameraBehaviorParam(FName("RotationLagSpeed")),
        UGameplayStatics::GetWorldDeltaSeconds(this));
    TargetCameraRotation = UKismetMathLibrary::RLerp(Interp, DebugViewRotation, GetCameraBehaviorParam(FName("Override_Debug")), true);

    // Step 3: Calculate the Smoothed Pivot Target (Orange Sphere).
    // Get the 3P Pivot Target (Green Sphere) and interpolate using axis independent lag for maximum control.
    FVector LagSpeeds   = FVector{GetCameraBehaviorParam(FName("PivotLagSpeed_X")), GetCameraBehaviorParam(FName("PivotLagSpeed_Y")), GetCameraBehaviorParam(FName("PivotLagSpeed_Z"))};
    FVector L_Location  = CalculateAxisIndependentLag(SmoothedPivotTarget.GetLocation(), PivotTarget.GetLocation(), TargetCameraRotation, LagSpeeds);
    SmoothedPivotTarget = FTransform{PivotTarget.GetRotation().Rotator(), L_Location, FVector{1.f}};

    // Step 4: Calculate Pivot Location (BlueSphere).
    // Get the Smoothed Pivot Target and apply local offsets for further camera control.
    PivotLocation = SmoothedPivotTarget.GetLocation() +
                    UKismetMathLibrary::GetForwardVector(SmoothedPivotTarget.GetRotation().Rotator()) * GetCameraBehaviorParam(FName("PivotOffset_X")) +
                    UKismetMathLibrary::GetRightVector(SmoothedPivotTarget.GetRotation().Rotator()) * GetCameraBehaviorParam(FName("PivotOffset_Y")) +
                    UKismetMathLibrary::GetUpVector(SmoothedPivotTarget.GetRotation().Rotator()) * GetCameraBehaviorParam(FName("PivotOffset_Z"));

    // Step 5: Calculate Target Camera Location. Get the Pivot location and apply camera relative offsets.
    FVector L_LocationForCamera = PivotLocation +
                                  UKismetMathLibrary::GetForwardVector(TargetCameraRotation) * GetCameraBehaviorParam(FName("CameraOffset_X")) +
                                  UKismetMathLibrary::GetRightVector(TargetCameraRotation) * GetCameraBehaviorParam(FName("CameraOffset_Y")) +
                                  UKismetMathLibrary::GetUpVector(TargetCameraRotation) * GetCameraBehaviorParam(FName("CameraOffset_Z"));
    TargetCameraLocation = UKismetMathLibrary::VLerp(L_LocationForCamera, PivotTarget.GetLocation() + DebugViewOffset, GetCameraBehaviorParam(FName("Override_Debug")));

    // Step 6: Trace for an object between the camera and character to apply a corrective offset.
    // Trace origins are set within the Character BP via the Camera Interface.
    // Functions like the normal spring arm, but can allow for different trace origins regardless of the pivot.
    if (IALSCamera* ALSCamera = Cast<IALSCamera>(ControlledPawn)) {
        FVector TraceOrigin;
        float TraceRadius;
        TEnumAsByte<ETraceTypeQuery> TraceChannel;
        ALSCamera->Get3PTraceParams(TraceOrigin, TraceRadius, TraceChannel);

        TEnumAsByte<EDrawDebugTrace::Type> DebugType = EDrawDebugTrace::Type::None;
        GetDebugTraceType(EDrawDebugTrace::Type::ForOneFrame, DebugType);
        TArray<AActor*> ActorsToIgnore;
        FHitResult OutHit;
        UKismetSystemLibrary::SphereTraceSingle(
            this,                 // const UObject* WorldContextObject
            TraceOrigin,          // const FVector Start
            TargetCameraLocation, // const FVector End
            TraceRadius,          // float Radius
            TraceChannel,         // ETraceTypeQuery TraceChannel
            false,                // bool bTraceComplex
            ActorsToIgnore,       // const TArray<AActor*>& ActorsToIgnore
            DebugType,            // EDrawDebugTrace::Type DrawDebugType
            OutHit,               // FHitResult& OutHit
            true                  // bool bIgnoreSelf
                                  // FLinearColor TraceColor = FLinearColor::Red
                                  // FLinearColor TraceHitColor = FLinearColor::Green
                                  // float DrawTime = 5.0f
        );

        if (OutHit.bBlockingHit && !OutHit.bStartPenetrating) {
            TargetCameraLocation = OutHit.Location - OutHit.TraceEnd + TargetCameraLocation;
        }
    }

    // Step 7: Draw Debug Shapes.
    if (IALSController* ALSController = Cast<IALSController>(GetOwningPlayerController())) {
        ACharacter* DebugFocusCharacter;
        bool DebugView;
        bool ShowHUD;
        bool ShowTraces;
        bool ShowDebugShapes;
        bool ShowLayerColors;
        bool Slomo;
        bool ShowCharacterInfo;
        ALSController->GetDebugInfo(DebugFocusCharacter, DebugView, ShowHUD, ShowTraces, ShowDebugShapes, ShowLayerColors, Slomo, ShowCharacterInfo);
        if (ShowDebugShapes) {
            UKismetSystemLibrary::DrawDebugSphere(
                this,                      // const UObject* WorldContextObject
                PivotTarget.GetLocation(), // const FVector Center
                16.f,                      // float Radius=100.f
                8,                         // int32 Segments=12
                FLinearColor::Green,       // FLinearColor LineColor = FLinearColor::White
                0.f,                       // float Duration=0.f
                0.5f                       // float Thickness = 0.f
            );
            UKismetSystemLibrary::DrawDebugSphere(
                this,                                   // const UObject* WorldContextObject
                SmoothedPivotTarget.GetLocation(),      // const FVector Center
                16.f,                                   // float Radius=100.f
                8,                                      // int32 Segments=12
                FLinearColor{1.f, 0.166667f, 0.f, 1.f}, // FLinearColor LineColor = FLinearColor::White
                0.f,                                    // float Duration=0.f
                0.5f                                    // float Thickness = 0.f
            );
            UKismetSystemLibrary::DrawDebugSphere(
                this,                                   // const UObject* WorldContextObject
                PivotLocation,                          // const FVector Center
                16.f,                                   // float Radius=100.f
                8,                                      // int32 Segments=12
                FLinearColor{0.f, 0.666667f, 1.f, 1.f}, // FLinearColor LineColor = FLinearColor::White
                0.f,                                    // float Duration=0.f
                0.5f                                    // float Thickness = 0.f
            );
            UKismetSystemLibrary::DrawDebugLine(
                this,                                   // const UObject* WorldContextObject
                SmoothedPivotTarget.GetLocation(),      // const FVector LineStart
                PivotTarget.GetLocation(),              // const FVector LineEnd
                FLinearColor{1.f, 0.166667f, 0.f, 1.f}, // FLinearColor LineColor
                0.f,                                    // float Duration=0.f
                1.f                                     // float Thickness = 0.f
            );
            UKismetSystemLibrary::DrawDebugLine(
                this,                                   // const UObject* WorldContextObject
                PivotLocation,                          // const FVector LineStart
                SmoothedPivotTarget.GetLocation(),      // const FVector LineEnd
                FLinearColor{0.f, 0.666667f, 1.f, 1.f}, // FLinearColor LineColor
                0.f,                                    // float Duration=0.f
                1.f                                     // float Thickness = 0.f
            );
        }
    }

    // Step 8: Lerp First Person Override and return target camera parameters.
    FTransform L_Transform = UKismetMathLibrary::TLerp(
        FTransform(TargetCameraRotation, TargetCameraLocation, FVector{1.f}),
        FTransform(TargetCameraRotation, FPTarget, FVector{1.f}),
        GetCameraBehaviorParam(FName("Weight_FirstPerson")),
        ELerpInterpolationMode::QuatInterp);

    FTransform L_TransformResult = UKismetMathLibrary::TLerp(
        L_Transform,
        FTransform(DebugViewRotation, TargetCameraLocation, FVector{1.f}),
        GetCameraBehaviorParam(FName("Override_Debug")),
        ELerpInterpolationMode::QuatInterp);

    OutLocation = L_TransformResult.GetLocation();
    OutRotation = L_TransformResult.GetRotation().Rotator();
    OutFOV      = UKismetMathLibrary::Lerp(TPFOV, FPFOV, GetCameraBehaviorParam(FName("Weight_FirstPerson")));
}

FVector AALSPlayerCameraManager::CalculateAxisIndependentLag(FVector CurrentLocation, FVector TargetLocation, FRotator CameraRotation, FVector LagSpeeds) {
    FRotator CameraRotationYaw = {0.f, 0.f, CameraRotation.Yaw};

    FVector Vector1         = UKismetMathLibrary::LessLess_VectorRotator(CurrentLocation, CameraRotationYaw);
    FVector Vector2         = UKismetMathLibrary::LessLess_VectorRotator(TargetLocation, CameraRotationYaw);
    float WorldDeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(this);

    float X = UKismetMathLibrary::FInterpTo(Vector1.X, Vector2.X, WorldDeltaSeconds, LagSpeeds.X);
    float Y = UKismetMathLibrary::FInterpTo(Vector1.Y, Vector2.Y, WorldDeltaSeconds, LagSpeeds.Y);
    float Z = UKismetMathLibrary::FInterpTo(Vector1.Z, Vector2.Z, WorldDeltaSeconds, LagSpeeds.Z);

    return UKismetMathLibrary::GreaterGreater_VectorRotator(FVector{X, Y, Z}, CameraRotationYaw);
}

float AALSPlayerCameraManager::GetCameraBehaviorParam(FName CurveName) {
    // Get an Anim Curve value from the Player Camera Behavior AnimBP to use as a parameter in the custom camera behavior calculations
    if (CameraBehavior->GetAnimInstance()) {
        return CameraBehavior->GetAnimInstance()->GetCurveValue(CurveName);
    }
    return 0.f;
}

void AALSPlayerCameraManager::GetDebugTraceType(TEnumAsByte<EDrawDebugTrace::Type> ShowTraceType, TEnumAsByte<EDrawDebugTrace::Type>& DebugType) {
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
            DebugType = ShowTraceType;
        } else {
            DebugType = EDrawDebugTrace::Type::None;
        }
    }
}
