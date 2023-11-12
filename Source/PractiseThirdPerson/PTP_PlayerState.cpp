// Fill out your copyright notice in the Description page of Project Settings.


#include "PTP_PlayerState.h"
#include "PTP_GameMode.h"
#include <Kismet/GameplayStatics.h>

/*int APTP_PlayerState::GetNumOfCoinsPickedUp()
{
    return NumOfCoinsPickedUp;
}*/

void APTP_PlayerState::CoinPickedUp()
{
	if (!GameMode) {
		GameMode = UGameplayStatics::GetGameMode(GetWorld());
	}
	NumOfCoinsPickedUp++;
	//PreviousScore = NumOfCoinsPickedUp - 1;

	if (NumOfCoinsPickedUp >= 5) {
		Cast<APTP_GameMode>(GameMode)->ExitGame(NumOfCoinsPickedUp);////TUT OSTANOVILSJA
	}
}
