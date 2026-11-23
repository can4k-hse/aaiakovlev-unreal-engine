#include "FindSearchPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UFindSearchPoint::UFindSearchPoint()
{
    NodeName = TEXT("Investigate the last clue");
}

EBTNodeResult::Type UFindSearchPoint::ExecuteTask(UBehaviorTreeComponent& Tree, uint8* Memory)
{
    auto* AI = Tree.GetAIOwner();
    APawn* Guard = AI ? AI->GetPawn() : nullptr;
    auto* Board = Tree.GetBlackboardComponent();
    auto* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Tree.GetWorld());
    if (!Guard || !Board || !Nav || !Board->IsVectorValueSet(TEXT("ClueLocation")))
        return EBTNodeResult::Failed;

    FNavLocation Destination;
    if (!Nav->ProjectPointToNavigation(Board->GetValueAsVector(TEXT("ClueLocation")), Destination, FVector(140, 140, 250)))
        return EBTNodeResult::Failed;

    // Approach the clue first, then inspect reachable points around it.
    if (FVector::DistSquared2D(Guard->GetActorLocation(), Destination.Location) < FMath::Square(360.f))
    {
        FNavLocation Nearby;
        if (Nav->GetRandomReachablePointInRadius(Destination.Location, 320.f, Nearby))
            Destination = Nearby;
    }
    Board->SetValueAsVector(TEXT("SearchPoint"), Destination.Location);
    return EBTNodeResult::Succeeded;
}
