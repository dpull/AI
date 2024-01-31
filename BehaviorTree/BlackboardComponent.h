#pragma once

#include "BehaviorTree.h"
#include "BehaviorTreeComponent.h"
#include "BlackboardData.h"
#include <map>
#include <vector>

class UBlackboardKeyType;

struct FOnBlackboardChangeNotificationInfo
{
};

struct FBlackboardInitializationData
{
	FBlackboard::FKey KeyID;
	uint16_t DataSize;

	FBlackboardInitializationData()
	{
	}
	FBlackboardInitializationData(FBlackboard::FKey InKeyID, uint16_t InDataSize) : KeyID(InKeyID)
	{
		DataSize = (InDataSize <= 2) ? InDataSize : ((InDataSize + 3) & ~3);
	}
	struct FMemorySort
	{
		bool operator()(const FBlackboardInitializationData& A, const FBlackboardInitializationData& B) const
		{
			return A.DataSize > B.DataSize;
		}
	};
};

class UBlackboardComponent
{
  public:
	bool InitializeBlackboard(UBlackboardData& NewAsset);
	void DestroyValues();
	uint8_t* GetKeyRawData(FBlackboard::FKey KeyID);

	void InitializeParentChain(UBlackboardData* NewAsset);
	void PopulateSynchronizedKeys();
	bool ShouldSyncWithBlackboard(UBlackboardComponent& OtherBlackboardComponent) const;

	UBrainComponent* BlackboardComp;
	UBlackboardData* DefaultBlackboardAsset;
	UBlackboardData* BlackboardAsset;
	std::vector<uint8_t> ValueMemory;
	std::vector<uint16_t> ValueOffsets;
	std::vector<UBlackboardKeyType*> KeyInstances;
	int32_t NotifyObserversRecursionCount = 0;
	int32_t ObserversToRemoveCount = 0;
	std::multimap<uint8_t, FOnBlackboardChangeNotificationInfo> Observers;
	std::vector<uint8_t> QueuedUpdates;
	uint32_t bPausedNotifies : 1;
	uint32_t bSynchronizedKeyPopulated : 1;
};
