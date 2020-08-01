// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"


#include "PlatformerPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API APlatformerPlayer : public APaperCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))	
	class UPlatformer2DMovement* Platformer2DMovement;

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation, meta = (AllowPrivateAccess = "true"))
	TMap<FName, class UPaperFlipbook*> AnimationFlipbooks;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category="Platformer|Movement")
    void MoveHorizontal(float AxisValue);

	UFUNCTION()
	void OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit  );

	/* Override CanJumpInternal for enable coyote jump
	 */
	virtual bool CanJumpInternal_Implementation() const override;

	/* Override to enable coyote jump
	 */
	virtual void CheckJumpInput(float DeltaTime) override;
	
public:
	APlatformerPlayer(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Platformer|Animation")
	UPaperFlipbook* GetAnimationFlipbook(const FName Key) const;

	virtual void Jump() override;
	
	virtual void StopJumping() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector WalkDirection = FVector::ForwardVector;
};
