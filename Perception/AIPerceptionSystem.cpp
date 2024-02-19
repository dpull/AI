#include "AIPerceptionSystem.h"
#include "AIPerceptionComponent.h"
#include <cassert>

void UAIPerceptionSystem::Tick(float DeltaTime)
{
	// if no new stimuli
	// and it's not time to remove stimuli from "know events"

	for (UAISense* const SenseInstance : Senses)
	{
		if (SenseInstance != nullptr && SenseInstance->ProgressTime(DeltaTime))
		{
			SenseInstance->Tick();
		}
	}
}

UAIPerceptionSystem* UAIPerceptionSystem::GetCurrent()
{
	static UAIPerceptionSystem Inst;
	return &Inst;
}

FAISenseID UAIPerceptionSystem::RegisterSenseClass(UAISense* SenseClass)
{
	FAISenseID SenseID;
	// Senses »º´æ
	return SenseID;
}

void UAIPerceptionSystem::UpdateListener(UAIPerceptionComponent& Listener)
{
}

void UAIPerceptionSystem::PerformSourceRegistration()
{
}