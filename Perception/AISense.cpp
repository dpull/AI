#include "AISense.h"
#include <algorithm>
#include <cassert>

enum
{
	INDEX_NONE = -1
};

namespace FAISystem
{
FVector FindClosestLocation(const FVector& Origin, const std::vector<FVector>& Locations)
{
	FVector BestLocation = FAISystem::InvalidLocation;
	float BestDistance = FLT_MAX;

	for (const FVector& Candidate : Locations)
	{
		const float CurrentDistanceSq = FVector::DistSquared(Origin, Candidate);
		if (CurrentDistanceSq < BestDistance)
		{
			BestDistance = CurrentDistanceSq;
			BestLocation = Candidate;
		}
	}

	return BestLocation;
}

bool CheckIsTargetInSightCone(const FVector& StartLocation, const FVector& ConeDirectionNormal, float PeripheralVisionAngleCos, float ConeDirectionBackwardOffset,
							  float NearClippingRadiusSq, float const FarClippingRadiusSq, const FVector& TargetLocation)
{
	const FVector BaseLocation = FMath::IsNearlyZero(ConeDirectionBackwardOffset) ? StartLocation : StartLocation - ConeDirectionNormal * ConeDirectionBackwardOffset;
	const FVector ActorToTarget = TargetLocation - BaseLocation;
	const float DistToTargetSq = ActorToTarget.SizeSquared();
	if (DistToTargetSq <= FarClippingRadiusSq && DistToTargetSq >= NearClippingRadiusSq)
	{
		// Will return true if squared distance to Target is smaller than SMALL_NUMBER
		if (DistToTargetSq < SMALL_NUMBER)
		{
			return true;
		}

		// Calculate the normal here instead of calling GetUnsafeNormal as we already have the DistToTargetSq (optim)
		const FVector DirectionToTargetNormal = ActorToTarget * FMath::InvSqrt(DistToTargetSq);

		return FVector::DotProduct(DirectionToTargetNormal, ConeDirectionNormal) > PeripheralVisionAngleCos;
	}

	return false;
}
} // namespace FAISystem

float UAISense_Sight::Update()
{
	// sort Sight Queries
	{
		auto RecalcScore = [](FAISightQuery& SightQuery) { SightQuery.RecalcScore(); };

		// Sort out of range queries
		if (bSightQueriesOutOfRangeDirty)
		{
			std::for_each(SightQueriesOutOfRange.begin(), SightQueriesOutOfRange.end(), RecalcScore);
			std::sort(SightQueriesOutOfRange.begin(), SightQueriesOutOfRange.end(), [](FAISightQuery& l, FAISightQuery& r) { return -1; });
			NextOutOfRangeIndex = 0;
			bSightQueriesOutOfRangeDirty = false;
		}

		// Sort in range queries
		std::for_each(SightQueriesInRange.begin(), SightQueriesInRange.end(), RecalcScore);
		std::sort(SightQueriesInRange.begin(), SightQueriesInRange.end(), [](FAISightQuery& l, FAISightQuery& r) { return -1; });
	}

	int32_t TracesCount = 0;
	int32_t NumQueriesProcessed = 0;
	double TimeSliceEnd = /* FPlatformTime::Seconds() */ +MaxTimeSlicePerTick;
	bool bHitTimeSliceLimit = false;
	static const int32_t InitialInvalidItemsSize = 16;
	enum class EOperationType : uint8_t
	{
		Remove,
		SwapList
	};
	struct FQueryOperation
	{
		FQueryOperation(bool bInInRange, EOperationType InOpType, int32_t InIndex) : bInRange(bInInRange), OpType(InOpType), Index(InIndex)
		{
		}
		bool bInRange;
		EOperationType OpType;
		int32_t Index;
	};
	std::vector<FQueryOperation> QueryOperations;
	std::vector<FAISightTarget::FTargetId> InvalidTargets;
	QueryOperations.reserve(InitialInvalidItemsSize);
	InvalidTargets.reserve(InitialInvalidItemsSize);

	AIPerception::FListenerMap& ListenersMap = *GetListeners();
	// return SightQueries.Num() > 0 ? 1.f/6 : FLT_MAX;
	return 0.f;
}

bool UAISense_Sight::ComputeVisibility(const UWorld* World, FAISightQuery& SightQuery, FPerceptionListener& Listener, const AActor* ListenerActor, FAISightTarget& Target,
									   AActor* TargetActor, const FDigestedSightProperties& PropDigest, float& OutStimulusStrength, FVector& OutSeenLocation,
									   int32_t& OutNumberOfLoSChecksPerformed) const
{
	return false;
}

bool UAISense_Sight::ShouldAutomaticallySeeTarget(const FDigestedSightProperties& PropDigest, FAISightQuery* SightQuery, FPerceptionListener& Listener, AActor* TargetActor,
												  float& OutStimulusStrength) const
{
	OutStimulusStrength = 1.0f;

	if ((PropDigest.AutoSuccessRangeSqFromLastSeenLocation != FAISystem::InvalidRange) && (SightQuery->LastSeenLocation != FAISystem::InvalidLocation))
	{
		const float DistanceToLastSeenLocationSq = 0; // FVector::DistSquared(TargetActor->GetActorLocation(), SightQuery->LastSeenLocation);
		return (DistanceToLastSeenLocationSq <= PropDigest.AutoSuccessRangeSqFromLastSeenLocation);
	}

	return false;
}
