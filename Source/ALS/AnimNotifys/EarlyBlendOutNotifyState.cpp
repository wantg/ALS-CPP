#include "ALS/AnimNotifys/EarlyBlendOutNotifyState.h"
#include "ALS/Interfaces/ALSCharacter.h"

UEarlyBlendOutNotifyState::UEarlyBlendOutNotifyState() {
    NotifyColor = FColor{200, 200, 255, 255};
}

FString UEarlyBlendOutNotifyState::GetNotifyName_Implementation() const {
    return "Early Blend Out";
}

void UEarlyBlendOutNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) {
    UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
    AActor* OwningActor = MeshComp->GetOwner();

    if (CheckMovementInput) {
        if (IALSCharacter* ALSCharacter = Cast<IALSCharacter>(OwningActor)) {
            TEnumAsByte<EMovementMode> PawnMovementMode;
            EALSMovementState MovementState;
            EALSMovementState PrevMovementState;
            EALSMovementAction MovementAction;
            EALSRotationMode RotationMode;
            EALSGait ActualGait;
            EALSStance ActualStance;
            EALSViewMode ViewMode;
            EALSOverlayState OverlayState;
            ALSCharacter->GetCurrentStates(PawnMovementMode, MovementState, PrevMovementState, MovementAction, RotationMode, ActualGait, ActualStance, ViewMode, OverlayState);
            if (MovementState == MovementStateEquals) {
                AnimInstance->Montage_Stop(BlendOutTime, ThisMontage);
            }
        }
    }

    if (CheckStance) {
        if (IALSCharacter* ALSCharacter = Cast<IALSCharacter>(OwningActor)) {
            TEnumAsByte<EMovementMode> PawnMovementMode;
            EALSMovementState MovementState;
            EALSMovementState PrevMovementState;
            EALSMovementAction MovementAction;
            EALSRotationMode RotationMode;
            EALSGait ActualGait;
            EALSStance ActualStance;
            EALSViewMode ViewMode;
            EALSOverlayState OverlayState;
            ALSCharacter->GetCurrentStates(PawnMovementMode, MovementState, PrevMovementState, MovementAction, RotationMode, ActualGait, ActualStance, ViewMode, OverlayState);
            if (ActualStance == StanceEquals) {
                AnimInstance->Montage_Stop(BlendOutTime, ThisMontage);
            }
        }
    }

    if (CheckMovementInput) {
        if (IALSCharacter* ALSCharacter = Cast<IALSCharacter>(OwningActor)) {
            FVector Velocity;
            FVector Acceleration;
            FVector MovementInput;
            bool IsMoving;
            bool HasMovementInput;
            float Speed;
            float MovementInputAmount;
            FRotator AimingRotation;
            float AimYawRate;
            ALSCharacter->GetEssentialValues(Velocity, Acceleration, MovementInput, IsMoving, HasMovementInput, Speed, MovementInputAmount, AimingRotation, AimYawRate);
            if (HasMovementInput) {
                AnimInstance->Montage_Stop(BlendOutTime, ThisMontage);
            }
        }
    }
}