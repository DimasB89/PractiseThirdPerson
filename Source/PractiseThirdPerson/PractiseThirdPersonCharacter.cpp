// Copyright Epic Games, Inc. All Rights Reserved.

#include "PractiseThirdPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TestActor.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include "PTP_PlayerController.h"


//////////////////////////////////////////////////////////////////////////
// APractiseThirdPersonCharacter

APractiseThirdPersonCharacter::APractiseThirdPersonCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void APractiseThirdPersonCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	//if (APTP_PlayerController* PlayerController = Cast<APTP_PlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);// 0 - stands for priority
			Subsystem->AddMappingContext(FirstMapContext, 0);
			bFirstMappingApplied = true;
		}
	}

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATestActor::StaticClass(), OutActors);
	if(OutActors.Num() >= 2){
		FirstObject = OutActors[0];
		SecondObject = OutActors[1];
	}
}

void APractiseThirdPersonCharacter::CheckActorsInFront(TArray<AActor *> &OutActors)
{
	FVector ActorToFirstObjDirection = (FirstObject->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	float dotp1 = FVector::DotProduct(ActorToFirstObjDirection, GetActorForwardVector());
	if(dotp1 > 0.0f){
		OutActors.Add(FirstObject);
	}
	FVector ActorToSecondObjDirection = (SecondObject->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	float dotp2 = FVector::DotProduct(ActorToSecondObjDirection, GetActorForwardVector());
	if(dotp2 > 0.0f){
		OutActors.Add(SecondObject);
	}
}

AActor *APractiseThirdPersonCharacter::ClosestActorInSight()
{
	TArray<AActor *> OutActors;
	CheckActorsInFront(OutActors);

	if(OutActors.Num() > 1){
		const float dist1 = FVector::DistSquared(GetActorLocation(), OutActors[0]->GetActorLocation());
		const float dist2 = FVector::DistSquared(GetActorLocation(), OutActors[1]->GetActorLocation());
		return dist1 <= dist2 ? OutActors[0] : OutActors[1];
	}else if(OutActors.Num() == 1){
		return OutActors[0];
	}else{
		return nullptr;
	}
    return nullptr;
}

void APractiseThirdPersonCharacter::AddNearbyPickableActor(AActor *Actor)
{
	if (Actor && !StoredPickables.Contains(Actor)){
		StoredPickables.Add(Actor);
	}
}

void APractiseThirdPersonCharacter::RemoveNearbyPickableActor(AActor *Actor)
{
	if(Actor){
		StoredPickables.Remove(Actor);
	}
}

AActor *APractiseThirdPersonCharacter::GetClosestPickableActor()
{
    AActor* ClosestActor = nullptr;
    float MinDistanceSquared = FLT_MAX; // Initialize with the maximum possible float value

    // Get the current location of your character or the component
    FVector MyLocation = GetActorLocation(); // Or GetComponentLocation() if this is a component

    for (AActor* Actor : StoredPickables)
    {
        if (Actor)
        {
            float DistanceSquared = FVector::DistSquared(MyLocation, Actor->GetActorLocation());
            if (DistanceSquared < MinDistanceSquared)
            {
                MinDistanceSquared = DistanceSquared;
                ClosestActor = Actor;
            }
        }
    }

    return ClosestActor;
}

void APractiseThirdPersonCharacter::PickupClosestActor()
{
    AActor* ClosestActor = GetClosestPickableActor();
    if (ClosestActor && StoredPickables.Contains(ClosestActor))
    {
		
        ClosestActor->Destroy();
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
			Cast<APTP_PlayerController>(PlayerController)->CoinPickedUp();
		}

		// Remove the actor from StoredPickables
        StoredPickables.Remove(ClosestActor);
    }
}

//////////////////////////////////////////////////////////////////////////
// Input

void APractiseThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APractiseThirdPersonCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APractiseThirdPersonCharacter::Look);

		//PickUp
		EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Triggered, this, &APractiseThirdPersonCharacter::PickupClosestActor);

		//Exercise 13.11.23
		
		EnhancedInputComponent->BindAction(MsgAction, ETriggerEvent::Started, this, &APractiseThirdPersonCharacter::PressBtn);
		EnhancedInputComponent->BindAction(MsgAction, ETriggerEvent::Completed, this, &APractiseThirdPersonCharacter::ReleaseBtn);
		EnhancedInputComponent->BindAction(MsgAction, ETriggerEvent::Ongoing, this, &APractiseThirdPersonCharacter::HoldBtn);
		EnhancedInputComponent->BindAction(DoubleInputAction, ETriggerEvent::Triggered, this, &APractiseThirdPersonCharacter::TwoBtnsPress);
		EnhancedInputComponent->BindAction(SwitchInputContextAction, ETriggerEvent::Triggered, this, &APractiseThirdPersonCharacter::SwitchInputMappings);
	}
}

void APractiseThirdPersonCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APractiseThirdPersonCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APractiseThirdPersonCharacter::PressBtn(const FInputActionValue &Value)
{
	UKismetSystemLibrary::PrintString(GetWorld(),TEXT("Button has been pressed"));
}

void APractiseThirdPersonCharacter::HoldBtn(const FInputActionValue &Value)
{
	UKismetSystemLibrary::PrintString(GetWorld(),TEXT("Button is held"));
}

void APractiseThirdPersonCharacter::ReleaseBtn(const FInputActionValue &Value)
{
	UKismetSystemLibrary::PrintString(GetWorld(),TEXT("Button has been released"));
}

void APractiseThirdPersonCharacter::TwoBtnsPress(const FInputActionValue &Value)
{
	UKismetSystemLibrary::PrintString(GetWorld(),TEXT("Two buttons have been pressed"));
}

void APractiseThirdPersonCharacter::SwitchInputMappings(const FInputActionValue &Value)
{
	UKismetSystemLibrary::PrintString(GetWorld(),TEXT("Context Input Mappings have been switched"));

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (bFirstMappingApplied)
			{
				Subsystem->RemoveMappingContext(FirstMapContext);
				Subsystem->AddMappingContext(SecondMapContext, 0);
				bFirstMappingApplied = false;
			}else{
				Subsystem->RemoveMappingContext(SecondMapContext);
				Subsystem->AddMappingContext(FirstMapContext, 0);
				bFirstMappingApplied = true;
			}
		}
	}
	

	
	
}
