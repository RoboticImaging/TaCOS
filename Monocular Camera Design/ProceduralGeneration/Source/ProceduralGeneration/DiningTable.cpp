// An actor for a diningroom setup

#include "DiningTable.h"
#include "BoundingCreator.h"

// Sets default values
ADiningTable::ADiningTable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMeshComponent"));
	TableMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/TMChairsAndTablesPack1/ChairsAndTables/Tables/Tabledining15/SM_TableDining15'"));
	UStaticMesh* Asset = MeshAsset.Object;

	TableMesh->SetStaticMesh(Asset);

	ChairMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairMeshComponent"));
	ChairMesh->AttachToComponent(TableMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>ChairMeshAsset(TEXT("StaticMesh'/Game/TMChairsAndTablesPack1/ChairsAndTables/Chairs/Chair26/SM_Chair26'"));
	Asset = ChairMeshAsset.Object;

	ChairMesh->SetStaticMesh(Asset);
	ChairMesh->SetRelativeLocation(FVector(70.f, -50.f, 0.f));
	ChairMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

	ChairMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairMesh2Component"));
	ChairMesh2->AttachToComponent(TableMesh, FAttachmentTransformRules::KeepRelativeTransform);

	ChairMesh2->SetStaticMesh(Asset);
	ChairMesh2->SetRelativeLocation(FVector(0.f, -50.f, 0.f));
	ChairMesh2->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

	ChairMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairMesh3Component"));
	ChairMesh3->AttachToComponent(TableMesh, FAttachmentTransformRules::KeepRelativeTransform);

	ChairMesh3->SetStaticMesh(Asset);
	ChairMesh3->SetRelativeLocation(FVector(-70.f, -50.f, 0.f));
	ChairMesh3->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

	ChairMesh4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairMesh4Component"));
	ChairMesh4->AttachToComponent(TableMesh, FAttachmentTransformRules::KeepRelativeTransform);

	ChairMesh4->SetStaticMesh(Asset);
	ChairMesh4->SetRelativeLocation(FVector(-70.f, 50.f, 0.f));
	ChairMesh4->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	ChairMesh5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairMesh5Component"));
	ChairMesh5->AttachToComponent(TableMesh, FAttachmentTransformRules::KeepRelativeTransform);

	ChairMesh5->SetStaticMesh(Asset);
	ChairMesh5->SetRelativeLocation(FVector(0.f, 50.f, 0.f));
	ChairMesh5->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	ChairMesh6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairMesh6Component"));
	ChairMesh6->AttachToComponent(TableMesh, FAttachmentTransformRules::KeepRelativeTransform);

	ChairMesh6->SetStaticMesh(Asset);
	ChairMesh6->SetRelativeLocation(FVector(70.f, 50.f, 0.f));
	ChairMesh6->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	LaptopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaptopMeshComponent"));
	LaptopMesh->AttachToComponent(TableMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>LaptopMeshAsset(TEXT("StaticMesh'/Game/TMHighTechPack1/HighTech/meshes/Tablet02/SM_Tablet02'"));
	Asset = LaptopMeshAsset.Object;

	LaptopMesh->SetStaticMesh(Asset);
	LaptopMesh->SetRelativeLocation(FVector(0.f, 20.f, 75.f));

	PlantMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlantMeshComponent"));
	PlantMesh->AttachToComponent(TableMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PlantMeshAsset(TEXT("StaticMesh'/Game/LoftOffice/Meshes/SM_Plant_1'"));
	Asset = PlantMeshAsset.Object;

	PlantMesh->SetStaticMesh(Asset);
	PlantMesh->SetRelativeLocation(FVector(117.f, 4.f, 75.f));
	PlantMesh->SetRelativeRotation(FRotator(0.f, 115.2f, 0.f));
}

// Called when the game starts or when spawned
void ADiningTable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADiningTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ADiningTable::GetBoundingPointsOnScreen(int ResX, int ResY)
{
	FString BoxPositions = "";
	float RenderTime;

	RenderTime = 0.5;

	if (WasComponentRenderedRecently(RenderTime, TableMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Table", "5", TableMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, ChairMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Chair", "6", ChairMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, ChairMesh2))
	{
		BoxPositions += GetBoundingsOnScreen("Chair", "6", ChairMesh2, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, ChairMesh3))
	{
		BoxPositions += GetBoundingsOnScreen("Chair", "6", ChairMesh3, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, ChairMesh4))
	{
		BoxPositions += GetBoundingsOnScreen("Chair", "6", ChairMesh4, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, ChairMesh5))
	{
		BoxPositions += GetBoundingsOnScreen("Chair", "6", ChairMesh5, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, ChairMesh6))
	{
		BoxPositions += GetBoundingsOnScreen("Chair", "6", ChairMesh6, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, LaptopMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Computer", "7", LaptopMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, PlantMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Plant", "4", PlantMesh, GetWorld(), ResX, ResY);
	}

	return BoxPositions;
}