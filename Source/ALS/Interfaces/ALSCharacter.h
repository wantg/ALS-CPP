#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ALS/Data/ALSEnums.h"
#include "ALSCharacter.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UALSCharacter : public UInterface {
    GENERATED_BODY()
};

class ALS_API IALSCharacter {
    GENERATED_BODY()

   public:
    UFUNCTION(BlueprintCallable, Category = "Character Information")
    virtual void GetCurrentStates(TEnumAsByte<EMovementMode>& PawnMovementMode, EALSMovementState& MovementState, EALSMovementState& PrevMovementState, EALSMovementAction& MovementAction, EALSRotationMode& RotationMode, EALSGait& ActualGait, EALSStance& ActualStance, EALSViewMode& ViewMode, EALSOverlayState& OverlayState) = 0;

    UFUNCTION(BlueprintCallable, Category = "Character Information")
    virtual void GetEssentialValues(FVector& Velocity, FVector& Acceleration, FVector& MovementInput, bool& IsMoving, bool& HasMovementInput, float& Speed, float& MovementInputAmount, FRotator& AimingRotation, float& AimYawRate) = 0;

    UFUNCTION(BlueprintCallable, Category = "Character States")
    virtual void SetMovementState(EALSMovementState NewMovementState) = 0;

    UFUNCTION(BlueprintCallable, Category = "Character States")
    virtual void SetMovementAction(EALSMovementAction NewMovementAction) = 0;

    UFUNCTION(BlueprintCallable, Category = "Character States")
    virtual void SetRotationMode(EALSRotationMode NewRotationMode) = 0;

    UFUNCTION(BlueprintCallable, Category = "Character States")
    virtual void SetGait(EALSGait NewGait) = 0;

    UFUNCTION(BlueprintCallable, Category = "Character States")
    virtual void SetViewMode(EALSViewMode NewViewMode) = 0;

    UFUNCTION(BlueprintCallable, Category = "Character States")
    virtual void SetOverlayState(EALSOverlayState NewOverlayState) = 0;
};
