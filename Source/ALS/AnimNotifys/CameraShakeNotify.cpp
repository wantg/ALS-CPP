#include "ALS/AnimNotifys/CameraShakeNotify.h"

UCameraShakeNotify::UCameraShakeNotify() {
    NotifyColor = FColor{243, 167, 0, 255};
}

void UCameraShakeNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
    if (APawn* Pawn = Cast<APawn>(MeshComp->GetOwner())) {
        if (APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController())) {
            PlayerController->ClientStartCameraShake(ShakeClass, Scale, ECameraShakePlaySpace::CameraLocal, FRotator::ZeroRotator);
        }
    }
}