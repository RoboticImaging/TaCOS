// A threshold actor acts as a obstacle, interacts with the character upon collision

#pragma once

#include "CoreMinimal.h"
#include "ProceduralGenerationCharacter.h"
#include "GameFramework/Actor.h"
#include "Threshold.generated.h"

UCLASS()
class PROCEDURALGENERATION_API AThreshold : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThreshold();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	AProceduralGenerationCharacter* ProceduralGenerationCharacter;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// declare overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	bool WasComponentRenderedRecently(float tolerance);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BoxMesh;
};
