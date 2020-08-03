// Fill out your copyright notice in the Description page of Project Settings.


#include "PaperPixelCharacter.h"


#include "MathHelper.h"
#include "PaperFlipbookComponent.h"
#include "PaperPixelMovement.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/EngineTypes.h"
#include "PhysicsEngine/PhysicsSettings.h"


void APaperPixelCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APaperPixelCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Setup Collision Detection for coyote jump
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APaperPixelCharacter::OnCapsuleHit);
}

void APaperPixelCharacter::FlipSprite() const
{
	FVector Scale = GetSprite()->GetRelativeScale3D();
	FVector Loc = GetSprite()->GetRelativeLocation();
	{
		GetSprite()->SetRelativeScale3D(FVector(Scale.X * -1, Scale.Y, Scale.Z));
		GetSprite()->SetRelativeLocation(FVector(Loc.X * -1, Loc.Y, Loc.Z));
	}
}

void APaperPixelCharacter::FlipSpriteByInput(float AxisValue) const
{
	const bool bIsSpriteFlipped = IsSpriteFlipped();
	if(AxisValue > 0 && bIsSpriteFlipped || AxisValue < 0 && !bIsSpriteFlipped)
	{
		FlipSprite();
	}
}

bool APaperPixelCharacter::IsSpriteFlipped() const
{
	return GetSprite()->GetRelativeScale3D().X < 0;
}

void APaperPixelCharacter::OnCapsuleHit(UPrimitiveComponent* HitComponent,
                                        AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherComp->GetCollisionObjectType() == ECC_WorldStatic && UMathHelper::Angle(NormalImpulse, FVector::UpVector) <= PaperPixelMovement->GetWalkableFloorAngle())
	{
		PaperPixelMovement->TimeSinceFall = 0;
	}
}

bool APaperPixelCharacter::CanJumpInternal_Implementation() const
{
	// Ensure the character isn't currently crouched.
	bool bCanJump = !bIsCrouched;

	// Ensure that the CharacterMovement state is valid
	bCanJump &= PaperPixelMovement->CanAttemptJump();

	if (bCanJump)
	{
		// Ensure JumpHoldTime and JumpCount are valid.
		if (!bWasJumping || GetJumpMaxHoldTime() <= 0.0f)
		{
			
			if (JumpCurrentCount == 0 && PaperPixelMovement->IsFalling())
			{
				// Coyote Jump
				if(GetVelocity().Z <= 0 &&
                    PaperPixelMovement->TimeSinceFall <= PaperPixelMovement->CoyoteJumpTime)
				{
					bCanJump = true;
				}else
				{
					bCanJump = JumpCurrentCount + 1 < JumpMaxCount;
				}
			}
			else
			{
				bCanJump = JumpCurrentCount < JumpMaxCount;
			}
		}
		else
		{
			// Only consider JumpKeyHoldTime as long as:
			// A) The jump limit hasn't been met OR
			// B) The jump limit has been met AND we were already jumping
			const bool bJumpKeyHeld = (bPressedJump && JumpKeyHoldTime < GetJumpMaxHoldTime());
			bCanJump = bJumpKeyHeld &&
                        ((JumpCurrentCount < JumpMaxCount) || (bWasJumping && JumpCurrentCount == JumpMaxCount));
		
		}
	}

	return bCanJump;
}

void APaperPixelCharacter::CheckJumpInput(float DeltaTime)
{
	Super::CheckJumpInput(DeltaTime);

	if(PaperPixelMovement)
	{
		if(JumpCurrentCount == 0 && PaperPixelMovement->IsFalling() && GetVelocity().Z <= 0 &&
	                    PaperPixelMovement->TimeSinceFall <= PaperPixelMovement->CoyoteJumpTime)
		{
			// Decrement only on Coyote jump
			JumpCurrentCount -= 1;
		}
	}
}

APaperPixelCharacter::APaperPixelCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPaperPixelMovement>(CharacterMovementComponentName))
{
	GetCapsuleComponent()->SetCapsuleRadius(4);
	GetCapsuleComponent()->SetCapsuleHalfHeight(6);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	SideViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	SideViewCamera->SetOrthoWidth(480.0f);

	JumpMaxHoldTime = 0.180702;

	PaperPixelMovement = Cast<UPaperPixelMovement>(GetCharacterMovement());

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

UPaperFlipbook* APaperPixelCharacter::GetAnimationFlipbook(const FName Key) const
{
	if(AnimationFlipbooks.Contains(Key))
		return AnimationFlipbooks.FindRef(Key);

	return nullptr;
}

void APaperPixelCharacter::Jump()
{
	Super::Jump();
}

void APaperPixelCharacter::StopJumping()
{
	PaperPixelMovement->FinishUpJumping();
	Super::StopJumping();
}

