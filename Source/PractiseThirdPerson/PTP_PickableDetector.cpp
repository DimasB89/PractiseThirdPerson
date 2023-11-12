// Fill out your copyright notice in the Description page of Project Settings.


#include "PTP_PickableDetector.h"
#include "PTP_PickableActor.h"
#include <Kismet/GameplayStatics.h>
#include "PTP_PlayerController.h"

UPTP_PickableDetector::UPTP_PickableDetector()
{
	SetGenerateOverlapEvents(true);
	SetCollisionProfileName(TEXT("Trigger"));
}

void UPTP_PickableDetector::BeginPlay()
{
	Super::BeginPlay();
	
	OnComponentBeginOverlap.AddDynamic(this, &UPTP_PickableDetector::OnPickupBeginOverlap);
	//endoverlap

	MyPlayerController = UGameplayStatics::GetPlayerController(this, 0);
}

void UPTP_PickableDetector::OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetOwner()) {
		APTP_PickableActor* PickableActor = Cast<APTP_PickableActor>(OtherActor);
		if (PickableActor) {
			//hansle collision with pickable actor
			PickableActor->Destroy();
			if (MyPlayerController) {
				Cast<APTP_PlayerController>(MyPlayerController)->CoinPickedUp();
			}
		}
	}
}
