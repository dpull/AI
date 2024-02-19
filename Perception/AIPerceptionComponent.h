#pragma once
#include "AIPerceptionSystem.h"
#include "AISense.h"

#include <map>
#include <string>
#include <vector>

class UAISenseConfig
{
	/** specifies age limit after stimuli generated by this sense become forgotten. 0 means "never" */
	float MaxAge;

	/** determines whether given sense starts in an enabled state */
	uint32_t bStartsEnabled : 1;

	// name of sense
	mutable std::string CachedSenseName;
};

struct FPerceptionChannelAllowList
{
	typedef int32_t FFlagsContainer;

	FFlagsContainer AcceptedChannelsMask;

	void Clear()
	{
		AcceptedChannelsMask = 0;
	}
};

struct FAIStimulus
{
};

class UAIPerceptionComponent
{
	UAISense* DominantSense;
	void* AIOwner; // AAIController
	FPerceptionChannelAllowList PerceptionFilter;

	struct FStimulusToProcess
	{
		void* Source; // AActor
		FAIStimulus Stimulus;
	};

	std::vector<FStimulusToProcess> StimuliToProcess;

	/** max age of stimulus to consider it "active" (e.g. target is visible) */
	std::vector<float> MaxActiveAge;

	/** Determines whether all knowledge of previously sensed actors will be removed or not when they become stale.
		That is, when they are no longer perceived and have exceeded the max age of the sense. */
	uint32_t bForgetStaleActors : 1;

	uint32_t bCleanedUp : 1;

	std::vector<UAISenseConfig*> SensesConfig;

	FPerceptionListenerID PerceptionListenerId;

	virtual void OnRegister();

	void RegisterSenseConfig(UAISenseConfig& SenseConfig, UAIPerceptionSystem& AIPerceptionSys);

	FPerceptionListenerID GetListenerId() const
	{
		return PerceptionListenerId;
	}
};