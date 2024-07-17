// Character actor, can move in the environment, carrys a camera sensor, change the camera's parameters
// Modified with code written by Epic Games, Inc.

#include "ProceduralGenerationCharacter.h"
#include "ProceduralGenerationProjectile.h"
#include "ProceduralRoom.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PreOpenCVHeaders.h"
#include "OpenCVHelper.h"
#include "UnrealClient.h"
#include "ImageUtils.h"
#include <ThirdParty/OpenCV/include/opencv2/imgproc.hpp>
#include <ThirdParty/OpenCV/include/opencv2/highgui/highgui.hpp>
#include <ThirdParty/OpenCV/include/opencv2/core.hpp>
#include "PostOpenCVHeaders.h"
#include <vector>
#include <math.h>
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Engine/GameViewportClient.h"
//#include "RealtimeStyleTransferViewExtension.h"


//////////////////////////////////////////////////////////////////////////
// AProceduralGenerationCharacter
USceneCaptureComponent2D* sceneCapture;
UTextureRenderTarget2D* renderTarget;

AProceduralGenerationCharacter::AProceduralGenerationCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;

	CameraPitch = 0.f;
	CameraFL = 10.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCineCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->SetRelativeRotation(FRotator(CameraPitch, 0.f, 0.f));
	FirstPersonCameraComponent->SetCurrentFocalLength(CameraFL);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
}

void AProceduralGenerationCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AProceduralGenerationCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AProceduralGenerationCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AProceduralGenerationCharacter::CountThreshold(bool IsSeen)
{
	FVector Vel = GetVelocity();
	FVector Forward = GetActorForwardVector();
	float ForwardSpeed = FVector::DotProduct(Vel, Forward);

	if (ForwardSpeed > 0)
	{
		if (!IsSeen)
		{
			MissedThreshold++;
		}
		else
		{
			SeenThreshold++;
		}

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Lower Threshold Seen: %d, Lower Threshold Missed: %d"), SeenThreshold, MissedThreshold));

	}
}

void AProceduralGenerationCharacter::ChangeSensorSize(float Width, float Height)
{
	FirstPersonCameraComponent->Filmback.SensorHeight = Height;
	FirstPersonCameraComponent->Filmback.SensorWidth = Width;
}

TArray<FString> AProceduralGenerationCharacter::GetFeatureImgPath(int index, int solution_num)
{
	TArray<FString> ImgDirectory;
	FString ScreenshotFilePath, ScreenshotFileName;

	ScreenshotFilePath = "E:\\train_data/";
	ScreenshotFilePath += FString::FromInt(solution_num);
	
	ImgDirectory.Add(ScreenshotFilePath);

	ScreenshotFileName = FString::FromInt(index);
	ScreenshotFileName += ".tif";

	ImgDirectory.Add(ScreenshotFileName);

	return ImgDirectory;
}

TArray<FString> AProceduralGenerationCharacter::GetTrainImgPath(int index, int solution_num)
{	
	TArray<FString> ImgDirectory;
	FString ScreenshotFilePath, ScreenshotFileName;

	ScreenshotFilePath = "E:\\train_data/";
	ScreenshotFilePath += FString::FromInt(solution_num);
	ScreenshotFilePath += "_train";

	ImgDirectory.Add(ScreenshotFilePath);

	ScreenshotFileName = "train";
	ScreenshotFileName += FString::FromInt(index);
	ScreenshotFileName += ".tif";

	ImgDirectory.Add(ScreenshotFileName);

	return ImgDirectory;
}

void AProceduralGenerationCharacter::ChangeOrientation(float Orientation)
{
	FirstPersonCameraComponent->SetRelativeRotation(FRotator(Orientation, 0.f, 0.f));
}

void AProceduralGenerationCharacter::ChangeCameraHeight()
{	
	float Height = FMath::FRandRange(70.f, 90.f);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, Height));
}

void AProceduralGenerationCharacter::ResetThresholdCount()
{
	MissedThreshold = 0;
	SeenThreshold = 0;
	MissedUpperThreshold = 0;
	SeenUpperThreshold = 0;
}

TArray<int> AProceduralGenerationCharacter::GetThresholdCount()
{
	TArray<int> ThresholdList;

	ThresholdList.Add(SeenThreshold);
	ThresholdList.Add(MissedThreshold);

	return ThresholdList;
}

TArray<float> AProceduralGenerationCharacter::ThresholdDetectionAccuracy()
{
	float UpperThresholdAccuracy;
	float LowerThresholdAccuracy;
	TArray<float> AccuracyList;

	UpperThresholdAccuracy = roundf(((SeenUpperThreshold / (MissedUpperThreshold + SeenUpperThreshold)) * 100) / 100);
	LowerThresholdAccuracy = roundf(((SeenThreshold / (MissedThreshold + SeenThreshold)) * 100) / 100);

	AccuracyList.Add(UpperThresholdAccuracy);
	AccuracyList.Add(LowerThresholdAccuracy);

	return AccuracyList;
}

void AProceduralGenerationCharacter::WaitTillComplete()
{

	ResetThresholdCount();

	while (MissedThreshold + SeenThreshold < 10){}
}

void AProceduralGenerationCharacter::ResetCamera(bool FullyReset)
{
	MissedThreshold = 0;
	SeenThreshold = 0;

	if (!FullyReset)
	{
		CameraFL += 2.5;
	}
	else if (FullyReset)
	{
		CameraFL = 5.f;
	}

	FirstPersonCameraComponent->SetCurrentFocalLength(CameraFL);
}

int AProceduralGenerationCharacter::GetMissedThreshold()
{
	return MissedThreshold;
}

int AProceduralGenerationCharacter::GetSeenThreshold()
{
	return SeenThreshold;
}

TArray<float> AProceduralGenerationCharacter::GetCameraSettings()
{
	TArray<float> CameraSettings;
	CameraSettings.Add(CameraPitch);
	CameraSettings.Add(CameraFL);

	return CameraSettings;
}

//////////////////////////////////////////////////////////////////////////// Input

void AProceduralGenerationCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AProceduralGenerationCharacter::OnPrimaryAction);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AProceduralGenerationCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AProceduralGenerationCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AProceduralGenerationCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AProceduralGenerationCharacter::LookUpAtRate);
}

void AProceduralGenerationCharacter::OnPrimaryAction()
{
	// Trigger the OnItemUsed Event
	OnUseItem.Broadcast();
}

void AProceduralGenerationCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnPrimaryAction();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AProceduralGenerationCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void AProceduralGenerationCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AProceduralGenerationCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AProceduralGenerationCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AProceduralGenerationCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

bool AProceduralGenerationCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AProceduralGenerationCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AProceduralGenerationCharacter::EndTouch);

		return true;
	}
	
	return false;
}
