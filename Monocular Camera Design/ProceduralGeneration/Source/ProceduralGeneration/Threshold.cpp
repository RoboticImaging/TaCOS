// A threshold actor acts as a obstacle, interacts with the character upon collision

#include "Threshold.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AThreshold::AThreshold()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionBox->SetBoxExtent(FVector(50.f, 50.f, 100.f));
	CollisionBox->SetCollisionProfileName("Trigger");
	RootComponent = CollisionBox;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMeshComponent"));
	BoxMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Threshold'"));
	UStaticMesh* Asset = MeshAsset.Object;

	BoxMesh->SetStaticMesh(Asset);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AThreshold::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AThreshold::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AThreshold::BeginPlay()
{
	Super::BeginPlay();
	
	ProceduralGenerationCharacter = Cast<AProceduralGenerationCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AProceduralGenerationCharacter::StaticClass()));
}

// Called every frame
void AThreshold::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AThreshold::WasComponentRenderedRecently(float tolerance)
{
	UWorld* World = BoxMesh->GetWorld();
	return (World) ? (World->GetTimeSeconds() - BoxMesh->GetLastRenderTimeOnScreen() <= tolerance) : false;
}

void AThreshold::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ProceduralGenerationCharacter && (ProceduralGenerationCharacter == OtherActor))
	{
		if (!WasComponentRenderedRecently(0.5))
		{
			ProceduralGenerationCharacter->CountThreshold(false);
		}
		else
		{
			ProceduralGenerationCharacter->CountThreshold(true);
		}
	}
}

void AThreshold::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

