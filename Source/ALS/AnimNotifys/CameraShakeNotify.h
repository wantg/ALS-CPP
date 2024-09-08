#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ALS/Misc/ALSSprintCameraShake.h"
#include "CameraShakeNotify.generated.h"

UCLASS()
class ALS_API UCameraShakeNotify : public UAnimNotify {
    GENERATED_BODY()

    UCameraShakeNotify();

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

   public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TSubclassOf<ULegacyCameraShake> ShakeClass = UALSSprintCameraShake::StaticClass();

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Scale = 1.f;
};
