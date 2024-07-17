// Floor actor, which can be split into rooms with different sizes
// Modified with code from Druid mechanics at https://www.youtube.com/watch?v=dXK-bLbmwAo

#pragma once
#include "FloorNode.h"

enum class ESplitOrientation
{
	ESO_Horizontal,
	ESO_Vertical
};

struct FFurnitureTransform
{
	FVector Location;
	FRotator Rotation;
};

class Floor
{
public:
	Floor();
	~Floor();

	void Partition();
	int32 CoinFlip();
	bool ShouldSplitNode(TSharedPtr<FloorNode> InNode, ESplitOrientation Orientation);
	bool SplitAttempt(TSharedPtr<FloorNode> InNode);
	void SplitHorizontal(TSharedPtr<FloorNode> InA, TSharedPtr<FloorNode> InB, TSharedPtr<FloorNode> InC);
	void SplitVertical(TSharedPtr<FloorNode> InA, TSharedPtr<FloorNode> InB, TSharedPtr<FloorNode> InC);

	void DrawFloorNodes(class UWorld* World);
	void DrawFloorNode(UWorld* World, FCornerCoordinates Coordinates);
	void PlaceWindows(FCornerCoordinates Coordinates);
	void PlaceDoors();
	void PlaceFurnitures(FCornerCoordinates Coordinates);

	inline static bool SortByX(FVector A, FVector B);
	inline static bool SortByY(FVector A, FVector B);
	inline static bool SortInteger(int32 A, int32 B);

	FORCEINLINE TArray<TSharedPtr<FloorNode>> GetPartitionedFloor() const { return PartitionedFloor;  }
	FORCEINLINE TArray<FVector> GetHorizontalWallLocations() const { return HorizontalLocationsList; }
	FORCEINLINE TArray<FVector> GetVerticalWallLocations() const { return VerticalLocationsList; }
	FORCEINLINE TArray<FVector> GetHorizontalDoors() const { return HorizontalDoorList; }
	FORCEINLINE TArray<FVector> GetVerticalDoors() const { return VerticalDoorList; }
	FORCEINLINE TArray<FVector> GetHorizontalWindows() const { return HorizontalWindowList; }
	FORCEINLINE TArray<FVector> GetVerticalWindows() const { return VerticalWindowList; }
	FORCEINLINE TArray<FFurnitureTransform> GetFurnitureTransforms() const { return FurnitureTransformList; }

private:
	TArray<TSharedPtr<FloorNode>> FloorNodeStack;
	TArray<TSharedPtr<FloorNode>> PartitionedFloor;
	TArray<FVector> HorizontalLocationsList;
	TArray<FVector> VerticalLocationsList;
	TArray<FVector> HorizontalLineLocations;
	TArray<FVector> VerticalLineLocations;
	TArray<int32> XLocations;
	TArray<int32> YLocations;
	TArray<FVector> HorizontalDoorList;
	TArray<FVector> VerticalDoorList;
	TArray<FVector> HorizontalWindowList;
	TArray<FVector> VerticalWindowList;
	TArray<FFurnitureTransform> FurnitureTransformList;

	int32 FloorGridSizeX;
	int32 FloorGridSizeY;
	int32 RoomMinX;
	int32 RoomMinY;

	float UnitLength;
	float SplitChance;
};