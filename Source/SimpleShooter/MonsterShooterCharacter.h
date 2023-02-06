// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterShooterCharacter.generated.h"

UCLASS()
class SIMPLESHOOTER_API AMonsterShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* HandsMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* GunMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* MuzzleLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float TurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float LookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		FVector GunOffset;

	UPROPERTY(EditAnywhere)
		float JumpMovement = 1.0f;

protected:

	void OnFire(TSubclassOf<class AProjectile> MyProjectile, USoundBase* sound);
	void OnLeftMouse();
	void OnRightMouse();
	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookAtRate(float Rate);

	// projectile props
public:
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> Projectile;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* AlterFireSound;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	class UAnimInstance* AnimInstance;
	class UWorld* World;

	//TSubclassOf<class AProjectile> MyProjectile;

	FRotator SpawnRotation;
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Health = 100.0f;

	void DealDamage(float DamageAmount);

};
