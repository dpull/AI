#include "AIPerceptionComponent.h"
#include "AIPerceptionSystem.h"
#include <cassert>

void UAIPerceptionComponent::OnRegister()
{

	bCleanedUp = false;

	UAIPerceptionSystem* AIPerceptionSys = UAIPerceptionSystem::GetCurrent();
	if (AIPerceptionSys != nullptr)
	{
		PerceptionFilter.Clear();

		if (SensesConfig.size() > 0)
		{
			// set up perception listener based on SensesConfig
			for (auto SenseConfig : SensesConfig)
			{
				if (SenseConfig)
				{
					RegisterSenseConfig(*SenseConfig, *AIPerceptionSys);
				}
			}

			AIPerceptionSys->UpdateListener(*this);
		}
	}

	// this should not be needed but aparently AAIController::PostRegisterAllComponents
	// gets called component's OnRegister
	// AAIController::SetPerceptionComponent(*this);
}

void UAIPerceptionComponent::RegisterSenseConfig(UAISenseConfig& SenseConfig, UAIPerceptionSystem& AIPerceptionSys)
{
	// 1. 获取/创建 UAISense 实例
	// 2. PerceptionFilter.AcceptChannel(SenseID)
	// 3. SetMaxStimulusAge(SenseID, SenseConfig.GetMaxAge());
}
