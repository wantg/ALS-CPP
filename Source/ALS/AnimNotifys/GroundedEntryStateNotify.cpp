#include "ALS/AnimNotifys/GroundedEntryStateNotify.h"
#include "Kismet/KismetStringLibrary.h"
#include "ALS/Interfaces/ALSAnimation.h"

UGroundedEntryStateNotify::UGroundedEntryStateNotify() {
    NotifyColor = FColor{255, 200, 200, 255};
}

FString UGroundedEntryStateNotify::GetNotifyName_Implementation() const {
    return UKismetStringLibrary::Concat_StrStr("Grounded Entry State: ", UEnum::GetDisplayValueAsText(GroundedEntryState).ToString());
}

void UGroundedEntryStateNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
    if (IALSAnimation* ALSAnimation = Cast<IALSAnimation>(MeshComp->GetAnimInstance())) {
        ALSAnimation->SetGroundedEntryState(GroundedEntryState);
    }
}