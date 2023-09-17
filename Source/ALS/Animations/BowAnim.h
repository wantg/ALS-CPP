#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BowAnim.generated.h"

UCLASS()
class ALS_API UBowAnim : public UAnimInstance {
    GENERATED_BODY()

   public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Draw = 0.f;
};