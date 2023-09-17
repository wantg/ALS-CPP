#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ALSAIController.generated.h"

UCLASS()
class ALS_API AALSAIController : public AAIController {
    GENERATED_BODY()

   protected:
    virtual void OnPossess(APawn* InPawn) override;
};
