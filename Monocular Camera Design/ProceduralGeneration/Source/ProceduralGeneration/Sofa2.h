// An actor for a living room setup

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sofa2.generated.h"

UCLASS()
class PROCEDURALGENERATION_API ASofa2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASofa2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* SofaMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* TableMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* CubeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* CabinMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* TVMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* PlantMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* PlantMesh2;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FString GetBoundingPointsOnScreen(int ResX, int ResY);
};
