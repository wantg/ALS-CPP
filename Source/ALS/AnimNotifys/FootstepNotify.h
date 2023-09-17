#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ALS/Data/Enums.h"
#include "FootstepNotify.generated.h"

UCLASS()
class ALS_API UFootstepNotify : public UAnimNotify {
    GENERATED_BODY()

    UFootstepNotify();

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

   public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    USoundBase* Sound;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName AttachPointName = "root";

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    EFootstepType FootstepType = EFootstepType::EFT_Step;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float VolumeMultiplier = 1.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float PitchMultiplier = 1.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool OverrideMaskCurve = false;
};
