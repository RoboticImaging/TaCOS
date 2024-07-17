// Floor actor, which can be split into rooms with different sizes
// Modified with code from Druid mechanics at https://www.youtube.com/watch?v=dXK-bLbmwAo

#include "Floor.h"
#include "FloorNode.h"
#include "DrawDebugHelpers.h"

Floor::Floor()
{
	// Specify the size of the room and the minimum room size
	FloorGridSizeX = 15;
	FloorGridSizeY = 15;
	RoomMinX = 6;
	RoomMinY = 6;

	UnitLength = 100.f;
	SplitChance = 1.f;
}

Floor::~Floor()
{
}

void Floor::Partition()
{
	FCornerCoordinates CornerCoordinatesA = {0, 0, FloorGridSizeX, FloorGridSizeY};
	FloorNodeStack.Push(TSharedPtr<FloorNode>(new FloorNode(CornerCoordinatesA)));

	while (FloorNodeStack.Num() > 0)
	{
		TSharedPtr<FloorNode> A = FloorNodeStack.Pop();

		bool bNodeWasSplit = SplitAttempt(A);

		if (!bNodeWasSplit)
		{
			PartitionedFloor.Push(A);
		}
	}
}

int32 Floor::CoinFlip()
{
	return FMath::RandRange(0, 1);
}

bool Floor::ShouldSplitNode(TSharedPtr<FloorNode> InNode, ESplitOrientation Orientation)
{
	FCornerCoordinates CornerCoordinates = InNode->GetCornerCoordinates();

	if (Orientation == ESplitOrientation::ESO_Horizontal)
	{
		int32 FloorLength = CornerCoordinates.LowerRightY - CornerCoordinates.UpperLeftY;
		float PercentChanceOfSplit = (float)FloorLength / (float)FloorGridSizeY;
		PercentChanceOfSplit *= SplitChance;
		float DiceRoll = FMath::FRandRange(0.f, 1.f);

		if (DiceRoll > PercentChanceOfSplit)
		{
			return false;
		}
		if (FloorLength >= 2 * RoomMinY)
		{
			return true;
		}
	}
	else if (Orientation == ESplitOrientation::ESO_Vertical)
	{
		int32 FloorWidth = CornerCoordinates.LowerRightX - CornerCoordinates.UpperLeftX;
		float PercentChanceOfSplit = (float)FloorWidth / (float)FloorGridSizeX;
		PercentChanceOfSplit *= SplitChance;
		float DiceRoll = FMath::FRandRange(0.f, 1.f);

		if (DiceRoll > PercentChanceOfSplit)
		{
			return false;
		}
		if (FloorWidth >= 2 * RoomMinX)
		{
			return true;
		}
	}

	return false;
}

bool Floor::SplitAttempt(TSharedPtr<FloorNode> InNode)
{
	int32 HorizontalOrVertical = CoinFlip();

	if (HorizontalOrVertical == 0)
	{
		// Try to split horizontally
		if (ShouldSplitNode(InNode, ESplitOrientation::ESO_Horizontal))
		{
			TSharedPtr<FloorNode> B(new FloorNode());
			TSharedPtr<FloorNode> C(new FloorNode());

			SplitHorizontal(InNode, B, C);

			return true;
		}
		else if (ShouldSplitNode(InNode, ESplitOrientation::ESO_Vertical))
		{
			TSharedPtr<FloorNode> B(new FloorNode());
			TSharedPtr<FloorNode> C(new FloorNode());

			SplitVertical(InNode, B, C);

			return true;
		}
	}
	else if (HorizontalOrVertical == 1)
	{
		// Try to split vertically
		if (ShouldSplitNode(InNode, ESplitOrientation::ESO_Vertical))
		{
			TSharedPtr<FloorNode> B(new FloorNode());
			TSharedPtr<FloorNode> C(new FloorNode());

			SplitVertical(InNode, B, C);

			return true;

		}
		else if (ShouldSplitNode(InNode, ESplitOrientation::ESO_Horizontal))
		{
			TSharedPtr<FloorNode> B(new FloorNode());
			TSharedPtr<FloorNode> C(new FloorNode());

			SplitHorizontal(InNode, B, C);

			return true;
		}
	}

	return false;
}

void Floor::SplitHorizontal(TSharedPtr<FloorNode> InA, TSharedPtr<FloorNode> InB, TSharedPtr<FloorNode> InC)
{
	int32 SplitPointY = FMath::RandRange(InA->GetCornerCoordinates().UpperLeftY + RoomMinY, InA->GetCornerCoordinates().LowerRightY - RoomMinY);

	FCornerCoordinates CornerCoordinatesB;
	CornerCoordinatesB.UpperLeftX = InA->GetCornerCoordinates().UpperLeftX;
	CornerCoordinatesB.UpperLeftY = InA->GetCornerCoordinates().UpperLeftY;
	CornerCoordinatesB.LowerRightX = InA->GetCornerCoordinates().LowerRightX;
	CornerCoordinatesB.LowerRightY = SplitPointY;

	InB->SetCornerCoordinates(CornerCoordinatesB);
	FloorNodeStack.Push(InB);

	FCornerCoordinates CornerCoordinatesC;
	CornerCoordinatesC.UpperLeftX = InA->GetCornerCoordinates().UpperLeftX;
	CornerCoordinatesC.UpperLeftY = SplitPointY;
	CornerCoordinatesC.LowerRightX = InA->GetCornerCoordinates().LowerRightX;
	CornerCoordinatesC.LowerRightY = InA->GetCornerCoordinates().LowerRightY;

	InC->SetCornerCoordinates(CornerCoordinatesC);
	FloorNodeStack.Push(InC);
}

void Floor::SplitVertical(TSharedPtr<FloorNode> InA, TSharedPtr<FloorNode> InB, TSharedPtr<FloorNode> InC)
{
	int32 SplitPointX = FMath::RandRange(InA->GetCornerCoordinates().UpperLeftX + RoomMinX, InA->GetCornerCoordinates().LowerRightX - RoomMinX);

	FCornerCoordinates CornerCoordinatesB;
	CornerCoordinatesB.UpperLeftX = InA->GetCornerCoordinates().UpperLeftX;
	CornerCoordinatesB.UpperLeftY = InA->GetCornerCoordinates().UpperLeftY;
	CornerCoordinatesB.LowerRightX = SplitPointX;
	CornerCoordinatesB.LowerRightY = InA->GetCornerCoordinates().LowerRightY;

	InB->SetCornerCoordinates(CornerCoordinatesB);
	FloorNodeStack.Push(InB);

	FCornerCoordinates CornerCoordinatesC;
	CornerCoordinatesC.UpperLeftX = SplitPointX;
	CornerCoordinatesC.UpperLeftY = InA->GetCornerCoordinates().UpperLeftY;
	CornerCoordinatesC.LowerRightX = InA->GetCornerCoordinates().LowerRightX;
	CornerCoordinatesC.LowerRightY = InA->GetCornerCoordinates().LowerRightY;

	InC->SetCornerCoordinates(CornerCoordinatesC);
	FloorNodeStack.Push(InC);
}

void Floor::DrawFloorNodes(UWorld* World)
{
	for (int32 i = 0; i < PartitionedFloor.Num(); i++)
	{
		FCornerCoordinates Coordinates = PartitionedFloor[i]->GetCornerCoordinates();
		DrawFloorNode(World, Coordinates);
		PlaceFurnitures(Coordinates);
	}

	PlaceDoors();
}

void Floor::DrawFloorNode(UWorld* World, FCornerCoordinates Coordinates)
{
	const FVector UpperLeft(Coordinates.UpperLeftX * UnitLength, Coordinates.UpperLeftY * UnitLength, 0.f);
	const FVector UpperRight(Coordinates.LowerRightX * UnitLength, Coordinates.UpperLeftY * UnitLength, 0.f);
	const FVector LowerLeft(Coordinates.UpperLeftX * UnitLength, Coordinates.LowerRightY * UnitLength, 0.f);
	const FVector LowerRight(Coordinates.LowerRightX * UnitLength, Coordinates.LowerRightY * UnitLength, 0.f);
	int32 RoomLength = Coordinates.LowerRightX - Coordinates.UpperLeftX;
	int32 RoomWidth = Coordinates.LowerRightY - Coordinates.UpperLeftY;
	bool IsCornerRoom = false;

	if (Coordinates.UpperLeftX != 0)
	{
		HorizontalLineLocations.AddUnique(UpperLeft);
		HorizontalLineLocations.AddUnique(LowerLeft);

		XLocations.AddUnique(Coordinates.UpperLeftX * UnitLength);
	}
	else
	{
		IsCornerRoom = true;
	}

	if (Coordinates.LowerRightX != FloorGridSizeX)
	{
		HorizontalLineLocations.AddUnique(UpperRight);
		HorizontalLineLocations.AddUnique(LowerRight);

		XLocations.AddUnique(Coordinates.LowerRightX * UnitLength);
	}
	else
	{
		IsCornerRoom = true;
	}

	if (Coordinates.UpperLeftY != 0)
	{
		VerticalLineLocations.AddUnique(UpperLeft);
		VerticalLineLocations.AddUnique(UpperRight);

		YLocations.AddUnique(Coordinates.UpperLeftY * UnitLength);
	}
	else
	{
		IsCornerRoom = true;
	}

	if (Coordinates.LowerRightY != FloorGridSizeY)
	{
		VerticalLineLocations.AddUnique(LowerLeft);
		VerticalLineLocations.AddUnique(LowerRight);

		YLocations.AddUnique(Coordinates.LowerRightY * UnitLength);
	}
	else
	{
		IsCornerRoom = true;
	}

	if (IsCornerRoom)
	{
		PlaceWindows(Coordinates);
	}

	for (int32 i = 0; i < RoomLength; i++)
	{	
		FVector CurrentUpperWallLocation(Coordinates.UpperLeftX * UnitLength + UnitLength/2 + i * UnitLength, Coordinates.UpperLeftY * UnitLength, 0.f);
		FVector CurrentLowerWallLocation(Coordinates.UpperLeftX * UnitLength + UnitLength/2 + i * UnitLength, Coordinates.LowerRightY * UnitLength, 0.f);

		VerticalLocationsList.AddUnique(CurrentUpperWallLocation);
		VerticalLocationsList.AddUnique(CurrentLowerWallLocation);
	}

	for (int32 i = 0; i < RoomWidth; i++)
	{
		FVector CurrentLeftWallLocation(Coordinates.UpperLeftX * UnitLength, Coordinates.UpperLeftY * UnitLength + i * UnitLength + UnitLength/2, 0.f);
		FVector CurrentRightWallLocation(Coordinates.LowerRightX * UnitLength, Coordinates.LowerRightY * UnitLength - i * UnitLength - UnitLength/2, 0.f);
		FRotator CurrentWallRotation(0.f, 0.f, 0.f);

		HorizontalLocationsList.AddUnique(CurrentLeftWallLocation);
		HorizontalLocationsList.AddUnique(CurrentRightWallLocation);
	}
}

void Floor::PlaceWindows(FCornerCoordinates Coordinates)
{
	int32 WindowX, WindowY;

	if (Coordinates.UpperLeftX == 0)
	{
		WindowY = FMath::FRandRange(Coordinates.UpperLeftY, Coordinates.LowerRightY - 2);
		HorizontalWindowList.AddUnique(FVector(Coordinates.UpperLeftX * UnitLength, WindowY * UnitLength + UnitLength / 2, 0.f));
		HorizontalWindowList.AddUnique(FVector(Coordinates.UpperLeftX * UnitLength, (WindowY + 1) * UnitLength + UnitLength / 2, 0.f));
	}

	if (Coordinates.LowerRightX == FloorGridSizeX)
	{
		WindowY = FMath::FRandRange(Coordinates.UpperLeftY, Coordinates.LowerRightY - 2);
		HorizontalWindowList.AddUnique(FVector(Coordinates.LowerRightX * UnitLength, WindowY * UnitLength + UnitLength / 2, 0.f));
		HorizontalWindowList.AddUnique(FVector(Coordinates.LowerRightX * UnitLength, (WindowY + 1) * UnitLength + UnitLength / 2, 0.f));
	}

	if (Coordinates.UpperLeftY == 0)
	{
		WindowX = FMath::FRandRange(Coordinates.UpperLeftX, Coordinates.LowerRightX - 2);
		VerticalWindowList.AddUnique(FVector(WindowX * UnitLength + UnitLength / 2, Coordinates.UpperLeftY * UnitLength, 0.f));
		VerticalWindowList.AddUnique(FVector((WindowX + 1) * UnitLength + UnitLength / 2, Coordinates.UpperLeftY * UnitLength, 0.f));
	}

	if (Coordinates.LowerRightY == FloorGridSizeY)
	{
		WindowX = FMath::FRandRange(Coordinates.UpperLeftX, Coordinates.LowerRightX - 2);
		VerticalWindowList.AddUnique(FVector(WindowX * UnitLength + UnitLength / 2, Coordinates.LowerRightY * UnitLength, 0.f));
		VerticalWindowList.AddUnique(FVector((WindowX + 1) * UnitLength + UnitLength / 2, Coordinates.LowerRightY * UnitLength, 0.f));
	}
}

void Floor::PlaceDoors()
{	
	TArray<FVector> TempLocations;
	int32 MinDoorSize = 2*UnitLength;

	HorizontalLineLocations.Sort(SortByX);
	XLocations.Sort(SortInteger);

	for (int32 i = 0; i < XLocations.Num(); i++)
	{
		for (int32 j = 0; j < HorizontalLineLocations.Num(); j++)
		{
			if (HorizontalLineLocations[j][0] == XLocations[i])
			{
				TempLocations.Add(HorizontalLineLocations[j]);
			}
		}
		TempLocations.Sort(SortByY);

		for (int32 k = 1; k < TempLocations.Num(); k++)
		{
			int32 DoorX = TempLocations[0][0];

			if (TempLocations[k][1] - TempLocations[k-1][1] > MinDoorSize)
			{
				int32 DoorY = FMath::FRandRange((TempLocations[k-1][1] + UnitLength) / UnitLength, (TempLocations[k][1] - UnitLength) / UnitLength);

				FVector DoorLocation(DoorX, DoorY * UnitLength + UnitLength/2, 0.f);

				HorizontalDoorList.AddUnique(DoorLocation);
			}
		}
		TempLocations.Empty();
	}

	VerticalLineLocations.Sort(SortByY);
	YLocations.Sort(SortInteger);

	for (int32 i = 0; i < YLocations.Num(); i++)
	{
		for (int32 j = 0; j < VerticalLineLocations.Num(); j++)
		{
			if (VerticalLineLocations[j][1] == YLocations[i])
			{
				TempLocations.Add(VerticalLineLocations[j]);
			}
		}
		TempLocations.Sort(SortByX);

		for (int32 k = 1; k < TempLocations.Num(); k++)
		{
			int32 DoorY = TempLocations[0][1];

			if (TempLocations[k][0] - TempLocations[k - 1][0] > MinDoorSize)
			{
				int32 DoorX = FMath::FRandRange((TempLocations[k - 1][0] + UnitLength) / UnitLength, (TempLocations[k][0] - UnitLength) / UnitLength);

				FVector DoorLocation(DoorX * UnitLength + UnitLength / 2, DoorY, 0.f);

				VerticalDoorList.AddUnique(DoorLocation);
			}
		}
		TempLocations.Empty();
	}
}

void Floor::PlaceFurnitures(FCornerCoordinates Coordinates)
{
	int32 RoomLength = Coordinates.LowerRightX - Coordinates.UpperLeftX;
	int32 RoomWidth = Coordinates.LowerRightY - Coordinates.UpperLeftY;
	int32 Distance2Wall = 3;
	int32 LargeRoomX = 10;
	float XLocation;
	float YLocation;
	float Yaw;
	FFurnitureTransform FurnitureTransform;

	if (RoomLength >= LargeRoomX && RoomWidth >= LargeRoomX)
	{
		XLocation = FMath::FRandRange(Coordinates.UpperLeftX + Distance2Wall, Coordinates.UpperLeftX + (Coordinates.LowerRightX - Coordinates.UpperLeftX) / 2 - Distance2Wall);
		YLocation = FMath::FRandRange(Coordinates.UpperLeftY + Distance2Wall, Coordinates.UpperLeftY + (Coordinates.LowerRightY - Coordinates.UpperLeftY) / 2 - Distance2Wall);
		Yaw = 90.f * FMath::RandRange(-1, 2);

		FurnitureTransform.Location = FVector(XLocation * UnitLength, YLocation * UnitLength, 0);
		FurnitureTransform.Rotation = FRotator(0.f, Yaw, 0.f);
		FurnitureTransformList.Add(FurnitureTransform);

		XLocation = FMath::FRandRange(Coordinates.UpperLeftX + (Coordinates.LowerRightX - Coordinates.UpperLeftX) / 2 + Distance2Wall, Coordinates.LowerRightX - Distance2Wall);
		YLocation = FMath::FRandRange(Coordinates.UpperLeftY + Distance2Wall, Coordinates.UpperLeftY + (Coordinates.LowerRightY - Coordinates.UpperLeftY) / 2 - Distance2Wall);
		Yaw = 90.f * FMath::RandRange(-1, 2);

		FurnitureTransform.Location = FVector(XLocation * UnitLength, YLocation * UnitLength, 0);
		FurnitureTransform.Rotation = FRotator(0.f, Yaw, 0.f);
		FurnitureTransformList.Add(FurnitureTransform);

		XLocation = FMath::FRandRange(Coordinates.UpperLeftX + Distance2Wall, Coordinates.UpperLeftX + (Coordinates.LowerRightX - Coordinates.UpperLeftX) / 2 - Distance2Wall);
		YLocation = FMath::FRandRange(Coordinates.UpperLeftY + (Coordinates.LowerRightY - Coordinates.UpperLeftY) / 2 + Distance2Wall, Coordinates.LowerRightY - Distance2Wall);
		Yaw = 90.f * FMath::RandRange(-1, 2);

		FurnitureTransform.Location = FVector(XLocation * UnitLength, YLocation * UnitLength, 0);
		FurnitureTransform.Rotation = FRotator(0.f, Yaw, 0.f);
		FurnitureTransformList.Add(FurnitureTransform);

		XLocation = FMath::FRandRange(Coordinates.UpperLeftX + (Coordinates.LowerRightX - Coordinates.UpperLeftX) / 2 + Distance2Wall, Coordinates.LowerRightX - Distance2Wall);
		YLocation = FMath::FRandRange(Coordinates.UpperLeftY + (Coordinates.LowerRightY - Coordinates.UpperLeftY) / 2 + Distance2Wall, Coordinates.LowerRightY - Distance2Wall);
		Yaw = 90.f * FMath::RandRange(-1, 2);

		FurnitureTransform.Location = FVector(XLocation * UnitLength, YLocation * UnitLength, 0);
		FurnitureTransform.Rotation = FRotator(0.f, Yaw, 0.f);
		FurnitureTransformList.Add(FurnitureTransform);
	}
	else if (RoomLength >= LargeRoomX && RoomWidth < LargeRoomX)
	{
		XLocation = FMath::FRandRange(Coordinates.UpperLeftX + Distance2Wall, Coordinates.UpperLeftX + (Coordinates.LowerRightX - Coordinates.UpperLeftX) / 2 - Distance2Wall);
		YLocation = FMath::FRandRange(Coordinates.UpperLeftY + Distance2Wall, Coordinates.LowerRightY - Distance2Wall);
		Yaw = 90.f * FMath::RandRange(-1, 2);

		FurnitureTransform.Location = FVector(XLocation * UnitLength, YLocation * UnitLength, 0);
		FurnitureTransform.Rotation = FRotator(0.f, Yaw, 0.f);
		FurnitureTransformList.Add(FurnitureTransform);

		XLocation = FMath::FRandRange(Coordinates.UpperLeftX + (Coordinates.LowerRightX - Coordinates.UpperLeftX) / 2 + Distance2Wall, Coordinates.LowerRightX - Distance2Wall);
		YLocation = FMath::FRandRange(Coordinates.UpperLeftY + Distance2Wall, Coordinates.LowerRightY - Distance2Wall);
		Yaw = 90.f * FMath::RandRange(-1, 2);

		FurnitureTransform.Location = FVector(XLocation * UnitLength, YLocation * UnitLength, 0);
		FurnitureTransform.Rotation = FRotator(0.f, Yaw, 0.f);
		FurnitureTransformList.Add(FurnitureTransform);
	}
	else if (RoomLength < LargeRoomX && RoomWidth >= LargeRoomX)
	{
		XLocation = FMath::FRandRange(Coordinates.UpperLeftX + Distance2Wall, Coordinates.LowerRightX - Distance2Wall);
		YLocation = FMath::FRandRange(Coordinates.UpperLeftY + Distance2Wall, Coordinates.UpperLeftY + (Coordinates.LowerRightY - Coordinates.UpperLeftY) / 2 - Distance2Wall);
		Yaw = 90.f * FMath::RandRange(-1, 2);

		FurnitureTransform.Location = FVector(XLocation * UnitLength, YLocation * UnitLength, 0);
		FurnitureTransform.Rotation = FRotator(0.f, Yaw, 0.f);
		FurnitureTransformList.Add(FurnitureTransform);

		XLocation = FMath::FRandRange(Coordinates.UpperLeftX + Distance2Wall, Coordinates.LowerRightX - Distance2Wall);
		YLocation = FMath::FRandRange(Coordinates.UpperLeftY + (Coordinates.LowerRightY - Coordinates.UpperLeftY) / 2 + Distance2Wall, Coordinates.LowerRightY - Distance2Wall);
		Yaw = 90.f * FMath::RandRange(-1, 2);

		FurnitureTransform.Location = FVector(XLocation * UnitLength, YLocation * UnitLength, 0);
		FurnitureTransform.Rotation = FRotator(0.f, Yaw, 0.f);
		FurnitureTransformList.Add(FurnitureTransform);
	}
	else
	{
		XLocation = FMath::FRandRange(Coordinates.UpperLeftX + Distance2Wall, Coordinates.LowerRightX - Distance2Wall);
		YLocation = FMath::FRandRange(Coordinates.UpperLeftY + Distance2Wall, Coordinates.LowerRightY - Distance2Wall);
		Yaw = 90.f * FMath::RandRange(-1, 2);

		FurnitureTransform.Location = FVector(XLocation * UnitLength, YLocation * UnitLength, 0);
		FurnitureTransform.Rotation = FRotator(0.f, Yaw, 0.f);
		FurnitureTransformList.Add(FurnitureTransform);
	}
}

inline bool Floor::SortByX(FVector A, FVector B)
{
	return (A[0] < B[0]);
}

inline bool Floor::SortByY(FVector A, FVector B)
{
	return (A[1] < B[1]);
}

inline bool Floor::SortInteger(int32 A, int32 B)
{
	return (A < B);
}
