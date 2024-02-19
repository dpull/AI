#pragma once
#include "AISense.h"

#include <map>
#include <string>
#include <vector>

struct FAISenseID
{
};

class UAIPerceptionComponent;

class UAIPerceptionSystem
{
	std::vector<UAISense*> Senses;
	float PerceptionAgingRate;
	uint32_t bHandlePawnNotification : 1;
	float NextStimuliAgingTick;
	float CurrentTime;

  public:
	virtual void Tick(float DeltaTime);

	static UAIPerceptionSystem* GetCurrent();
	FAISenseID RegisterSenseClass(UAISense* SenseClass);
	void UpdateListener(UAIPerceptionComponent& Listener);
	void PerformSourceRegistration();
};