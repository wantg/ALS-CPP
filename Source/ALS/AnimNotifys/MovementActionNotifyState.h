#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ALS/Data/Enums.h"
#include "MovementActionNotifyState.generated.h"

UCLASS()
class ALS_API UMovementActionNotifyState : public UAnimNotifyState {
    GENERATED_BODY()

    UMovementActionNotifyState();

    FString GetNotifyName_Implementation() const;

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

   public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    EALSMovementAction MovementAction = EALSMovementAction::EAMA_None;
};
