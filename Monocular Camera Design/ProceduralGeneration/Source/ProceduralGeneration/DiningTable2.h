// An actor for a diningroom setup

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DiningTable2.generated.h"

UCLASS()
class PROCEDURALGENERATION_API ADiningTable2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADiningTable2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TableMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ChairMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ChairMesh2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ChairMesh3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ChairMesh4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PlantMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FString GetBoundingPointsOnScreen(int ResX, int ResY);
};
