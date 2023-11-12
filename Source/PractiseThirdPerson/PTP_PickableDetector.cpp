// Fill out your copyright notice in the Description page of Project Settings.


#include "PTP_PickableDetector.h"
#include "PTP_PickableActor.h"
#include <Kismet/GameplayStatics.h>
#include "PTP_PlayerController.h"
#include "PTP_HUD.h"
#include "PractiseThirdPersonCharacter.h"

UPTP_PickableDetector::UPTP_PickableDetector()
{
	SetGenerateOverlapEvents(true);
	SetCollisionProfileName(TEXT("Trigger"));
}

void UPTP_PickableDetector::BeginPlay()
{
	Super::BeginPlay();
	
	OnComponentBeginOverlap.AddDynamic(this, &UPTP_PickableDetector::OnPickupBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UPTP_PickableDetector::OnPickupEndOverlap);

	MyPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (MyPlayerController)
        {
            APTP_HUD* HUD = Cast<APTP_HUD>(MyPlayerController->GetHUD());
			if(HUD){
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hud loaded in pickable detector"));
				HUD->InitializeCoinsDisplayWidget();
				HUD->UpdateCoinDisplay(0);
			}
        }
}

void UPTP_PickableDetector::OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetOwner()) {
		APTP_PickableActor* PickableActor = Cast<APTP_PickableActor>(OtherActor);
		if (PickableActor) {
			//handle collision with pickable actor

			//old way
			/*PickableActor->Destroy();
			if (MyPlayerController) {
				Cast<APTP_PlayerController>(MyPlayerController)->CoinPickedUp();
			}*/

			Cast<APractiseThirdPersonCharacter>(GetOwner())->AddNearbyPickableActor(OtherActor);

		}
	}
}

void UPTP_PickableDetector::OnPickupEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{

	 UE_LOG(LogTemp, Warning, TEXT("Overlap Ended with Actor: %s"), *OtherActor->GetName());
    if (OtherActor && OtherActor != GetOwner()) {
		APTP_PickableActor* PickableActor = Cast<APTP_PickableActor>(OtherActor);
		if (PickableActor) {
			Cast<APractiseThirdPersonCharacter>(GetOwner())->RemoveNearbyPickableActor(OtherActor);
			UE_LOG(LogTemp, Warning, TEXT("Coin removed from StoredPickables"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Coin removed from StoredPickables"));
		}
	}
}
