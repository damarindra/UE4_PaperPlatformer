// Copyright Epic Games, Inc. All Rights Reserved.

#include "Platformer2DGameMode.h"
#include "Platformer2DCharacter.h"

APlatformer2DGameMode::APlatformer2DGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = APlatformer2DCharacter::StaticClass();	
}
