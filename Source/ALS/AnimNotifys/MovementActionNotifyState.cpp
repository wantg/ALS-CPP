#include "ALS/AnimNotifys/MovementActionNotifyState.h"
#include "Kismet/KismetStringLibrary.h"
#include "ALS/Interfaces/ALSCharacter.h"

UMovementActionNotifyState::UMovementActionNotifyState() {
    NotifyColor = FColor{200, 200, 255, 255};
}

FString UMovementActionNotifyState::GetNotifyName_Implementation() const {
    return UKismetStringLibrary::Concat_StrStr("Movement Action: ", UEnum::GetDisplayValueAsText(MovementAction).ToString());
}

void UMovementActionNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) {
    if (IALSCharacter* ALSCharacter = Cast<IALSCharacter>(MeshComp->GetOwner())) {
        ALSCharacter->SetMovementAction(MovementAction);
    }
}

void UMovementActionNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
    if (IALSCharacter* ALSCharacter = Cast<IALSCharacter>(MeshComp->GetOwner())) {
        TEnumAsByte<EMovementMode> PawnMovementMode;
        EALSMovementState MovementState;
        EALSMovementState PrevMovementState;
        EALSMovementAction L_MovementAction;
        EALSRotationMode RotationMode;
        EALSGait ActualGait;
        EALSStance ActualStance;
        EALSViewMode ViewMode;
        EALSOverlayState OverlayState;
        ALSCharacter->GetCurrentStates(PawnMovementMode, MovementState, PrevMovementState, L_MovementAction, RotationMode, ActualGait, ActualStance, ViewMode, OverlayState);
        if (MovementAction == L_MovementAction) {
            ALSCharacter->SetMovementAction(EALSMovementAction::EAMA_None);
        }
    }
}