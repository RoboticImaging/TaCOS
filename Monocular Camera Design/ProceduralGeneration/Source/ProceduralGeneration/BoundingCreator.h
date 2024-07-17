// Generate bounding boxes for object detection

#pragma once
#ifndef BOUNDINGCREATOR_H
#define BOUNDINGCREATOR_H

TArray<FVector> GetBoundingPoints(UStaticMeshComponent* Mesh, UWorld* TheWorld);
TArray<FVector> GetVectorPermutations(FVector ExtentBox);

bool WasComponentRenderedRecently(float tolerance, UStaticMeshComponent* Mesh);

inline static bool SortFloat(float A, float B);
FString GetBoundingsOnScreen(FString ClassName, FString ClassLabel, UStaticMeshComponent* Mesh, UWorld* TheWorld, int ResX, int ResY);

int GetDistance(FVector PlayerLocation, FVector MeshLocation);

#endif
