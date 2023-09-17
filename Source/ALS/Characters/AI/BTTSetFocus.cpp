#include "ALS/Characters/AI/BTTSetFocus.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTSetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner())) {
        AIController->SetFocus(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
        return EBTNodeResult::Type::Succeeded;
    }
    return EBTNodeResult::Type::Failed;
}
