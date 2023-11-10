// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PTP_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class PRACTISETHIRDPERSON_API APTP_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void PlayerPickedUpCoin();
	int NumOfPickedUpCoins = 0;
	
};
