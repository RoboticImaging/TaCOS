// An actor for a bathroom setup
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bathroom2.generated.h"

UCLASS()
class PROCEDURALGENERATION_API ABathroom2 : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABathroom2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* SinkMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TapMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TapMesh2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TubMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BookMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TowelMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* HolderMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PineappleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PineappleTopMesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString GetBoundingPointsOnScreen(int ResX, int ResY);
};
