// An actor for a bathroom setup

#include "Bathroom.h"
#include "BoundingCreator.h"


ABathroom::ABathroom()
{
	PrimaryActorTick.bCanEverTick = true;

	SinkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SinkMeshComponent"));
	SinkMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/Storage/Storage02/SM_Storage02'"));
	UStaticMesh* Asset = MeshAsset.Object;

	SinkMesh->SetStaticMesh(Asset);

	TapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TapMeshComponent"));
	TapMesh->AttachToComponent(SinkMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TapMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/Bathroom/BathroomFaucet02/SM_BathroomFaucet02'"));
	Asset = TapMeshAsset.Object;

	TapMesh->SetStaticMesh(Asset);
	TapMesh->SetRelativeLocation(FVector(0.f, 4.f, 88.f));

	TubMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TubMeshComponent"));
	TubMesh->AttachToComponent(SinkMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TubMeshAsset(TEXT("StaticMesh'/Game/FreeFurniturePack/Meshes/SM_Bathtub'"));
	Asset = TubMeshAsset.Object;

	TubMesh->SetStaticMesh(Asset);
	TubMesh->SetRelativeLocation(FVector(120.f, 70.f, 0.f));
	TubMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	PlantMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlantMeshComponent"));
	PlantMesh->AttachToComponent(SinkMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PlantMeshAsset(TEXT("StaticMesh'/Game/LoftOffice/Meshes/SM_Plant_4'"));
	Asset = PlantMeshAsset.Object;

	PlantMesh->SetStaticMesh(Asset);
	PlantMesh->SetRelativeLocation(FVector(120.f, 180.f, 0.f));

	BookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BookMeshComponent"));
	BookMesh->AttachToComponent(SinkMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BookMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/Books/BookGroup07/SM_BookGroup07'"));
	Asset = BookMeshAsset.Object;

	BookMesh->SetStaticMesh(Asset);
	BookMesh->SetRelativeLocation(FVector(120.f, 70.f, 10.f));
	BookMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	BookMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));

	BookMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BookMesh2Component"));
	BookMesh2->AttachToComponent(SinkMesh, FAttachmentTransformRules::KeepRelativeTransform);
	BookMesh2->SetStaticMesh(Asset);
	BookMesh2->SetRelativeLocation(FVector(120.f, 10.f, 10.f));
	BookMesh2->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	BookMesh2->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
}

// Called when the game starts or when spawned
void ABathroom::BeginPlay()
{
	Super::BeginPlay();
}

FString ABathroom::GetBoundingPointsOnScreen(int ResX, int ResY)
{
	FString BoxPositions = "";
	float RenderTime;

	RenderTime = 0.5;

	if (WasComponentRenderedRecently(RenderTime, TubMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Bathtub", "3", TubMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, PlantMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Plant", "4", PlantMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, SinkMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Basin", "9", SinkMesh, GetWorld(), ResX, ResY);
	}

	return BoxPositions;
}

// Called every frame
void ABathroom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

