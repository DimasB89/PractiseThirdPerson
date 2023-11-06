// Copyright Epic Games, Inc. All Rights Reserved.

#include "PractiseThirdPersonGameMode.h"
#include "PractiseThirdPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

APractiseThirdPersonGameMode::APractiseThirdPersonGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
