#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ALSPlayerCameraManager.generated.h"

UCLASS()
class ALS_API AALSPlayerCameraManager : public APlayerCameraManager {
    GENERATED_BODY()

   public:
    AALSPlayerCameraManager();
    void OnPossess(APawn* NewPawn);

   protected:
    virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;
    void CustomCameraBehavior(FVector& OutLocation, FRotator& OutRotation, float& OutFOV);
    FVector CalculateAxisIndependentLag(FVector CurrentLocation, FVector TargetLocation, FRotator CameraRotation, FVector LagSpeeds);
    float GetCameraBehaviorParam(FName CurveName);
    void GetDebugTraceType(TEnumAsByte<EDrawDebugTrace::Type> ShowTraceType, TEnumAsByte<EDrawDebugTrace::Type>& DebugType);

   public:
    USkeletalMeshComponent* CameraBehavior;
    APawn* ControlledPawn;
    FVector DebugViewOffset = {350.f, 0.f, 50.f};
    FRotator DebugViewRotation = {-5.f, 180.f, 0.f};
    FVector RootLocation = FVector::ZeroVector;
    FTransform SmoothedPivotTarget = {FRotator::ZeroRotator, FVector::ZeroVector, FVector{1.f}};
    FVector PivotLocation = FVector::ZeroVector;
    FVector TargetCameraLocation = FVector::ZeroVector;
    FRotator TargetCameraRotation = FRotator::ZeroRotator;
};
