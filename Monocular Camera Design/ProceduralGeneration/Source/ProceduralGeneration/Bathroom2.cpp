// An actor for a bathroom setup

#include "Bathroom2.h"
#include "BoundingCreator.h"

// Sets default values
ABathroom2::ABathroom2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SinkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SinkMeshComponent"));
	SinkMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/Storage/Storage06/SM_Storage06'"));
	UStaticMesh* Asset = MeshAsset.Object;

	SinkMesh->SetStaticMesh(Asset);

	TapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TapMeshComponent"));
	TapMesh->AttachToComponent(SinkMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TapMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/Bathroom/BathroomFaucet11/SM_BathroomFaucet11'"));
	Asset = TapMeshAsset.Object;

	TapMesh->SetStaticMesh(Asset);
	TapMesh->SetRelativeLocation(FVector(18.f, 16.f, 75.f));
	TapMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	TapMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TapMesh2Component"));
	TapMesh2->AttachToComponent(SinkMesh, FAttachmentTransformRules::KeepRelativeTransform);
	TapMesh2->SetStaticMesh(Asset);
	TapMesh2->SetRelativeLocation(FVector(-18.f, 16.f, 75.f));
	TapMesh2->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	TubMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TubMeshComponent"));
	TubMesh->AttachToComponent(SinkMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TubMeshAsset(TEXT("StaticMesh'/Game/FreeFurniturePack/Meshes/SM_Bathtub'"));
	Asset = TubMeshAsset.Object;

	TubMesh->SetStaticMesh(Asset);
	TubMesh->SetRelativeLocation(FVector(10.f, 110.f, 0.f));
	TubMesh->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	BookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BookMeshComponent"));
	BookMesh->AttachToComponent(SinkMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BookMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/Books/BookGroup04/SM_BookGroup04'"));
	Asset = BookMeshAsset.Object;

	BookMesh->SetStaticMesh(Asset);
	BookMesh->SetRelativeLocation(FVector(-50.f, 110.f, 10.f));
	BookMesh->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));

	TowelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowelMeshComponent"));
	TowelMesh->AttachToComponent(SinkMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TowelMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/Bathroom/Towel04/SM_Towel04Medium'"));
	Asset = TowelMeshAsset.Object;

	TowelMesh->SetStaticMesh(Asset);
	TowelMesh->SetRelativeLocation(FVector(120.f, 110.f, 1.f));
	TowelMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	HolderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HolderMeshComponent"));
	HolderMesh->AttachToComponent(SinkMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>HolderMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/Storage/TowelHolder07/SM_TowelHolder07'"));
	Asset = HolderMeshAsset.Object;

	HolderMesh->SetStaticMesh(Asset);
	HolderMesh->SetRelativeLocation(FVector(120.f, 110.f, 0.f));
	HolderMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	PineappleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PineappleMeshComponent"));
	PineappleMesh->AttachToComponent(SinkMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PineappleMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/Decorations/PineappleSculpture01/SM_PineappleSculpture01'"));
	Asset = PineappleMeshAsset.Object;

	PineappleMesh->SetStaticMesh(Asset);
	PineappleMesh->SetRelativeLocation(FVector(-110.f, 110.f, 0.f));
	PineappleMesh->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));

	PineappleTopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PineappleTopMeshComponent"));
	PineappleTopMesh->AttachToComponent(SinkMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PineappleTopMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/Decorations/PineappleSculpture01/SM_PineappleSculpture01Top'"));
	Asset = PineappleTopMeshAsset.Object;

	PineappleTopMesh->SetStaticMesh(Asset);
	PineappleTopMesh->SetRelativeLocation(FVector(-110.f, 110.f, 39.f));
	PineappleTopMesh->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));
}

// Called when the game starts or when spawned
void ABathroom2::BeginPlay()
{
	Super::BeginPlay();
}

FString ABathroom2::GetBoundingPointsOnScreen(int ResX, int ResY)
{
	FString BoxPositions = "";
	float RenderTime;

	RenderTime = 0.5;

	if (WasComponentRenderedRecently(RenderTime, TubMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Bathtub", "3", TubMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, SinkMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Basin", "9", SinkMesh, GetWorld(), ResX, ResY);
	}

	return BoxPositions;
}

// Called every frame
void ABathroom2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

