// Character actor, can move in the environment, carrys a camera sensor, change the camera's parameters
// Modified with code written by Epic Games, Inc.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/CinematicCamera/Public/CineCameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/Character.h"
#include "ProceduralGenerationCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCineCameraComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class USceneCaptureComponent2D;

// Declaration of the delegate that will be called when the Primary Action is triggered
// It is declared as dynamic so it can be accessed also in Blueprints
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);

UCLASS(config=Game)
class AProceduralGenerationCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCineCameraComponent* FirstPersonCameraComponent;

public:
	AProceduralGenerationCharacter();

protected:
	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void CountThreshold(bool IsSeen);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItem;

	UPROPERTY(EditAnywhere)
	UTexture2D* MTexture;

	UFUNCTION(Exec, BlueprintCallable, Category = Camera)
	void ChangeSensorSize(float Width, float Height);


	UFUNCTION(Exec, BlueprintCallable, Category = Camera)
	TArray<FString> GetFeatureImgPath(int index, int solution_num);

	UFUNCTION(Exec, BlueprintCallable, Category = Camera)
	TArray<FString> GetTrainImgPath(int index, int solution_num);

	UFUNCTION(Exec, BlueprintCallable, Category = Camera)
	void ChangeOrientation(float Orientation);

	UFUNCTION(Exec, BlueprintCallable, Category = Camera)
	void ChangeCameraHeight();

	UFUNCTION(Exec, BlueprintCallable, Category = Threshold)
	void ResetThresholdCount();

	UFUNCTION(Exec, BlueprintCallable, Category = Threshold)
	TArray<int> GetThresholdCount();

	UFUNCTION(Exec, BlueprintCallable, Category = Threshold)
	TArray<float> ThresholdDetectionAccuracy();

	UFUNCTION(Exec, BlueprintCallable, Category = Threshold)
	void WaitTillComplete();

	UFUNCTION()
	void ResetCamera(bool FullyReset);

	UFUNCTION()
	int GetMissedThreshold();

	UFUNCTION()
	int GetSeenThreshold();

	UFUNCTION(Exec, BlueprintCallable, Category = Camera)
	TArray<float> GetCameraSettings();

protected:
	
	/** Fires a projectile. */
	void OnPrimaryAction();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TextureFromImage_Internal(const int32 SrcWidth, const int32 SrcHeight, const TArray<FColor>& SrcData, const bool UseAlpha);


protected:
	int SeenThreshold, MissedThreshold;
	int SeenUpperThreshold, MissedUpperThreshold;
	TArray<uint8> InputImg;
	float CameraPitch, CameraFL;
	TArray<FString> SaveBoxPosition;
};

