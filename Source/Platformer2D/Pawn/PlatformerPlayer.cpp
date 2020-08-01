// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerPlayer.h"


#include "MathHelper.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/EngineTypes.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "Platformer2D/GameFramework/Platformer2DMovement.h"


void APlatformerPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APlatformerPlayer::BeginPlay()
{
	Super::BeginPlay();
	WalkDirection.Normalize(SMALL_NUMBER);
	// Setup Collision Detection for coyote jump
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APlatformerPlayer::OnCapsuleHit);
}

void APlatformerPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveRight", this, &APlatformerPlayer::MoveHorizontal);
}

void APlatformerPlayer::MoveHorizontal(float AxisValue)
{
	AddMovementInput(WalkDirection, AxisValue);

	FVector Scale = GetSprite()->GetRelativeScale3D();
	FVector Loc = GetSprite()->GetRelativeLocation();
	if(AxisValue < 0 && Scale.X > 0 || AxisValue > 0 && Scale.X < 0)
	{
		GetSprite()->SetRelativeScale3D(FVector(Scale.X * -1, Scale.Y, Scale.Z));
		GetSprite()->SetRelativeLocation(FVector(Loc.X * -1, Loc.Y, Loc.Z));
	}
}

void APlatformerPlayer::OnCapsuleHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherComp->GetCollisionObjectType() == ECC_WorldStatic && UMathHelper::Angle(NormalImpulse, FVector::UpVector) <= Platformer2DMovement->GetWalkableFloorAngle())
	{
		Platformer2DMovement->TimeSinceFall = 0;
	}
}

bool APlatformerPlayer::CanJumpInternal_Implementation() const
{
	// Ensure the character isn't currently crouched.
	bool bCanJump = !bIsCrouched;

	// Ensure that the CharacterMovement state is valid
	bCanJump &= Platformer2DMovement->CanAttemptJump();

	if (bCanJump)
	{
		// Ensure JumpHoldTime and JumpCount are valid.
		if (!bWasJumping || GetJumpMaxHoldTime() <= 0.0f)
		{
			
			if (JumpCurrentCount == 0 && Platformer2DMovement->IsFalling())
			{
				// Coyote Jump
				if(GetVelocity().Z <= 0 &&
                    Platformer2DMovement->TimeSinceFall <= Platformer2DMovement->CoyoteJumpTime)
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

void APlatformerPlayer::CheckJumpInput(float DeltaTime)
{
	Super::CheckJumpInput(DeltaTime);
	if(JumpCurrentCount == 0 && Platformer2DMovement->IsFalling() && GetVelocity().Z <= 0 &&
                    Platformer2DMovement->TimeSinceFall <= Platformer2DMovement->CoyoteJumpTime)
	{
		// Decrement only on Coyote jump
		JumpCurrentCount -= 1;
	}
}

APlatformerPlayer::APlatformerPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPlatformer2DMovement>(CharacterMovementComponentName))
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

	Platformer2DMovement = Cast<UPlatformer2DMovement>(GetCharacterMovement());

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

UPaperFlipbook* APlatformerPlayer::GetAnimationFlipbook(const FName Key) const
{
	if(AnimationFlipbooks.Contains(Key))
		return AnimationFlipbooks.FindRef(Key);

	return nullptr;
}

void APlatformerPlayer::Jump()
{
	Super::Jump();
}

void APlatformerPlayer::StopJumping()
{
	Platformer2DMovement->FinishUpJumping();
	Super::StopJumping();
}

