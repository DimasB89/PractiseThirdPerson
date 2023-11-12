// Fill out your copyright notice in the Description page of Project Settings.


#include "PTP_GameMode.h"
#include <GameFramework/PlayerController.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

void APTP_GameMode::AttemptToExitGame(int NumOfCoinsPickedUp)
{
	if(NumOfCoinsPickedUp == NumOfCoinsNeededToExitGame){
			//exit game
			// Set a timer to call a function after 1 second
    		GetWorld()->GetTimerManager().SetTimer(ExitTimerHandle, this, &APTP_GameMode::ExitGame, 1.0f, false);
    	}
}

void APTP_GameMode::ExitGame(){

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    
    	if (PlayerController)
    	{
        	UWorld* World = PlayerController->GetWorld();

        	if (World)
        	{
				UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
        	}
    	}
}
