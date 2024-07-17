// An actor for a bathroom setup

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bathroom.generated.h"

UCLASS()
class PROCEDURALGENERATION_API ABathroom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABathroom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* SinkMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* TapMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* TubMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* PlantMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* BookMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* BookMesh2;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString GetBoundingPointsOnScreen(int ResX, int ResY);
};
