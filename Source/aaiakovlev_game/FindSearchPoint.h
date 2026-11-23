#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "FindSearchPoint.generated.h"

UCLASS()
class AAIAKOVLEV_GAME_API UFindSearchPoint : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UFindSearchPoint();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Tree, uint8* Memory) override;
};
