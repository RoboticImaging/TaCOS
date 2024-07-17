// An actor for a living room setup

#include "Sofa6.h"
#include "BoundingCreator.h"

// Sets default values
ASofa6::ASofa6()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SofaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SofaMeshComponent"));
	SofaMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/BigTriplexHouseVilla/Meshes/Props/SM_Sofa2'"));
	UStaticMesh* Asset = MeshAsset.Object;

	SofaMesh->SetStaticMesh(Asset);

	ChairMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairMeshComponent"));
	ChairMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>ChairMeshAsset(TEXT("StaticMesh'/Game/TMChairsAndTablesPack1/ChairsAndTables/Chairs/Chair17/SM_Chair17'"));
	Asset = ChairMeshAsset.Object;

	ChairMesh->SetStaticMesh(Asset);
	ChairMesh->SetRelativeLocation(FVector(90.f, 150.f, 0.f));
	ChairMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMeshComponent"));
	TableMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TableMeshAsset(TEXT("StaticMesh'/Game/TMChairsAndTablesPack1/ChairsAndTables/Tables/TableLow03/SM_TableLow03'"));
	Asset = TableMeshAsset.Object;

	TableMesh->SetStaticMesh(Asset);
	TableMesh->SetRelativeLocation(FVector(-20.f, 130.f, 0.f));
	TableMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMeshComponent"));
	BallMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BallMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/ToysGames/Toy25/SM_Toy25'"));
	Asset = BallMeshAsset.Object;

	BallMesh->SetStaticMesh(Asset);
	BallMesh->SetRelativeLocation(FVector(-20.f, 130.f, 40.f));
	BallMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMeshComponent"));
	CubeMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshAsset(TEXT("StaticMesh'/Game/Sofa_Cube_6'"));
	Asset = CubeMeshAsset.Object;

	CubeMesh->SetStaticMesh(Asset);
	CubeMesh->SetRelativeLocation(FVector(0.f, 0.f, 20.f));
	CubeMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	CubeMesh->SetRelativeScale3D(FVector(1.8375f, 0.8125f, 0.3f));
}

// Called when the game starts or when spawned
void ASofa6::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASofa6::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ASofa6::GetBoundingPointsOnScreen(int ResX, int ResY)
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

	if (WasComponentRenderedRecently(RenderTime, BallMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Toy", "2", BallMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, ChairMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Chair", "6", ChairMesh, GetWorld(), ResX, ResY);
	}

	return BoxPositions;
}