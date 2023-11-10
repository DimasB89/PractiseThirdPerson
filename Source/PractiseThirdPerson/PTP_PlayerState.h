// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PTP_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PRACTISETHIRDPERSON_API APTP_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	int NumOfCoinsPickedUp = 0;
	int PreviousScore = 0;

	void CoinPickedUp();

private:
	AGameModeBase GameMode;
	
};
