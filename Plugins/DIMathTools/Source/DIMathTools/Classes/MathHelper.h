// Copyright 2019 RUS Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathHelper.generated.h"

/**
 * 
 */
UCLASS()
class DIMATHTOOLS_API UMathHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/* Get angle from 2 direction vector
	* @params V1 Vector direction 1
	* @params V2 Vector direction 2
	* @return Angle between the 2 direction vector
	*/
	UFUNCTION(BlueprintPure, Category = "Math|Vector", meta = (BlueprintThreadSafe))
	static float Angle(FVector V1, FVector V2);
	UFUNCTION(BlueprintPure, Category = "Math|Vector", meta = (BlueprintThreadSafe))
	static float SignedAngle(FVector V1, FVector V2);
	UFUNCTION(BlueprintPure, Category = "Math|Float", meta = (BlueprintThreadSafe))
	static float RoundStep(const float& Value, float Step = 0.33f, float Offset = 0.f);
	/** Round Step with negative value.  */
	UFUNCTION(BlueprintPure, Category = "Math|Float", meta = (BlueprintThreadSafe))
	static float RoundStepPosNeg(const float& Value, float Step = 0.33f, float Offset = 0.f);
	UFUNCTION(BlueprintPure, Category = "Math|Float", meta = (BlueprintThreadSafe))
	static float Remap(const float& Value, float OldMin = 0, float OldMax = 1, float NewMin = 0, float NewMax = 1);
	UFUNCTION(BlueprintPure, Category = "Math|Bool", meta = (BlueprintThreadSafe))
	static int BoolToInt(bool b);
};
