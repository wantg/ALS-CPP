#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ALS/Data/ALSEnums.h"
#include "EarlyBlendOutNotifyState.generated.h"

UCLASS()
class ALS_API UEarlyBlendOutNotifyState : public UAnimNotifyState {
    GENERATED_BODY()

    UEarlyBlendOutNotifyState();

    FString GetNotifyName_Implementation() const;

    virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

   public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UAnimMontage* ThisMontage;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float BlendOutTime = 0.25f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool CheckMovementState = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    EALSMovementState MovementStateEquals = EALSMovementState::EAMS_None;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool CheckStance = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    EALSStance StanceEquals = EALSStance::EAS_Standing;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool CheckMovementInput = false;
};
