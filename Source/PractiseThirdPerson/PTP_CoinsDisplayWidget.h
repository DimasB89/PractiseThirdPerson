// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PTP_CoinsDisplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTISETHIRDPERSON_API UPTP_CoinsDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
    UFUNCTION(BlueprintCallable, Category="Coins")
    void UpdateCoinDisplay(int32 CoinCount);

protected:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UTextBlock* CoinCountTextBlock;
};
