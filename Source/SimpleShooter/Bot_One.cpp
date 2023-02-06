// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot_One.h"

#include "Components/BoxComponent.h"
#include "MonsterShooterCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

// Sets default values
ABot_One::ABot_One()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(RootComponent);

	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = 1250.0f;
	SightConfig->LoseSightRadius = 1280.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->SetMaxAge(0.1f); // forget the target. if it is 0 it will never forget the player 

	AIPerComp->ConfigureSense(*SightConfig);
	AIPerComp->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerComp->OnPerceptionUpdated.AddDynamic(this, &ABot_One::OnSensed);

	CurrentVelocity = FVector::ZeroVector;
	MovementSpeed = 200.0f;

	DistanceSquared = BIG_NUMBER;
}

// Called when the game starts or when spawned
void ABot_One::BeginPlay()
{
	Super::BeginPlay();

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ABot_One::OnHit);

	BaseLocation = this->GetActorLocation();
	
}

// Called every frame
void ABot_One::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentVelocity.IsZero())
	{
		NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;

		if (BackToBaseLocation)
		{
			//todo: maybe this is affects performance, have to check
			float DistanceFromBaseLocation = (NewLocation - BaseLocation).SizeSquared2D();
			if ((DistanceFromBaseLocation < DistanceSquared))
			{
				DistanceSquared = DistanceFromBaseLocation;
			}
			else
			{
				// stop the actor and reset the variables
				CurrentVelocity = FVector::ZeroVector; 
				DistanceSquared = BIG_NUMBER; 
				BackToBaseLocation = false; 
				
				SetNewRotation(GetActorForwardVector(), GetActorLocation());
			}
		}

		SetActorLocation(NewLocation);
	}

}

// Called to bind functionality to input
void ABot_One::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABot_One::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{

}

void ABot_One::OnSensed(const TArray<AActor*>& UpdatedActors)
{
	//check if something is detected
	for (int i = 0; i < UpdatedActors.Num(); i++)
	{
		FActorPerceptionBlueprintInfo Info;

		AIPerComp->GetActorsPerception(UpdatedActors[i], Info);

		if (Info.LastSensedStimuli[0].WasSuccessfullySensed())
		{
			FVector dir = UpdatedActors[i]->GetActorLocation() - GetActorLocation(); // last seen actor location minus own location
			dir.Z = 0.0f;

			CurrentVelocity = dir.GetSafeNormal() * MovementSpeed; // normalizes the vector so it can multiply with speed

			SetNewRotation(UpdatedActors[i]->GetActorLocation(), GetActorLocation());
		}
		else
		{
			FVector dir = BaseLocation - GetActorLocation();
			dir.Z = 0.0f;

			if (dir.SizeSquared2D() > 1.0f)
			{
				CurrentVelocity = dir.GetSafeNormal() * MovementSpeed;
				BackToBaseLocation = true;

				SetNewRotation(BaseLocation, GetActorLocation());
			}



		}
	}
}

void ABot_One::SetNewRotation(FVector TargetPosition, FVector CurrentPosition)
{
	FVector NewDirection = TargetPosition - CurrentPosition;
	NewDirection.Z = 0.0f;

	EnemyRotation = NewDirection.Rotation();
	SetActorRotation(EnemyRotation);

}

void ABot_One::DealDamage(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health >= 0.0f)
	{
		Destroy();
	}
}

