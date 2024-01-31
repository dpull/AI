#include "AISystem.h"
#include "BlackboardData.h"
#include <cassert>

template <typename K, typename V> void RemoveSingle(std::multimap<K, V>& myMap, const K& key, const V& value)
{
	auto range = myMap.equal_range(key);
	for (auto it = range.first; it != range.second; ++it)
	{
		if (it->second == value)
		{
			myMap.erase(it);
			break;
		}
	}
}

template <typename K, typename V> bool FindPair(std::multimap<K, V>& myMap, const K& key, const V& value)
{
	auto range = myMap.equal_range(key);
	for (auto it = range.first; it != range.second; ++it)
	{
		if (it->second == value)
		{
			myMap.erase(it);
			return true;
		}
	}
	return false;
}

UAISystem* UAISystem::GetCurrentSafe()
{
	static UAISystem Inst;
	return &Inst;
}

UAISystem::FBlackboardDataToComponentsIterator UAISystem::CreateBlackboardDataToComponentsIterator(UBlackboardData& BlackboardAsset)
{
	return UAISystem::FBlackboardDataToComponentsIterator();
}

void UAISystem::RegisterBlackboardComponent(UBlackboardData& BlackboardData, UBlackboardComponent& BlackboardComp)
{
	// mismatch of register/unregister.
	assert(FindPair(BlackboardDataToComponentsMap, &BlackboardData, &BlackboardComp));

	BlackboardDataToComponentsMap.insert(std::make_pair(&BlackboardData, &BlackboardComp));
	if (BlackboardData.Parent)
	{
		RegisterBlackboardComponent(*BlackboardData.Parent, BlackboardComp);
	}
}

void UAISystem::UnregisterBlackboardComponent(UBlackboardData& BlackboardData, UBlackboardComponent& BlackboardComp)
{
	if (BlackboardData.Parent)
	{
		UnregisterBlackboardComponent(*BlackboardData.Parent, BlackboardComp);
	}
	RemoveSingle(BlackboardDataToComponentsMap, &BlackboardData, &BlackboardComp);
}
