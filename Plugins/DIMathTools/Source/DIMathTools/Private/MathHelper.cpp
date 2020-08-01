// Copyright 2019 RUS Studio. All Rights Reserved.


#include "MathHelper.h"

float UMathHelper::Angle(FVector V1, FVector V2) {
	return FMath::Acos(Dot3(V1, V2));
}

float UMathHelper::SignedAngle(FVector V1, FVector V2) {
	float fAngle = Angle(V1, V2);
	FVector Cross = FVector::CrossProduct(V1, V2);
	if (Dot3(FVector(0.f, 0.f, 1), Cross) < 0)
		fAngle *= -1;
	return fAngle;
}

float UMathHelper::RoundStep(const float& Value, float Step, float Offset) {
	return FMath::CeilToFloat((Value - Offset) / Step) * Step + Offset;
}

float UMathHelper::RoundStepPosNeg(const float& Value, float Step, float Offset) {
	int Direction = FMath::Sign((Value - Offset) / Step);
	return FMath::CeilToFloat(FMath::Abs((Value - Offset) / Step)) * Direction * Step + Offset;
}

float UMathHelper::Remap(const float& Value, float OldMin, float OldMax, float NewMin, float NewMax) {
	return NewMin + (Value - OldMin) * (NewMax - NewMin) / (OldMax - OldMin);
}

int UMathHelper::BoolToInt(bool b) {
	return b ? 1 : 0;
}
