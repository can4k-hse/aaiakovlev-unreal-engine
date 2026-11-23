#include "GuardAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

void AGuardAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (!InPawn || !GuardTree || !GuardTree->BlackboardAsset)
    {
        return;
    }

    UBlackboardComponent* Memory = nullptr;
    if (UseBlackboard(GuardTree->BlackboardAsset, Memory) && Memory)
    {
        Memory->SetValueAsVector(TEXT("HomeLocation"), InPawn->GetActorLocation());
        RunBehaviorTree(GuardTree);
    }
}
