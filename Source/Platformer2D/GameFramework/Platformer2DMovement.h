// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Platformer2D/Platformer2DTypes.h"

#include "Platformer2DMovement.generated.h"

class APlatformerPlayer;

/**
 * 
 */
UCLASS()
class PLATFORMER2D_API UPlatformer2DMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()

	friend class APlatformerPlayer;

	UPROPERTY(EditAnywhere, Category="Character Movement: Jumping / Falling|Velocity Clamp", meta = (UIMin = "0.0", UIMax = "1.0"))
	float MaxJumpHeight = 16;
	
	UPROPERTY(EditAnywhere, Category="Character Movement: Jumping / Falling|Velocity Clamp", meta = (UIMin = "0.0", UIMax = "1.0"))
	float MinJumpHeightPercentage = 0.2;

	/* Calculate the JumpZVelocity and CharacterOwner->JumpMaxHoldTime
	  Modify GravityScale and MaxJumpHeight to make different result
	  See OutputLog to see the value
	 */
	UFUNCTION(CallInEditor, Category="Character Movement: Jumping / Falling")
	void CalculateAndPrintLogJumpingValue();

	UPROPERTY()
	APlatformerPlayer* PlayerOwner;

	float TimeSinceFall = 0.0;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Movement: Jumping / Falling", meta=(AllowPrivateAccess = "true"))
	float CoyoteJumpTime = 0.05;
	

public:
	UPlatformer2DMovement();
	
	/* Use method Velocity Clamp when jumping. This really good when creating controlable jump. Don't forget to CalculateJumpingValue
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Movement: Jumping / Falling")
	bool bJumpVelocityClamp = true;

	UFUNCTION(BlueprintCallable, Category="Pawn|Components|CharacterMovement")
	float GetMinJumpHeight() const;

	/* Finishup remaining velocity calculation.
	 */
	UFUNCTION(BlueprintCallable, Category="Pawn|Components|CharacterMovement")
	void FinishUpJumping();


	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
