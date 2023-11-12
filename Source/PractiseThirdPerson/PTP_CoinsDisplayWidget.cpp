// Fill out your copyright notice in the Description page of Project Settings.


#include "PTP_CoinsDisplayWidget.h"
#include "Components/TextBlock.h"

void UPTP_CoinsDisplayWidget::UpdateCoinDisplay(int32 CoinCount)
{
    if (CoinCountTextBlock) // TextBlockPointer is a reference to your Text widget
    {
        FString CoinText = FString::Printf(TEXT("COINS PICKED: %d"), CoinCount);
        CoinCountTextBlock->SetText(FText::FromString(CoinText));
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Coin Count Text should be changed now"));
    }/*else{
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CoinCountTextBlock not referenced"));
    }*/
}

