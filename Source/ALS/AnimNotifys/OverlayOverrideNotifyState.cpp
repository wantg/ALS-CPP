#include "ALS/AnimNotifys/OverlayOverrideNotifyState.h"
#include "Kismet/KismetStringLibrary.h"
#include "ALS/Interfaces/ALSAnimation.h"

UOverlayOverrideNotifyState::UOverlayOverrideNotifyState() {
    NotifyColor = FColor{200, 200, 255, 255};
}

FString UOverlayOverrideNotifyState::GetNotifyName_Implementation() const {
    return UKismetStringLibrary::Concat_StrStr("OverlayOverrideState: ", UKismetStringLibrary::Conv_IntToString(OverlayOverrideState));
}

void UOverlayOverrideNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) {
    if (IALSAnimation* ALSAnimation = Cast<IALSAnimation>(MeshComp->GetAnimInstance())) {
        ALSAnimation->SetOverlayOverrideState(OverlayOverrideState);
    }
}

void UOverlayOverrideNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
    if (IALSAnimation* ALSAnimation = Cast<IALSAnimation>(MeshComp->GetAnimInstance())) {
        ALSAnimation->SetOverlayOverrideState(0);
    }
}