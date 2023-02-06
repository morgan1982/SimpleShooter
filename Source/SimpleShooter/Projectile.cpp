// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Bot_One.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	CollisionSphere->InitSphereRadius(10.0f);

	// sets the sphere as the root of all other components that might be added to the class 
	RootComponent = CollisionSphere;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movemement"));

	ProjectileMovement->UpdatedComponent = CollisionSphere;

	
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 6000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;



	// lifespan of projectile in seconds
	InitialLifeSpan = 5.0f;

}

// with this method ititial and max can set via the editor!!
void AProjectile::OnConstruction(const FTransform& Transform)
{
	ProjectileMovement->InitialSpeed = ProjectileInitialSpeed;
	ProjectileMovement->MaxSpeed = ProjectileMaxSpeed;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();


	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnHit);
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	ABot_One* Enemy = Cast<ABot_One>(OtherActor);
	if (Enemy)
	{
		Enemy->DealDamage(DamageValue);
		Destroy(); // to destroy the prjectile after touching the enemy
	}

}

