// AI controller for character

#pragma once

#include "CoreMinimal.h"
#include "ProceduralGenerationCharacter.h"
#include "ProceduralRoom.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Engine/TargetPoint.h"
#include "AutoAgent.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALGENERATION_API AAutoAgent : public AAIController
{
	GENERATED_BODY()

protected:
    void BeginPlay() override;

public:
    AProceduralGenerationCharacter* ProceduralGenerationCharacter;
    AProceduralRoom* ProceduralRoom;

    void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

    UFUNCTION()
    void GoToRandomWaypoint();

    UFUNCTION()
    ATargetPoint* GetRandomWaypoint();

    UFUNCTION()
    void GoToTargetPoint();

    UFUNCTION(Exec, BlueprintCallable, Category = "Autoagent")
    void SetStop(bool ifStopped);

    UFUNCTION(Exec, BlueprintCallable, Category = "Autoagent")
    void ResetMovingTarget();

    UPROPERTY()
    TArray<AActor*> Waypoints;

private:
    UNavigationSystemV1* NavArea;
    FNavLocation RandomLocaiton;
    FTimerHandle TimerHandle;
    int index;
    int regenerate;
    bool isStop;
    TArray<FString> SaveText;
	
};
