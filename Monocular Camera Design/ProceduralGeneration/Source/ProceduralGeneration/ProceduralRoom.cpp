// Generate interiors

#include "ProceduralRoom.h"
#include "DrawDebugHelpers.h"
#include "FloorNode.h"
#include "Floor.h"
#include "Wall.h"
#include "Door.h"
#include "Threshold.h"
#include "Window.h"
#include "Bed.h"
#include "Bed2.h"
#include "Bed3.h"
#include "Bathroom.h"
#include "Bathroom2.h"
#include "Bathroom3.h"
#include "Desk.h"
#include "Desk2.h"
#include "DiningTable.h"
#include "DiningTable2.h"
#include "Sofa.h"
#include "Sofa2.h"
#include "Sofa3.h"
#include "Sofa4.h"
#include "Sofa5.h"
#include "Sofa6.h"
#include "Engine/TargetPoint.h"
#include "HAL/FileManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
AProceduralRoom::AProceduralRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/FloorMesh'"));
	UStaticMesh* Asset = MeshAsset.Object;

	FloorMesh->SetStaticMesh(Asset);
	FloorMesh->SetRelativeLocation(FVector(750.f, 750.f, 0.f));
	FloorMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 0.1f));

	CeilingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CeilingMeshComponent"));
	CeilingMesh->AttachToComponent(CeilingMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CeilingMesh->SetStaticMesh(Asset);
	CeilingMesh->SetRelativeLocation(FVector(750.f, 750.f, 299.f));
	CeilingMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 0.1f));

	static ConstructorHelpers::FObjectFinder<UMaterial>CeilingMaterialAsset(TEXT("Material'/Game/LoftOffice/Materials/M_Wall'"));
	CeilingMaterial = CeilingMaterialAsset.Object;
	CeilingMesh->SetMaterial(0, CeilingMaterial);
}

// Called when the game starts or when spawned
void AProceduralRoom::BeginPlay()
{
	Super::BeginPlay();

	//SetActorTickInterval(1.f);

	GenerateInterior();
}

void AProceduralRoom::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

// Called every frame
void AProceduralRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AProceduralRoom::GetBoundingBox(int solution_num, int label_idx, int ResX, int ResY)
{
	TArray<AActor*> FoundActors;
	FString PositionsOnFrame = "";

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABed::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ABed* Bed = Cast<ABed>(ActorFound);

		PositionsOnFrame += Bed->GetBoundingPointsOnScreen(ResX, ResY);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABed2::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ABed2* Bed2 = Cast<ABed2>(ActorFound);

		PositionsOnFrame += Bed2->GetBoundingPointsOnScreen(ResX, ResY);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABed3::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ABed3* Bed3 = Cast<ABed3>(ActorFound);

		PositionsOnFrame += Bed3->GetBoundingPointsOnScreen(ResX, ResY);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABathroom::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ABathroom* Bathroom = Cast<ABathroom>(ActorFound);

		PositionsOnFrame += Bathroom->GetBoundingPointsOnScreen(ResX, ResY);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABathroom2::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ABathroom2* Bathroom2 = Cast<ABathroom2>(ActorFound);
		PositionsOnFrame += Bathroom2->GetBoundingPointsOnScreen(ResX, ResY);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABathroom3::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ABathroom3* Bathroom3 = Cast<ABathroom3>(ActorFound);
		PositionsOnFrame += Bathroom3->GetBoundingPointsOnScreen(ResX, ResY);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADesk::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ADesk* Desk = Cast<ADesk>(ActorFound);
		PositionsOnFrame += Desk->GetBoundingPointsOnScreen(ResX, ResY);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADesk2::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ADesk2* Desk2 = Cast<ADesk2>(ActorFound);
		PositionsOnFrame += Desk2->GetBoundingPointsOnScreen(ResX, ResY);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADiningTable::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ADiningTable* DiningTable = Cast<ADiningTable>(ActorFound);
		PositionsOnFrame += DiningTable->GetBoundingPointsOnScreen(ResX, ResY);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADiningTable2::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ADiningTable2* DiningTable2 = Cast<ADiningTable2>(ActorFound);
		PositionsOnFrame += DiningTable2->GetBoundingPointsOnScreen(ResX, ResY);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASofa::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ASofa* Sofa = Cast<ASofa>(ActorFound);
		PositionsOnFrame += Sofa->GetBoundingPointsOnScreen(ResX, ResY);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASofa2::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ASofa2* Sofa2 = Cast<ASofa2>(ActorFound);
		PositionsOnFrame += Sofa2->GetBoundingPointsOnScreen(ResX, ResY);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASofa3::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ASofa3* Sofa3 = Cast<ASofa3>(ActorFound);
		PositionsOnFrame += Sofa3->GetBoundingPointsOnScreen(ResX, ResY);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASofa4::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ASofa4* Sofa4 = Cast<ASofa4>(ActorFound);
		PositionsOnFrame += Sofa4->GetBoundingPointsOnScreen(ResX, ResY);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASofa5::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ASofa5* Sofa5 = Cast<ASofa5>(ActorFound);
		PositionsOnFrame += Sofa5->GetBoundingPointsOnScreen(ResX, ResY);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASofa6::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ASofa6* Sofa6 = Cast<ASofa6>(ActorFound);
		PositionsOnFrame += Sofa6->GetBoundingPointsOnScreen(ResX, ResY);
	}

	if (!PositionsOnFrame.IsEmpty())
	{
		SaveCSV(label_idx, solution_num, PositionsOnFrame);
		return true;
	}
	else
	{
		return false;
	}
}

void AProceduralRoom::CopyImage(int solution_num, int index, int train_index)
{
	FString TrainFilePath;

	TrainFilePath = "E:\\train_data/";
	TrainFilePath += FString::FromInt(solution_num);
	TrainFilePath += "_train/train";
	TrainFilePath += FString::FromInt(train_index);
	TrainFilePath += ".tif";

	FString FeatureFilePath;

	FeatureFilePath = "E:\\train_data/";
	FeatureFilePath += FString::FromInt(solution_num);
	FeatureFilePath += "/";
	FeatureFilePath += FString::FromInt(index);
	FeatureFilePath += ".tif";

	FPlatformFileManager::Get().GetPlatformFile().CopyFile(*FeatureFilePath, *TrainFilePath);
}

void AProceduralRoom::SaveCSV(int index, int solution_num, FString StringToSave)
{
	FString SaveDirectory;

	SaveDirectory = "E:\\train_data/";
	SaveDirectory += FString::FromInt(solution_num);
	SaveDirectory += "_annotations\\labels";

	SaveDirectory += FString::FromInt(index);
	SaveDirectory += ".csv";

	FFileHelper::SaveStringToFile(StringToSave, *SaveDirectory);
}

void AProceduralRoom::GenerateInterior()
{
	TSharedPtr<Floor> TheFloor(new Floor());
	TheFloor->Partition();

	TheFloor->DrawFloorNodes(GetWorld());

	HorizontalWindows = TheFloor->GetHorizontalWindows();
	VerticalWindows = TheFloor->GetVerticalWindows();

	SpawnWalls(TheFloor->GetHorizontalWallLocations(), TheFloor->GetVerticalWallLocations(), TheFloor->GetHorizontalDoors(), TheFloor->GetVerticalDoors());
	SpawnFurnitures(TheFloor->GetFurnitureTransforms());
}

void AProceduralRoom::SpawnWalls(TArray<FVector> HorizontalLocations, TArray<FVector> VerticalLocations, TArray<FVector> HorizontalDoors, TArray<FVector> VerticalDoors)
{
	FRotator HorizontalRotation(0.f, 0.f, 0.f);
	FRotator VerticalRotation(0.f, 90.f, 0.f);

	for(int i = 0; i < HorizontalLocations.Num(); i++)
	{	
		if (HorizontalWindows.Contains(HorizontalLocations[i]))
		{
			GetWorld()->SpawnActor<AWindow>(HorizontalLocations[i], HorizontalRotation);
		}
		else if (HorizontalDoors.Contains(HorizontalLocations[i]))
		{
			GetWorld()->SpawnActor<ADoor>(HorizontalLocations[i], HorizontalRotation);
			GetWorld()->SpawnActor<AThreshold>(HorizontalLocations[i], HorizontalRotation);
		}
		else
		{
			GetWorld()->SpawnActor<AWall>(HorizontalLocations[i], HorizontalRotation);
		}
	}

	for (int i = 0; i < VerticalLocations.Num(); i++)
	{
		if (VerticalWindows.Contains(VerticalLocations[i]))
		{
			GetWorld()->SpawnActor<AWindow>(VerticalLocations[i], VerticalRotation);
		}
		else if (VerticalDoors.Contains(VerticalLocations[i]))
		{
			GetWorld()->SpawnActor<ADoor>(VerticalLocations[i], VerticalRotation);
			GetWorld()->SpawnActor<AThreshold>(VerticalLocations[i], VerticalRotation);
		}
		else
		{
			GetWorld()->SpawnActor<AWall>(VerticalLocations[i], VerticalRotation);
		}
	}
}

void AProceduralRoom::SpawnFurnitures(TArray<FFurnitureTransform> FurnitureTransforms)
{
	for (int i = 0; i < FurnitureTransforms.Num(); i++)
	{
		int index = FMath::FRandRange(0.f, 15.f);

		if (index == 0)
		{
			GetWorld()->SpawnActor<ABed>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
		else if (index == 1)
		{
			GetWorld()->SpawnActor<ABed2>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
		else if (index == 2)
		{
			GetWorld()->SpawnActor<ABed3>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
		else if (index == 3)
		{
			GetWorld()->SpawnActor<ABathroom>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
		else if (index == 4)
		{
			GetWorld()->SpawnActor<ABathroom2>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
		else if (index == 5)
		{
			GetWorld()->SpawnActor<ABathroom3>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
		else if (index == 6)
		{
			GetWorld()->SpawnActor<ADesk>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
		else if (index == 7)
		{
			GetWorld()->SpawnActor<ADesk2>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
		else if (index == 8)
		{
			GetWorld()->SpawnActor<ADiningTable>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
		else if (index == 9)
		{
			GetWorld()->SpawnActor<ADiningTable2>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
		else if (index == 10)
		{
			GetWorld()->SpawnActor<ASofa>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
		else if (index == 11)
		{
			GetWorld()->SpawnActor<ASofa2>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
		else if (index == 12)
		{
			GetWorld()->SpawnActor<ASofa3>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
		else if (index == 13)
		{
			GetWorld()->SpawnActor<ASofa4>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
		else if (index == 14)
		{
			GetWorld()->SpawnActor<ASofa5>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
		else if (index == 15)
		{
			GetWorld()->SpawnActor<ASofa6>(FurnitureTransforms[i].Location, FurnitureTransforms[i].Rotation);
		}
	}
}

void AProceduralRoom::DestroyActors()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWall::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AThreshold::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWindow::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABed::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABed2::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABed3::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABathroom::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABathroom2::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABathroom3::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADesk::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADesk2::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADiningTable::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADiningTable2::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASofa::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASofa2::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASofa3::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASofa4::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASofa5::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASofa6::StaticClass(), FoundActors);

	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}
}



