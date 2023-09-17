#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTSetFocus.generated.h"

UCLASS()
class ALS_API UBTTSetFocus : public UBTTaskNode {
    GENERATED_BODY()

   protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
