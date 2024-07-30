// An actor for a living room setup

#include "Sofa2.h"
#include "BoundingCreator.h"

// Sets default values
ASofa2::ASofa2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SofaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SofaMeshComponent"));
	SofaMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/LoftOffice/Meshes/SM_Sofa_2'"));
	UStaticMesh* Asset = MeshAsset.Object;

	SofaMesh->SetStaticMesh(Asset);

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMeshComponent"));
	TableMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TableMeshAsset(TEXT("StaticMesh'/Game/TMChairsAndTablesPack1/ChairsAndTables/Tables/TableLow11/SM_TableLow11'"));
	Asset = TableMeshAsset.Object;

	TableMesh->SetStaticMesh(Asset);
	TableMesh->SetRelativeLocation(FVector(0.f, 100.f, 0.f));
	TableMesh->SetRelativeScale3D(FVector(1.f, 0.5f, 1.f));

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMeshComponent"));
	CubeMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshAsset(TEXT("StaticMesh'/Game/Sofa_Cube'"));
	Asset = CubeMeshAsset.Object;

	CubeMesh->SetStaticMesh(Asset);
	CubeMesh->SetRelativeLocation(FVector(0.f, 0.f, 20.f));
	CubeMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	CubeMesh->SetRelativeScale3D(FVector(2.135f, 1.f, 0.4175f));

	CabinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CabinMeshComponent"));
	CabinMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CabinMeshAsset(TEXT("StaticMesh'/Game/TMChairsAndTablesPack1/ChairsAndTables/Tables/TableLow02/SM_TableLow02'"));
	Asset = CabinMeshAsset.Object;

	CabinMesh->SetStaticMesh(Asset);
	CabinMesh->SetRelativeLocation(FVector(0.f, 170.f, 0.f));

	TVMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TVMeshComponent"));
	TVMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TVMeshAsset(TEXT("StaticMesh'/Game/TMHighTechPack1/HighTech/meshes/TV01/SM_TV01'"));
	Asset = TVMeshAsset.Object;

	TVMesh->SetStaticMesh(Asset);
	TVMesh->SetRelativeLocation(FVector(2.f, 170.f, 40.f));
	TVMesh->SetRelativeRotation(FRotator(0.f, -180.f, 0.f));

	PlantMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlantMeshComponent"));
	PlantMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PlantMeshAsset(TEXT("StaticMesh'/Game/LoftOffice/Meshes/SM_Plant_3'"));
	Asset = PlantMeshAsset.Object;

	PlantMesh->SetStaticMesh(Asset);
	PlantMesh->SetRelativeLocation(FVector(-151.6f, 95.72f, 0.f));
	PlantMesh->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));

	PlantMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlantMesh2Component"));
	PlantMesh2->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	PlantMesh2->SetStaticMesh(Asset);
	PlantMesh2->SetRelativeLocation(FVector(120.314f, 112.487f, 0.f));
	PlantMesh2->SetRelativeRotation(FRotator(0.f, 165.6f, 0.f));
	PlantMesh2->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));
}

// Called when the game starts or when spawned
void ASofa2::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASofa2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ASofa2::GetBoundingPointsOnScreen(int ResX, int ResY)
{
	FString BoxPositions = "";
	float RenderTime;

	RenderTime = 0.5;

	if (WasComponentRenderedRecently(RenderTime, SofaMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Sofa", "10", SofaMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, TableMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Table", "5", TableMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, CabinMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Table", "5", CabinMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, TVMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Computer", "7", TVMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, PlantMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Plant", "4", PlantMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, PlantMesh2))
	{
		BoxPositions += GetBoundingsOnScreen("Plant", "4", PlantMesh2, GetWorld(), ResX, ResY);
	}

	return BoxPositions;
}
