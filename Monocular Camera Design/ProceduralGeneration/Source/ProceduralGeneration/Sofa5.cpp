// An actor for a living room setup

#include "Sofa5.h"
#include "BoundingCreator.h"

// Sets default values
ASofa5::ASofa5()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SofaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SofaMeshComponent"));
	SofaMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/LoftOffice/Meshes/SM_Armchair_2'"));
	UStaticMesh* Asset = MeshAsset.Object;

	SofaMesh->SetStaticMesh(Asset);
	SofaMesh->SetRelativeScale3D(FVector(2.5f, 1.f, 1.f));

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMeshComponent"));
	TableMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TableMeshAsset(TEXT("StaticMesh'/Game/TMChairsAndTablesPack1/ChairsAndTables/Tables/TableLow11/SM_TableLow11'"));
	Asset = TableMeshAsset.Object;

	TableMesh->SetStaticMesh(Asset);
	TableMesh->SetRelativeLocation(FVector(0.f, 110.f, 0.f));
	TableMesh->SetRelativeScale3D(FVector(.5f, .5f, 1.f));

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMeshComponent"));
	CubeMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshAsset(TEXT("StaticMesh'/Game/Sofa_Cube_4'"));
	Asset = CubeMeshAsset.Object;

	CubeMesh->SetStaticMesh(Asset);
	CubeMesh->SetRelativeLocation(FVector(0.f, 0.f, 19.f));
	CubeMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	CubeMesh->SetRelativeScale3D(FVector(.85f, .85f, .35f));

	LaptopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaptopMeshComponent"));
	LaptopMesh->AttachToComponent(SofaMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>LaptopMeshAsset(TEXT("StaticMesh'/Game/TMHighTechPack1/HighTech/meshes/Tablet02/SM_Tablet02'"));
	Asset = LaptopMeshAsset.Object;

	LaptopMesh->SetStaticMesh(Asset);
	LaptopMesh->SetRelativeLocation(FVector(0.f, 110.f, 40.f));
	LaptopMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	LaptopMesh->SetRelativeScale3D(FVector(.4f, .9f, 1.f));
}

// Called when the game starts or when spawned
void ASofa5::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASofa5::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ASofa5::GetBoundingPointsOnScreen(int ResX, int ResY)
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

	return BoxPositions;
}
