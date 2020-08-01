// Fill out your copyright notice in the Description page of Project Settings.


#include "Platformer2DMovement.h"



#include "PhysicsEngine/PhysicsSettings.h"
#include "Platformer2D/Pawn/PlatformerPlayer.h"

void UPlatformer2DMovement::CalculateAndPrintLogJumpingValue()
{
	// Still not working properly, need way to mark blueprint need to recompile!
	// When changing the variable value from c++, it will ignored!. Even after that you change from editor, it will always ignored!!
	// Dunno how to do this

	const float Gravity = UPhysicsSettings::Get()->DefaultGravityZ * GravityScale;
	UE_LOG(LogTemp, Warning, TEXT("RESULT JUMPING VALUE"));
	const float JumpZ = FMath::Sqrt(FMath::Abs(2 * Gravity * MaxJumpHeight));
	UE_LOG(LogTemp, Warning, TEXT("JumpZVelocity = %f"), JumpZ);
	UE_LOG(LogTemp, Warning, TEXT("JumpMaxHoldTime = %f"), FMath::Abs(JumpZ / Gravity));

}


bool UPlatformer2DMovement::CanCoyoteJump() const
{
	return PlayerOwner->JumpCurrentCount == 0 && IsFalling() && PlayerOwner->GetVelocity().Z <= 0 &&
                    TimeSinceFall <= CoyoteJumpTime;
}

void UPlatformer2DMovement::CalculateJumpVelocityClamp()
{
	// This calculation is alright.
	if(PlayerOwner)
	{
		const float Gravity = GetGravityZ();
		const float JumpZ = FMath::Sqrt(FMath::Abs(2 * Gravity * MaxJumpHeight));
		// const float Gravity = (2 * MaxJumpHeight / (CharacterOwner->JumpMaxHoldTime * CharacterOwner->JumpMaxHoldTime));
		JumpZVelocity = JumpZ;
		PlayerOwner->JumpMaxHoldTime = FMath::Abs(JumpZ / Gravity);
	}
}

UPlatformer2DMovement::UPlatformer2DMovement()
{
	JumpZVelocity = 177.087555;
	AirControl = 1.0;
	CoyoteJumpTime = 0.75;
	MaxStepHeight = 2;
	MaxWalkSpeed = 150;
	MaxWalkSpeedCrouched = 80;
}

float UPlatformer2DMovement::GetMinJumpHeight() const
{
	return MinJumpHeightPercentage * GetMaxJumpHeight();
}

void UPlatformer2DMovement::FinishUpJumping()
{
	if(CharacterOwner)
	{
		if(bJumpVelocityClamp)
		{
			const float TermVelocity = FMath::Square(JumpZVelocity*JumpZVelocity + 2 * GetGravityZ() * (GetMaxJumpHeight() - GetMinJumpHeight()));
			// const float TermTime = CharacterOwner->JumpMaxHoldTime - (2* (MaxJumpHeight - GetMinJumpHeight()) / (JumpZVelocity + TermVelocity));
			if(Velocity.Z > TermVelocity)
				Velocity.Z = TermVelocity;
		}
	}
}

void UPlatformer2DMovement::BeginPlay()
{
	Super::BeginPlay();

	if(bJumpVelocityClamp)
	{
		CalculateJumpVelocityClamp();
	}
}

void UPlatformer2DMovement::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(IsFalling())
	{
		TimeSinceFall += DeltaTime;
	}
}

void UPlatformer2DMovement::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
	Super::SetUpdatedComponent(NewUpdatedComponent);

	PlayerOwner = Cast<APlatformerPlayer>(PawnOwner);
}
