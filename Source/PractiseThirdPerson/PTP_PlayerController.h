// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PTP_PlayerController.generated.h"



//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCoinPickupDelegate);
/**
 * 
 */
UCLASS()
class PRACTISETHIRDPERSON_API APTP_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//UPROPERTY(BlueprintAssignable, Category = "Events")
	//FOnCoinPickupDelegate OnCoinPickup;
	AGameModeBase* GameMode;
	APlayerState* PlayerState;

	void CoinPickedUp();
	
};
