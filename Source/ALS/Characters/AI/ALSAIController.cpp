#include "ALS/Characters/AI/ALSAIController.h"
#include "BehaviorTree/BehaviorTree.h"

void AALSAIController::OnPossess(APawn* InPawn) {
    Super::OnPossess(InPawn);
    RunBehaviorTree(LoadObject<UBehaviorTree>(nullptr, TEXT("/Game/AdvancedLocomotionV4/Blueprints/CharacterLogic/AI/ALS_BT_AICharacter")));
}
