// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PTP_CoinsDisplayWidget.h"
#include "PTP_HUD.generated.h"

/**
 * 
 */
UCLASS()
class PRACTISETHIRDPERSON_API APTP_HUD : public AHUD
{
	GENERATED_BODY()

public:
	APTP_HUD();
	void InitializeCoinsDisplayWidget();
	void UpdateCoinDisplay(int32 coinCount);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<UPTP_CoinsDisplayWidget> CoinsDisplayWGClass;

	UPTP_CoinsDisplayWidget* CoinsDisplayWG;
	
};
