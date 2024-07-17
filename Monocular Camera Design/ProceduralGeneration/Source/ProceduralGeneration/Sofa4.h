// An actor for a living room setup

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sofa4.generated.h"

UCLASS()
class PROCEDURALGENERATION_API ASofa4 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASofa4();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* SofaMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TableMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* UniconMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FString GetBoundingPointsOnScreen(int ResX, int ResY);
};
