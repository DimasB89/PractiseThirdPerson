// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "PTP_PickableActor.h"
#include "PTP_PickableDetector.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta = (BlueprintSpawnableComponent))
class PRACTISETHIRDPERSON_API UPTP_PickableDetector : public USphereComponent
{
	GENERATED_BODY()

public:
	UPTP_PickableDetector();

	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
};
