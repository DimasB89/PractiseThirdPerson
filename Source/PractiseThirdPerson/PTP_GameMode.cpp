// Fill out your copyright notice in the Description page of Project Settings.


#include "PTP_GameMode.h"
#include <GameFramework/PlayerController.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

void APTP_GameMode::ExitGame(int NumOfCoinsPickedUp)
{
	if(NumOfCoinsPickedUp == NumOfCoinsNeededToExitGame){
		//exit game
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    
    	if (PlayerController)
    	{
        	UWorld* World = PlayerController->GetWorld();

        	if (World)
        	{
           	 	// The parameters are: World context, player controller, quit reason, and bShouldForce
            	UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
        	}
    	}
	}
	//NumOfPickedUpCoins++;
}
