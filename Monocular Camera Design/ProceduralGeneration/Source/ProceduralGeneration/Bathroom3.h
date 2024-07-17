// An actor for a bathroom setup

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bathroom3.generated.h"

UCLASS()
class PROCEDURALGENERATION_API ABathroom3 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABathroom3();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* SinkMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TapMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TubMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString GetBoundingPointsOnScreen(int ResX, int ResY);
};
