// An actor for a bathroom setup

#include "Bathroom3.h"
#include "BoundingCreator.h"

// Sets default values
ABathroom3::ABathroom3()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SinkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SinkMeshComponent"));
	SinkMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/Storage/Storage24/SM_Storage24'"));
	UStaticMesh* Asset = MeshAsset.Object;

	SinkMesh->SetStaticMesh(Asset);

	TapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TapMeshComponent"));
	TapMesh->AttachToComponent(SinkMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TapMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/Bathroom/BathroomFaucet06/SM_BathroomFaucet06'"));
	Asset = TapMeshAsset.Object;

	TapMesh->SetStaticMesh(Asset);
	TapMesh->SetRelativeLocation(FVector(0.f, 5.f, 75.f));
	TapMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));

	TubMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TubMeshComponent"));
	TubMesh->AttachToComponent(SinkMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TubMeshAsset(TEXT("StaticMesh'/Game/BigTriplexHouseVilla/Meshes/Props/SM_BathUpTube'"));
	Asset = TubMeshAsset.Object;

	TubMesh->SetStaticMesh(Asset);
	TubMesh->SetRelativeLocation(FVector(0.f, 155.f, -3.f));
	TubMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}

// Called when the game starts or when spawned
void ABathroom3::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABathroom3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ABathroom3::GetBoundingPointsOnScreen(int ResX, int ResY)
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