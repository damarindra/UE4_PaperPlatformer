// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerPlayer.h"

#include "Components/CapsuleComponent.h"


void APlatformerPlayer::Tick(float DeltaSeconds)
{
}

APlatformerPlayer::APlatformerPlayer()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(12);
	GetCapsuleComponent()->SetCapsuleRadius(4);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	
}

UPaperFlipbook* APlatformerPlayer::GetAnimationFlipbook(const FName Key) const
{
	if(AnimationFlipbooks.Contains(Key))
		return AnimationFlipbooks.FindRef(Key);

	return nullptr;
};
