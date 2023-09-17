#include "ALS/Characters/AI/BTTGetRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

FString UBTTGetRandomLocation::GetStaticDescription() const {
    return FString::Printf(TEXT("Radius: %f\nMove To Location: %s"), Radius, *MoveToLocation.SelectedKeyName.ToString());
}

EBTNodeResult::Type UBTTGetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner())) {
        if (APawn* Pawn = Cast<APawn>(AIController->GetPawn())) {
            FVector ResultLocation;
            if (UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, Pawn->GetActorLocation(), ResultLocation, Radius, nullptr, nullptr)) {
                Location = ResultLocation;
                Blackboard->SetValueAsVector(MoveToLocation.SelectedKeyName, Location);
                return EBTNodeResult::Type::Succeeded;
            }
        }
    }
    return EBTNodeResult::Type::Failed;
}