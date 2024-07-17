// An actor for a living room setup

#include "Sofa.h"
#include "BoundingCreator.h"

// Sets default values
ASofa::ASofa()
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

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TableMeshAsset(TEXT("StaticMesh'/Game/LoftOffice/Meshes/SM_Coffee_table_1'"));
	Asset = TableMeshAsset.Object;

	TableMesh->SetStaticMesh(Asset);
	TableMesh->SetRelativeLocation(FVector(0.f, 110.f, 0.f));

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMeshComponent"));
	CubeMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshAsset(TEXT("StaticMesh'/Game/Sofa_Cube'"));
	Asset = CubeMeshAsset.Object;

	CubeMesh->SetStaticMesh(Asset);
	CubeMesh->SetRelativeLocation(FVector(0.f, 0.f, 20.f));
	CubeMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	CubeMesh->SetRelativeScale3D(FVector(2.135f, 1.f, 0.4175f));

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMeshComponent"));
	BallMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BallMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/ToysGames/Toy25/SM_Toy25'"));
	Asset = BallMeshAsset.Object;

	BallMesh->SetStaticMesh(Asset);
	BallMesh->SetRelativeLocation(FVector(-118.5f, 115.63f, 0.f));
	BallMesh->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));

	RabbitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RabbitMeshComponent"));
	RabbitMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>RabbitMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/ToysGames/Toy21/SM_Toy21'"));
	Asset = RabbitMeshAsset.Object;

	RabbitMesh->SetStaticMesh(Asset);
	RabbitMesh->SetRelativeLocation(FVector(110.f, 110.f, 0.f));
	RabbitMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));

	UniconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UniconMeshComponent"));
	UniconMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>UniconMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/ToysGames/Toy19/SM_Toy19'"));
	Asset = UniconMeshAsset.Object;

	UniconMesh->SetStaticMesh(Asset);
	UniconMesh->SetRelativeLocation(FVector(0.f, 110.f, 40.f));
	UniconMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
}

// Called when the game starts or when spawned
void ASofa::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASofa::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ASofa::GetBoundingPointsOnScreen(int ResX, int ResY)
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

	if (WasComponentRenderedRecently(RenderTime, RabbitMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Toy", "2", RabbitMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, UniconMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Toy", "2", UniconMesh, GetWorld(), ResX, ResY);
	}

	return BoxPositions;
}