// Window actor

#include "Window.h"

// Sets default values
AWindow::AWindow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMeshComponent"));
	FrameMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Window'"));
	UStaticMesh* Asset = MeshAsset.Object;

	FrameMesh->SetStaticMesh(Asset);

	WindowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlassMeshComponent"));
	WindowMesh->AttachToComponent(FrameMesh, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>WindowMeshAsset(TEXT("StaticMesh'/Game/Glass'"));
	Asset = WindowMeshAsset.Object;

	WindowMesh->SetStaticMesh(Asset);
	WindowMesh->SetRelativeScale3D(FVector(0.055f, 0.94f, 2.94f));
	WindowMesh->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
}

// Called when the game starts or when spawned
void AWindow::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWindow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

