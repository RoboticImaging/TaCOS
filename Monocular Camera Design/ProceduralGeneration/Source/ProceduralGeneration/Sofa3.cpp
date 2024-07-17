// An actor for a living room setup

#include "Sofa3.h"
#include "BoundingCreator.h"

// Sets default values
ASofa3::ASofa3()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SofaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SofaMeshComponent"));
	SofaMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/FreeFurniturePack/Meshes/SM_Classic_Leather_sofa_2'"));
	UStaticMesh* Asset = MeshAsset.Object;

	SofaMesh->SetStaticMesh(Asset);

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMeshComponent"));
	TableMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TableMeshAsset(TEXT("StaticMesh'/Game/TMChairsAndTablesPack1/ChairsAndTables/Tables/TableLow03/SM_TableLow03'"));
	Asset = TableMeshAsset.Object;

	TableMesh->SetStaticMesh(Asset);
	TableMesh->SetRelativeLocation(FVector(0.f, 150.f, 0.f));

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMeshComponent"));
	CubeMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshAsset(TEXT("StaticMesh'/Game/Sofa_Cube_2'"));
	Asset = CubeMeshAsset.Object;

	CubeMesh->SetStaticMesh(Asset);
	CubeMesh->SetRelativeLocation(FVector(0.f, 5.f, 20.f));
	CubeMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	CubeMesh->SetRelativeScale3D(FVector(1.8075f, 0.9675f, 0.42f));

	LaptopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaptopMeshComponent"));
	LaptopMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>LaptopMeshAsset(TEXT("StaticMesh'/Game/TMHighTechPack1/HighTech/meshes/Laptop03/SM_Laptop03'"));
	Asset = LaptopMeshAsset.Object;

	LaptopMesh->SetStaticMesh(Asset);
	LaptopMesh->SetRelativeLocation(FVector(0.f, 150.f, 40.f));
	LaptopMesh->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	LaptopMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));

	LampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LampMeshComponent"));
	LampMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>LampMeshAsset(TEXT("StaticMesh'/Game/BigTriplexHouseVilla/Meshes/Props/SM_LampStand'"));
	Asset = LampMeshAsset.Object;

	LampMesh->SetStaticMesh(Asset);
	LampMesh->SetRelativeLocation(FVector(-100.f, 150.f, 0.f));
	LampMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}

// Called when the game starts or when spawned
void ASofa3::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASofa3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ASofa3::GetBoundingPointsOnScreen(int ResX, int ResY)
{
	FString BoxPositions = "";
	float RenderTime;

	RenderTime = 0.5;

	if (WasComponentRenderedRecently(RenderTime, SofaMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Sofa", "0", SofaMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, TableMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Table", "5", TableMesh, GetWorld(), ResX, ResY);
	}
	
	if (WasComponentRenderedRecently(RenderTime, LaptopMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Computer", "7", LaptopMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, LampMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Lamp", "9", LampMesh, GetWorld(), ResX, ResY);
	}

	return BoxPositions;
}

