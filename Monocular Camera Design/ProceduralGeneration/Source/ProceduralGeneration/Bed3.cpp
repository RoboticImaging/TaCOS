// An actor for a bedroom setup


#include "Bed3.h"
#include "BoundingCreator.h"

// Sets default values
ABed3::ABed3()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BedMeshComponent"));
	BedMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/BigTriplexHouseVilla/Meshes/Props/SM_BedDubleR1'"));
	UStaticMesh* Asset = MeshAsset.Object;

	BedMesh->SetStaticMesh(Asset);

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMeshComponent"));
	CubeMesh->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshAsset(TEXT("StaticMesh'/Game/Bed_Cube3'"));
	Asset = CubeMeshAsset.Object;

	CubeMesh->SetStaticMesh(Asset);
	CubeMesh->SetRelativeLocation(FVector(0.f, 0.f, 20.f));
	CubeMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	CubeMesh->SetRelativeScale3D(FVector(0.4375f, 1.085f, 0.3975f));

	LampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LampMeshComponent"));
	LampMesh->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>LampMeshAsset(TEXT("StaticMesh'/Game/BigTriplexHouseVilla/Meshes/Props/SM_BedBabyLight'"));
	Asset = LampMeshAsset.Object;

	LampMesh->SetStaticMesh(Asset);
	LampMesh->SetRelativeLocation(FVector(-140.f, -50.f, 0.f));

	LampMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LampMesh2Component"));
	LampMesh2->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);
	LampMesh2->SetStaticMesh(Asset);
	LampMesh2->SetRelativeLocation(FVector(140.f, -50.f, 0.f));
}

// Called when the game starts or when spawned
void ABed3::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABed3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ABed3::GetBoundingPointsOnScreen(int ResX, int ResY)
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


