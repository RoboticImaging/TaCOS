// An actor for a study setup


#include "Desk.h"
#include "BoundingCreator.h"

// Sets default values
ADesk::ADesk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DeskMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DeskMeshComponent"));
	DeskMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/LoftOffice/Meshes/SM_Table_2'"));
	UStaticMesh* Asset = MeshAsset.Object;

	DeskMesh->SetStaticMesh(Asset);

	SplitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SplitMeshComponent"));
	SplitMesh->AttachToComponent(DeskMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SplitMeshAsset(TEXT("StaticMesh'/Game/Desk_Split'"));
	Asset = SplitMeshAsset.Object;

	SplitMesh->SetStaticMesh(Asset);
	SplitMesh->SetRelativeLocation(FVector(0.f, -170.f, 35.f));
	SplitMesh->SetRelativeRotation(FRotator(-90.f, 90.f, 0.f));
	SplitMesh->SetRelativeScale3D(FVector(0.6f, 0.5f, 0.485f));

	ChairMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairMeshComponent"));
	ChairMesh->AttachToComponent(DeskMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>ChairMeshAsset(TEXT("StaticMesh'/Game/TMChairsAndTablesPack1/ChairsAndTables/Chairs/Chair27/SM_Chair27'"));
	Asset = ChairMeshAsset.Object;

	ChairMesh->SetStaticMesh(Asset);
	ChairMesh->SetRelativeLocation(FVector(90.f, -80.f, 0.f));
	ChairMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	ChairMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairMesh2Component"));
	ChairMesh2->AttachToComponent(DeskMesh, FAttachmentTransformRules::KeepRelativeTransform);

	ChairMesh2->SetStaticMesh(Asset);
	ChairMesh2->SetRelativeLocation(FVector(-80.f, -80.f, 0.f));
	ChairMesh2->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	TabletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TabletMeshComponent"));
	TabletMesh->AttachToComponent(DeskMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TabletMeshAsset(TEXT("StaticMesh'/Game/TMHighTechPack1/HighTech/meshes/GraphicTablet02/SM_GraphicTablet02'"));
	Asset = TabletMeshAsset.Object;

	TabletMesh->SetStaticMesh(Asset);
	TabletMesh->SetRelativeLocation(FVector(-50.f, -80.f, 74.f));
	TabletMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	LaptopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaptopMeshComponent"));
	LaptopMesh->AttachToComponent(DeskMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>LaptopMeshAsset(TEXT("StaticMesh'/Game/TMHighTechPack1/HighTech/meshes/Laptop01/SM_Laptop01'"));
	Asset = LaptopMeshAsset.Object;

	LaptopMesh->SetStaticMesh(Asset);
	LaptopMesh->SetRelativeLocation(FVector(60.f, -80.f, 74.f));
	LaptopMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	LaptopMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));

	PlantMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlantMeshComponent"));
	PlantMesh->AttachToComponent(DeskMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PlantMeshAsset(TEXT("StaticMesh'/Game/LoftOffice/Meshes/SM_Plant_4'"));
	Asset = PlantMeshAsset.Object;

	PlantMesh->SetStaticMesh(Asset);
	PlantMesh->SetRelativeLocation(FVector(-70.f, 30.f, 0.f));

	PlantMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlantMesh2Component"));
	PlantMesh2->AttachToComponent(DeskMesh, FAttachmentTransformRules::KeepRelativeTransform);

	PlantMesh2->SetStaticMesh(Asset);
	PlantMesh2->SetRelativeLocation(FVector(90.f, 30.f, 0.f));
}

// Called when the game starts or when spawned
void ADesk::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADesk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FString ADesk::GetBoundingPointsOnScreen(int ResX, int ResY)
{
	FString BoxPositions = "";
	float RenderTime;

	RenderTime = 0.5;

	if (WasComponentRenderedRecently(RenderTime, DeskMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Table", "5", DeskMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, ChairMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Chair", "6", ChairMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, ChairMesh2))
	{
		BoxPositions += GetBoundingsOnScreen("Chair", "6", ChairMesh2, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, TabletMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Computer", "7", TabletMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, LaptopMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Computer", "7", LaptopMesh, GetWorld(), ResX, ResY);
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
