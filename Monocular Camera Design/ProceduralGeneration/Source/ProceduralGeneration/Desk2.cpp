// An actor for a study setup

#include "Desk2.h"
#include "BoundingCreator.h"

// Sets default values
ADesk2::ADesk2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DeskMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DeskMeshComponent"));
	DeskMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/TMChairsAndTablesPack1/ChairsAndTables/Tables/TableHigh13/SM_TableHigh13'"));
	UStaticMesh* Asset = MeshAsset.Object;

	DeskMesh->SetStaticMesh(Asset);

	SplitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SplitMeshComponent"));
	SplitMesh->AttachToComponent(DeskMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SplitMeshAsset(TEXT("StaticMesh'/Game/Desk_Split2'"));
	Asset = SplitMeshAsset.Object;

	SplitMesh->SetStaticMesh(Asset);
	SplitMesh->SetRelativeLocation(FVector(0.f, 0.f, 23.f));
	SplitMesh->SetRelativeScale3D(FVector(2.0475f, 0.06f, 0.3925f));

	ChairMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairMeshComponent"));
	ChairMesh->AttachToComponent(DeskMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>ChairMeshAsset(TEXT("StaticMesh'/Game/LoftOffice/Meshes/SM_Chair_2'"));
	Asset = ChairMeshAsset.Object;

	ChairMesh->SetStaticMesh(Asset);
	ChairMesh->SetRelativeLocation(FVector(0.f, 50.f, 0.f));
	ChairMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	ScreenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScreenMeshComponent"));
	ScreenMesh->AttachToComponent(DeskMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>ScreenMeshAsset(TEXT("StaticMesh'/Game/TMHighTechPack1/HighTech/meshes/Screen04/SM_Screen04'"));
	Asset = ScreenMeshAsset.Object;

	ScreenMesh->SetStaticMesh(Asset);
	ScreenMesh->SetRelativeLocation(FVector(0.f, -10.f, 90.f));

	DesktopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DesktopMeshComponent"));
	DesktopMesh->AttachToComponent(DeskMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>DesktopMeshAsset(TEXT("StaticMesh'/Game/TMHighTechPack1/HighTech/meshes/Computer02/SM_Computer02'"));
	Asset = DesktopMeshAsset.Object;

	DesktopMesh->SetStaticMesh(Asset);
	DesktopMesh->SetRelativeLocation(FVector(-60.f, 10.f, 90.f));

	KeyboardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyboardMeshComponent"));
	KeyboardMesh->AttachToComponent(DeskMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>KeyboardMeshAsset(TEXT("StaticMesh'/Game/TMHighTechPack1/HighTech/meshes/PCKeyboard01/SM_PCKeyboard01'"));
	Asset = KeyboardMeshAsset.Object;

	KeyboardMesh->SetStaticMesh(Asset);
	KeyboardMesh->SetRelativeLocation(FVector(0.f, 30.f, 90.f));

	MouseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MouseMeshComponent"));
	MouseMesh->AttachToComponent(DeskMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MouseMeshAsset(TEXT("StaticMesh'/Game/TMHighTechPack1/HighTech/meshes/PCMouse03/SM_PCMouse03'"));
	Asset = MouseMeshAsset.Object;

	MouseMesh->SetStaticMesh(Asset);
	MouseMesh->SetRelativeLocation(FVector(40.f, 30.f, 90.f));

	ClockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockMeshComponent"));
	ClockMesh->AttachToComponent(DeskMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>ClockMeshAsset(TEXT("StaticMesh'/Game/TMStoragesPack1/Storages/meshes/Decorations/FlipCalendar01/SM_FlipCalendar01'"));
	Asset = ClockMeshAsset.Object;

	ClockMesh->SetStaticMesh(Asset);
	ClockMesh->SetRelativeLocation(FVector(70.f, 0.f, 90.f));
	ClockMesh->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
}

// Called when the game starts or when spawned
void ADesk2::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADesk2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString ADesk2::GetBoundingPointsOnScreen(int ResX, int ResY)
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

	if (WasComponentRenderedRecently(RenderTime, ScreenMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Computer", "7", ScreenMesh, GetWorld(), ResX, ResY);
	}

	if (WasComponentRenderedRecently(RenderTime, DesktopMesh))
	{
		BoxPositions += GetBoundingsOnScreen("Computer", "7", DesktopMesh, GetWorld(), ResX, ResY);
	}

	return BoxPositions;
}

