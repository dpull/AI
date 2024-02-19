#pragma once

#include <cmath>
#include <map>
#include <vector>

#define UE_SMALL_NUMBER (1.e-8f)
#define SMALL_NUMBER 1e-8

typedef uint32_t FPerceptionListenerID;
struct FPerceptionListener
{
};

namespace AIPerception
{
typedef std::map<FPerceptionListenerID, FPerceptionListener> FListenerMap;
}

class UAISense
{
	float TimeUntilNextUpdate;

  public:
	bool ProgressTime(float DeltaSeconds)
	{
		TimeUntilNextUpdate -= DeltaSeconds;
		return TimeUntilNextUpdate <= 0.f;
	}

	void Tick()
	{
		if (TimeUntilNextUpdate <= 0.f)
		{
			TimeUntilNextUpdate = Update();
		}
	}

	virtual float Update()
	{
		return FLT_MAX;
	}

	AIPerception::FListenerMap* UAISense::GetListeners()
	{
		return nullptr;
	}
};

struct FVector
{
	FVector operator-(const FVector&) const
	{
		return FVector();
	}

	FVector operator*(const float&) const
	{
		return FVector();
	}

	bool operator!=(const FVector&) const
	{
		return false;
	}

	static float DistSquared(const FVector& V1, const FVector& V2)
	{
		return 0;
	}

	float SizeSquared() const
	{
		return 0;
	}

	static float DotProduct(const FVector& A, const FVector& B)
	{
		return 0;
	}
};

struct FRotator
{
};

enum ECollisionChannel
{
};

struct FMath
{
	static bool IsNearlyZero(float Value, float ErrorTolerance = UE_SMALL_NUMBER)
	{
		return true;
	}

	static inline float InvSqrt(float x)
	{
		return 1.0f / std::sqrtf(x);
	}
};

namespace FAISystem
{
static const FRotator InvalidRotation;
static const FVector InvalidLocation;
static const float InvalidRange = -1.f;
} // namespace FAISystem

class UWorld;
class AActor;

class UAISense_Sight : public UAISense
{
	struct FAISightTarget
	{
		typedef uint32_t FTargetId;
		static const FTargetId InvalidTargetId;

		void* Target;
		void* SightTargetInterface;
		uint8_t TeamId;
		FTargetId TargetId;

		FVector GetLocationSimple() const
		{
			// const AActor* TargetPtr = Target.Get();
			// return TargetPtr ? TargetPtr->GetActorLocation() : FVector::ZeroVector;
			return FVector();
		}

		const void* GetTargetActor() const
		{
			return Target;
		}
	};

	struct FDigestedSightProperties
	{
		float PeripheralVisionAngleCos;
		float SightRadiusSq;
		float AutoSuccessRangeSqFromLastSeenLocation;
		float LoseSightRadiusSq;
		float PointOfViewBackwardOffset;
		float NearClippingRadiusSq;
		uint8_t AffiliationFlags;
	};

	struct FAISightQuery
	{
		void RecalcScore()
		{
		}
		FPerceptionListenerID ObserverId;
		FAISightTarget::FTargetId TargetId;
		FVector LastSeenLocation;
	};

	typedef std::map<FAISightTarget::FTargetId, FAISightTarget> FTargetsContainer;
	FTargetsContainer ObservedTargets;
	std::map<FPerceptionListenerID, FDigestedSightProperties> DigestedProperties;

	/** The SightQueries are a n^2 problem and to reduce the sort time, they are now split between in range and out of range */
	/** Since the out of range queries only age as the distance component of the score is always 0, there is few need to sort them */
	/** In the majority of the cases most of the queries are out of range, so the sort time is greatly reduced as we only sort the in range queries */
	int32_t NextOutOfRangeIndex = 0;
	bool bSightQueriesOutOfRangeDirty = true;
	std::vector<FAISightQuery> SightQueriesOutOfRange;
	std::vector<FAISightQuery> SightQueriesInRange;

	int32_t MaxTracesPerTick;
	int32_t MinQueriesPerTimeSliceCheck;
	double MaxTimeSlicePerTick;
	float HighImportanceQueryDistanceThreshold;
	float HighImportanceDistanceSquare;
	float MaxQueryImportance;
	float SightLimitQueryImportance;

	ECollisionChannel DefaultSightCollisionChannel;

	float Update();
	bool ComputeVisibility(const UWorld* World, FAISightQuery& SightQuery, FPerceptionListener& Listener, const AActor* ListenerActor, FAISightTarget& Target, AActor* TargetActor,
						   const FDigestedSightProperties& PropDigest, float& OutStimulusStrength, FVector& OutSeenLocation, int32_t& OutNumberOfLoSChecksPerformed) const;
	virtual bool ShouldAutomaticallySeeTarget(const FDigestedSightProperties& PropDigest, FAISightQuery* SightQuery, FPerceptionListener& Listener, AActor* TargetActor,
											  float& OutStimulusStrength) const;
};