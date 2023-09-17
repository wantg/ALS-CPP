#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "OverlayOverrideNotifyState.generated.h"

UCLASS()
class ALS_API UOverlayOverrideNotifyState : public UAnimNotifyState {
    GENERATED_BODY()

    UOverlayOverrideNotifyState();

    FString GetNotifyName_Implementation() const;

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

   public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 OverlayOverrideState = 0;
};
