// Fill out your copyright notice in the Description page of Project Settings.


#include "PTP_HUD.h"
#include <UObject/ConstructorHelpers.h>
#include "PTP_CoinsDisplayWidget.h"

APTP_HUD::APTP_HUD()
{
    // ConstructorHelpers::FClassFinder is used to find the widget class
    static ConstructorHelpers::FClassFinder<UPTP_CoinsDisplayWidget> CoinsDisplayWGClassFinder(TEXT("/Game/ThirdPerson/Blueprints/BP_UICoinsDisplay"));
    if (CoinsDisplayWGClassFinder.Succeeded())
    {
        CoinsDisplayWGClass = CoinsDisplayWGClassFinder.Class;
    }
}

void APTP_HUD::InitializeCoinsDisplayWidget()
{

    if (CoinsDisplayWGClass) // Check if the widget class is set
    {
        CoinsDisplayWG = CreateWidget<UPTP_CoinsDisplayWidget>(GetWorld(), CoinsDisplayWGClass);
        if (CoinsDisplayWG)
        {
            CoinsDisplayWG->AddToViewport();
            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Coins Display Widget Added to viewPort"));
        }
    }
}

void APTP_HUD::UpdateCoinDisplay(int32 coinCount){
    CoinsDisplayWG->UpdateCoinDisplay(coinCount);
}
