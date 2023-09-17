#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ALS/Data/Enums.h"
#include "GroundedEntryStateNotify.generated.h"

UCLASS()
class ALS_API UGroundedEntryStateNotify : public UAnimNotify {
    GENERATED_BODY()

    UGroundedEntryStateNotify();

    FString GetNotifyName_Implementation() const;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

   public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    EGroundedEntryState GroundedEntryState = EGroundedEntryState::EGES_None;
};
