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

	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation, meta = (AllowPrivateAccess = "true"))
	TMap<FName, class UPaperFlipbook*> AnimationFlipbooks;
	
public:
	APlatformerPlayer();

	UFUNCTION(BlueprintCallable, Category = Animation)
	UPaperFlipbook* GetAnimationFlipbook(const FName Key) const;
};
