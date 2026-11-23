#include "GuardAwarenessService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "aaiakovlev.h"

UGuardAwarenessService::UGuardAwarenessService()
{
    NodeName = TEXT("Watch and listen for the player");
    bCreateNodeInstance = true;
    bNotifyTick = true;
    Interval = 0.2f;
    RandomDeviation = 0.f;
}

void UGuardAwarenessService::TickNode(UBehaviorTreeComponent& Tree, uint8* Memory, float DeltaSeconds)
{
    Super::TickNode(Tree, Memory, DeltaSeconds);
    auto* AI = Tree.GetAIOwner();
    auto* Guard = AI ? Cast<ACharacter>(AI->GetPawn()) : nullptr;
    auto* Board = Tree.GetBlackboardComponent();
    if (!Guard || !Board) return;

    auto* Player = Cast<Aaaiakovlev>(UGameplayStatics::GetPlayerPawn(Guard, 0));
    const double Now = Guard->GetWorld()->GetTimeSeconds();
    bool Visible = false;
    if (Player)
    {
        const FVector Offset = Player->GetActorLocation() - Guard->GetActorLocation();
        const bool Tracking = Board->GetValueAsObject(TEXT("TargetActor")) == Player;
        const float SightRadius = Tracking ? 1350.f : 950.f;
        const bool InView = Tracking || Offset.SizeSquared() < FMath::Square(180.f)
            || FVector::DotProduct(Guard->GetActorForwardVector(), Offset.GetSafeNormal2D()) >= 0.42f;
        Visible = InView && Offset.SizeSquared() <= FMath::Square(SightRadius)
            && AI->LineOfSightTo(Player);
        if (Visible)
        {
            Board->SetValueAsVector(TEXT("ClueLocation"), Player->GetActorLocation());
            Board->SetValueAsObject(TEXT("TargetActor"), Player);
            LastClueTime = Now;
        }
        else if (Player->LastFootstepTime > LastFootstepHeard && Now - Player->LastFootstepTime <= 0.55
            && FVector::DistSquared(Guard->GetActorLocation(), Player->LastFootstepLocation) <= FMath::Square(1500.f))
        {
            LastFootstepHeard = Player->LastFootstepTime;
            LastClueTime = Now;
            Board->SetValueAsVector(TEXT("ClueLocation"), Player->LastFootstepLocation);
        }
    }
    if (!Visible) Board->ClearValue(TEXT("TargetActor"));
    if (Now - LastClueTime >= 7.) Board->ClearValue(TEXT("ClueLocation"));
    Guard->GetCharacterMovement()->MaxWalkSpeed = Visible ? 620.f : 260.f;
}
