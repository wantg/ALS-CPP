#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ALS/Data/Enums.h"
#include "ALSPlayerCameraBehavior.generated.h"

UCLASS()
class ALS_API UALSPlayerCameraBehavior : public UAnimInstance {
    GENERATED_BODY()

   public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

   protected:
    void UpdateCharacterInfo();

   public:
    APlayerController* PlayerController;
    APawn* ControlledPawn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Info")
    EALSMovementState MovementState = EALSMovementState::EAMS_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Info")
    EALSMovementAction MovementAction = EALSMovementAction::EAMA_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Info")
    EALSRotationMode RotationMode = EALSRotationMode::EARM_VelocityDirection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Info")
    EALSGait Gait = EALSGait::EAG_Walking;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Info")
    EALSStance Stance = EALSStance::EAS_Standing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Info")
    EALSViewMode ViewMode = EALSViewMode::EAVM_ThirdPerson;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Info")
    bool RightShoulder = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Info")
    bool DebugView = false;
};
