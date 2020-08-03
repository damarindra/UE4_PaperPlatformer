// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PaperPixelMovement.generated.h"

/**
 * 
 */
UCLASS()
class PAPERPIXEL_API UPaperPixelMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()

	friend class APaperPixelCharacter;
	
	/* Use method Velocity Clamp when jumping. This really good when creating controlable jump.
	Automatically set the JumpZVelocity and MaxJumpHoldTime at begin play. So setup this 2 variable at blueprint is useless if this enabled
	*/
	UPROPERTY(EditAnywhere,  BlueprintReadOnly, Category="Character Movement: Jumping / Falling", meta = (AllowPrivateAccess = "true"))
	bool bJumpVelocityClamp = true;

	UPROPERTY(EditAnywhere, Category="Character Movement: Jumping / Falling", meta = (UIMin = "0.0", UIMax = "1.0", editcondition = "bJumpVelocityClamp", DisplayAfter="bJumpVelocityClamp"))
	float MaxJumpHeight = 16;
	
	UPROPERTY(EditAnywhere, Category="Character Movement: Jumping / Falling", meta = (UIMin = "0.0", UIMax = "1.0", editcondition = "bJumpVelocityClamp", DisplayAfter="MaxJumpHeight"))
	float MinJumpHeightPercentage = 0.2;

	/* Calculate the JumpZVelocity and CharacterOwner->JumpMaxHoldTime
	Modify GravityScale and MaxJumpHeight to make different result
	See OutputLog to see the value
	*/
	UFUNCTION(CallInEditor, Category="Character Movement: Jumping / Falling")
    void CalculateAndPrintLogJumpingValue();

	UPROPERTY()
	APaperPixelCharacter* PlayerOwner;

	float TimeSinceFall = 0.0;

protected:
	/* Set 0 to disable CoyoteJump
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Movement: Jumping / Falling", meta=(AllowPrivateAccess = "true"))
	float CoyoteJumpTime = 0.085;

	bool CanCoyoteJump() const;
	
	UFUNCTION(BlueprintCallable, Category="Pawn|Components|CharacterMovement")
    void CalculateJumpVelocityClamp();

public:
	UPaperPixelMovement();
	

	UFUNCTION(BlueprintCallable, Category="Pawn|Components|CharacterMovement")
    float GetMinJumpHeight() const;

	/* Finishup remaining velocity calculation.
	*/
	UFUNCTION(BlueprintCallable, Category="Pawn|Components|CharacterMovement")
    void FinishUpJumping();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;
};
