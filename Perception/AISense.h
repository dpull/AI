#pragma once

#include <map>
#include <vector>

class UAISystemBase
{
};

class UAISystem : public UAISystemBase
{
  public:
	static UAISystem* GetCurrentSafe();

	struct FBlackboardDataToComponentsIterator
	{
	};
	FBlackboardDataToComponentsIterator CreateBlackboardDataToComponentsIterator(class UBlackboardData& BlackboardAsset);

	void RegisterBlackboardComponent(class UBlackboardData& BlackboardAsset, class UBlackboardComponent& BlackboardComp);
	void UnregisterBlackboardComponent(class UBlackboardData& BlackboardAsset, class UBlackboardComponent& BlackboardComp);

	typedef std::multimap<UBlackboardData*, UBlackboardComponent*> FBlackboardDataToComponentsMap;
	FBlackboardDataToComponentsMap BlackboardDataToComponentsMap;
};