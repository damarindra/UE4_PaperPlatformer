// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperPixelCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PAPERPIXEL_API APaperPixelCharacter : public APaperCharacter
{
	GENERATED_BODY()
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))	
	class UPaperPixelMovement* PaperPixelMovement;

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation, meta = (AllowPrivateAccess = "true"))
	TMap<FName, class UPaperFlipbook*> AnimationFlipbooks;

	UFUNCTION()
    void OnCapsuleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit  );

	/* Override CanJumpInternal for enable coyote jump
	*/
	virtual bool CanJumpInternal_Implementation() const override;

	/* Override to enable coyote jump
	*/
	virtual void CheckJumpInput(float DeltaTime) override;
	
public:
	APaperPixelCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "PaperPixel")
    UPaperFlipbook* GetAnimationFlipbook(const FName Key) const;

	virtual void Jump() override;
	
	virtual void StopJumping() override;

	UFUNCTION(BlueprintCallable, Category="PaperPixel")
	void FlipSprite() const;

	/* When AxisValue is positif, it will be not flipped, and otherwise
	 */
	UFUNCTION(BlueprintCallable, Category="PaperPixel")
    void FlipSpriteByInput(float AxisValue) const;
	
	UFUNCTION(BlueprintCallable, Category="PaperPixel")
	bool IsSpriteFlipped() const;
};
