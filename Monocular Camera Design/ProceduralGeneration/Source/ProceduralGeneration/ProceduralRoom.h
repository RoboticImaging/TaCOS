// Generate interiors

#pragma once

#include "CoreMinimal.h"
#include "Floor.h"
#include "Wall.h"
#include "Door.h"
#include "Threshold.h"
#include "Window.h"
#include "Bed.h"
#include "Bathroom.h"
#include "Bathroom2.h"
#include "Desk.h"
#include "Desk2.h"
#include "DiningTable.h"
#include "Sofa.h"
#include "Sofa2.h"
#include "Sofa3.h"
#include "ProceduralGenerationCharacter.h"
#include "GameFramework/Actor.h"
#include "ProceduralRoom.generated.h"

UCLASS()
class PROCEDURALGENERATION_API AProceduralRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Room, meta = (AllowPrivateAccess = "true"));
	UStaticMeshComponent* FloorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CeilingMesh;

	UFUNCTION(Exec, BlueprintCallable, Category = "Object Detect")
	bool GetBoundingBox(int solution_num, int label_idx, int ResX, int ResY);

	UFUNCTION(Exec, BlueprintCallable, Category = "Object Detect")
	void CopyImage(int solution_num, int index, int train_index);

	UFUNCTION(Exec, BlueprintCallable, Category = "Object Detect")
	void SaveCSV(int index, int solution_num, FString StringToSave);

	UFUNCTION(Exec, BlueprintCallable, Category = "Generation")
	void GenerateInterior();

	UFUNCTION(Exec, BlueprintCallable, Category = "Generation")
	void DestroyActors();

private:
	/*TArray<TSubclassOf<AActor>> FurnitureType;*/
	TArray<FVector> VerticalWindows, HorizontalWindows;
	TArray<FString> SaveBoxPosition;

	void SpawnWalls(TArray<FVector> HorizontalLocations, TArray<FVector> VerticalLocations, TArray<FVector> HorizontalDoors, TArray<FVector> VerticalDoors);
	void SpawnFurnitures(TArray<FFurnitureTransform> FurnitureTransforms);

	UMaterial* CeilingMaterial;
};
