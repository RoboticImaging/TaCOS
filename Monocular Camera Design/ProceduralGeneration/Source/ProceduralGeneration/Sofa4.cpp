// An actor for a living room setup

#include "Sofa4.h"
#include "BoundingCreator.h"

// Sets default values
ASofa4::ASofa4()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SofaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SofaMeshComponent"));
	SofaMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/BigTriplexHouseVilla/Meshes/Props/SM_Sofa3'"));
	UStaticMesh* Asset = MeshAsset.Object;

	SofaMesh->SetStaticMesh(Asset);
	SofaMesh->SetRelativeScale3D(FVector(.65f, .75f, .75f));

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMeshComponent"));
	TableMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TableMeshAsset(TEXT("StaticMesh'/Game/LoftOffice/Meshes/SM_Coffee_table_1'"));
	Asset = TableMeshAsset.Object;

	TableMesh->SetStaticMesh(Asset);
	TableMesh->SetRelativeLocation(FVector(45.f, 80.f, 0.f));
	TableMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.2f));

	UniconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UniconMeshComponent"));
	UniconMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>UniconMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/ToysGames/Toy19/SM_Toy19'"));
	Asset = UniconMeshAsset.Object;

	UniconMesh->SetStaticMesh(Asset);
	UniconMesh->SetRelativeLocation(FVector(60.f, 85.f, 50.f));
	UniconMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
}

// Called when the game starts or when spawned
void ASofa4::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASofa4::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ASofa4::GetBoundingPointsOnScreen(int ResX, int ResY)
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

	if (WasComponentRenderedRecently(RenderTime, UniconMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Toy", "2", UniconMesh, GetWorld(), ResX, ResY);
	}

	return BoxPositions;
}
