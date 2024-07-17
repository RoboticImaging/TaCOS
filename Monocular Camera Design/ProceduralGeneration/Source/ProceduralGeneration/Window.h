// Window actor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Window.generated.h"

UCLASS()
class PROCEDURALGENERATION_API AWindow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWindow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* FrameMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WindowMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
