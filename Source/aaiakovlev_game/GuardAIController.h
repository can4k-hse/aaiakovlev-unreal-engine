#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GuardAIController.generated.h"

class UBehaviorTree;

UCLASS()
class AAIAKOVLEV_GAME_API AGuardAIController : public AAIController
{
    GENERATED_BODY()

protected:
    virtual void OnPossess(APawn* InPawn) override;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    TObjectPtr<UBehaviorTree> GuardTree;
};
