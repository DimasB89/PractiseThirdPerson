// Fill out your copyright notice in the Description page of Project Settings.


#include "PTP_PlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "PTP_PlayerState.h"
#include "PTP_GameMode.h"

void APTP_PlayerController::CoinPickedUp()
{
	//OnCoinPickup.Broadcast();

	/*if (!GameMode) {
		GameMode = UGameplayStatics::GetGameMode(GetWorld());
	}*/
	if (!PlayerState) {
		PlayerState = UGameplayStatics::GetPlayerState(GetWorld(), 0);
	}

	if (PlayerState) {
		Cast<APTP_PlayerState>(PlayerState)->CoinPickedUp();
	}

	/*if (GameMode) {
		Cast<APTP_GameMode>(GameMode)->PlayerPickedUpCoin();
	}*/


}
