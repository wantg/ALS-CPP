#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTGetRandomLocation.generated.h"

UCLASS()
class ALS_API UBTTGetRandomLocation : public UBTTaskNode {
    GENERATED_BODY()

    virtual FString GetStaticDescription() const override;

   protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

   protected:
    UPROPERTY(EditAnywhere)
    float Radius = 1000.f;

    UPROPERTY(EditAnywhere)
    FBlackboardKeySelector MoveToLocation;

    FVector Location = FVector::ZeroVector;
};
