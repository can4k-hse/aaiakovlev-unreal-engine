#pragma once

#include "BehaviorTree/BTService.h"
#include "GuardAwarenessService.generated.h"

UCLASS()
class AAIAKOVLEV_GAME_API UGuardAwarenessService : public UBTService
{
    GENERATED_BODY()

public:
    UGuardAwarenessService();

protected:
    virtual void TickNode(UBehaviorTreeComponent& Tree, uint8* Memory, float DeltaSeconds) override;

private:
    double LastClueTime = -100.;
    double LastFootstepHeard = -100.;
};
