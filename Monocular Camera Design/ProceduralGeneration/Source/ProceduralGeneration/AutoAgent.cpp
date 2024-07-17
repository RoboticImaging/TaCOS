// AI controller for character

#include "AutoAgent.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

void AAutoAgent::BeginPlay()
{
	Super::BeginPlay();

	index = 0;
	regenerate = 30;
	isStop = false;

	ProceduralGenerationCharacter = Cast<AProceduralGenerationCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AProceduralGenerationCharacter::StaticClass()));
	ProceduralRoom = Cast<AProceduralRoom>(UGameplayStatics::GetActorOfClass(GetWorld(), AProceduralRoom::StaticClass()));

	NavArea = UNavigationSystemV1::GetCurrent(GetWorld());

	GoToRandomWaypoint();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), Waypoints);
}

void AAutoAgent::GoToRandomWaypoint()
{
	NavArea->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), 2000.f, RandomLocaiton);

	MoveToLocation(RandomLocaiton.Location);
}

ATargetPoint* AAutoAgent::GetRandomWaypoint()
{
	return Cast<ATargetPoint>(Waypoints[0]);
}

void AAutoAgent::GoToTargetPoint()
{
	MoveToActor(GetRandomWaypoint());
}

void AAutoAgent::SetStop(bool ifStopped)
{
	isStop = ifStopped;
}

void AAutoAgent::ResetMovingTarget()
{
	index = regenerate - 1;
	GoToRandomWaypoint();
}

void AAutoAgent::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	index++;

	NavArea = UNavigationSystemV1::GetCurrent(GetWorld());

	if (index < regenerate)
	{
		GoToRandomWaypoint();
	}
	else if (index == regenerate)
	{
		GoToTargetPoint();
	}
	else if (index > regenerate && !isStop)
	{	
		index = 0;

		ProceduralGenerationCharacter->ChangeCameraHeight();
		ProceduralRoom->DestroyActors();
		ProceduralRoom->GenerateInterior();

		NavArea = UNavigationSystemV1::GetCurrent(GetWorld());
		GoToRandomWaypoint();
	}
}