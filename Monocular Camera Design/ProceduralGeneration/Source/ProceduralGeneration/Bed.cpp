// An actor for a bedroom setup

#include "Bed.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BoundingCreator.h"

// Sets default values
ABed::ABed()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BedMeshComponent"));
	BedMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/FreeFurniturePack/Meshes/SM_Modern_Bed_2'"));
	UStaticMesh* Asset = MeshAsset.Object;

	BedMesh->SetStaticMesh(Asset);

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMeshComponent"));
	CubeMesh->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshAsset(TEXT("StaticMesh'/Game/Bed_Cube'"));
	Asset = CubeMeshAsset.Object;

	CubeMesh->SetStaticMesh(Asset);
	CubeMesh->SetRelativeLocation(FVector(-6.f, -5.f, 20.f));
	CubeMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	CubeMesh->SetRelativeScale3D(FVector(0.4925f, 2.04f, 0.53f));

	DrawerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DrawerMeshComponent"));
	DrawerMesh->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>DrawerMeshAsset(TEXT("StaticMesh'/Game/FreeFurniturePack/Meshes/SM_Bed_Drawer'"));
	Asset = DrawerMeshAsset.Object;

	DrawerMesh->SetStaticMesh(Asset);
	DrawerMesh->SetRelativeLocation(FVector(-100.f, 130.f, 0.f));
	DrawerMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	DrawerMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drawer2MeshComponent"));
	DrawerMesh2->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);

	DrawerMesh2->SetStaticMesh(Asset);
	DrawerMesh2->SetRelativeLocation(FVector(-100.f, -140.f, 0.f));
	DrawerMesh2->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	OpenerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpenerMeshComponent"));
	OpenerMesh->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>OpenerMeshAsset(TEXT("StaticMesh'/Game/FreeFurniturePack/Meshes/SM_Bed_Drawer_Openable_Part'"));
	Asset = OpenerMeshAsset.Object;

	OpenerMesh->SetStaticMesh(Asset);
	OpenerMesh->SetRelativeLocation(FVector(-88.f, 130.f, 6.f));
	OpenerMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	OpenerMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpenerMesh2Component"));
	OpenerMesh2->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);

	OpenerMesh2->SetStaticMesh(Asset);
	OpenerMesh2->SetRelativeLocation(FVector(-88.f, 130.f, 28.f));
	OpenerMesh2->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	OpenerMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpenerMesh3Component"));
	OpenerMesh3->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);

	OpenerMesh3->SetStaticMesh(Asset);
	OpenerMesh3->SetRelativeLocation(FVector(-88.f, -140.f, 6.f));
	OpenerMesh3->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	OpenerMesh4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpenerMesh4Component"));
	OpenerMesh4->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);

	OpenerMesh4->SetStaticMesh(Asset);
	OpenerMesh4->SetRelativeLocation(FVector(-88.f, -140.f, 28.f));
	OpenerMesh4->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	LampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LampMeshComponent"));
	LampMesh->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>LampMeshAsset(TEXT("StaticMesh'/Game/FreeFurniturePack/Meshes/SM_Bed_lamp_3'"));
	Asset = LampMeshAsset.Object;

	LampMesh->SetStaticMesh(Asset);
	LampMesh->SetRelativeLocation(FVector(-100.f, -140.f, 56.f));

	LampMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LampMesh2Component"));
	LampMesh2->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);
	LampMesh2->SetStaticMesh(Asset);
	LampMesh2->SetRelativeLocation(FVector(-100.f, 130.f, 56.f));

	RabbitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RabbitMeshComponent"));
	RabbitMesh->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>RabbitMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/ToysGames/Toy21/SM_Toy21'"));
	Asset = RabbitMeshAsset.Object;

	RabbitMesh->SetStaticMesh(Asset);
	RabbitMesh->SetRelativeLocation(FVector(30.f, 50.f, 60.f));
	RabbitMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	RabbitMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	UniconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UniconMeshComponent"));
	UniconMesh->AttachToComponent(BedMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>UniconMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/ToysGames/Toy19/SM_Toy19'"));
	Asset = UniconMeshAsset.Object;

	UniconMesh->SetStaticMesh(Asset);
	UniconMesh->SetRelativeLocation(FVector(40.f, -40.f, 60.f));
	UniconMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
	UniconMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}

// Called when the game starts or when spawned
void ABed::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ABed::GetBoundingPointsOnScreen(int ResX, int ResY)
{
	FString BoxPositions = "";
	float RenderTime;

	RenderTime = 0.5;

	if (WasComponentRenderedRecently(RenderTime, BedMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Bed", "1", BedMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, RabbitMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Toy", "2", RabbitMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, UniconMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Toy", "2", UniconMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, LampMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Lamp", "8", LampMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, LampMesh2))
	{
		BoxPositions += GetBoundingsOnScreen("Lamp", "8", LampMesh2, GetWorld(), ResX, ResY);
	}

	return BoxPositions;
}
