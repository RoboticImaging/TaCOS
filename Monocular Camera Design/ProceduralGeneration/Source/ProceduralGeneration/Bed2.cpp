// An actor for a bedroom setup


#include "Bed2.h"
#include "BoundingCreator.h"

// Sets default values
ABed2::ABed2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BedMeshComponent"));
	BedMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/BigTriplexHouseVilla/Meshes/Props/SM_BedAttic'"));
	UStaticMesh* Asset = MeshAsset.Object;

	BedMesh->SetStaticMesh(Asset);

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMeshComponent"));
	CubeMesh->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshAsset(TEXT("StaticMesh'/Game/Bed_Cube2'"));
	Asset = CubeMeshAsset.Object;

	CubeMesh->SetStaticMesh(Asset);
	CubeMesh->SetRelativeLocation(FVector(0.f, 0.f, 17.f));
	CubeMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	CubeMesh->SetRelativeScale3D(FVector(1.8f, 2.f, 0.2f));

	LampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LampMeshComponent"));
	LampMesh->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>LampMeshAsset(TEXT("StaticMesh'/Game/LoftOffice/Meshes/SM_Floor_Lamp'"));
	Asset = LampMeshAsset.Object;

	LampMesh->SetStaticMesh(Asset);
	LampMesh->SetRelativeLocation(FVector(-150.f, -80.f, 0.f));

	LampMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LampMesh2Component"));
	LampMesh2->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);
	LampMesh2->SetStaticMesh(Asset);
	LampMesh2->SetRelativeLocation(FVector(150.f, -80.f, 0.f));
}

// Called when the game starts or when spawned
void ABed2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABed2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ABed2::GetBoundingPointsOnScreen(int ResX, int ResY)
{
	FString BoxPositions = "";
	float RenderTime;

	RenderTime = 0.5;

	if (WasComponentRenderedRecently(RenderTime, BedMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Bed", "1", BedMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, LampMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Lamp", "9", LampMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, LampMesh2))
	{
		BoxPositions += GetBoundingsOnScreen("Lamp", "8", LampMesh2, GetWorld(), ResX, ResY);
	}

	return BoxPositions;
}

