#include "BlackboardComponent.h"
#include "AISystem.h"
#include "BlackboardData.h"
#include <algorithm>
#include <cassert>

bool UBlackboardComponent::InitializeBlackboard(UBlackboardData& NewAsset)
{
	if (&NewAsset == BlackboardAsset)
	{
		return true;
	}

	UAISystem* AISystem = UAISystem::GetCurrentSafe();
	if (AISystem == nullptr)
	{
		return false;
	}

	if (BlackboardAsset && BlackboardAsset->HasSynchronizedKeys())
	{
		AISystem->UnregisterBlackboardComponent(*BlackboardAsset, *this);
		DestroyValues();
	}

	BlackboardAsset = &NewAsset;
	ValueMemory.clear();
	ValueOffsets.clear();
	bSynchronizedKeyPopulated = false;

	bool bSuccess = true;

	if (BlackboardAsset)
	{
		InitializeParentChain(BlackboardAsset);

		std::vector<FBlackboardInitializationData> InitList;
		const int32_t NumKeys = BlackboardAsset->GetNumKeys();
		InitList.reserve(NumKeys);
		ValueOffsets.resize(NumKeys);

		for (UBlackboardData* It = BlackboardAsset; It; It = It->Parent)
		{
			for (int32_t KeyIndex = 0; KeyIndex < It->Keys.size(); KeyIndex++)
			{
				UBlackboardKeyType* KeyType = It->Keys[KeyIndex].KeyType;
				if (KeyType)
				{
					KeyType->PreInitialize(*this);

					const uint16_t KeyMemory = KeyType->GetValueSize() + (KeyType->HasInstance() ? sizeof(FBlackboardInstancedKeyMemory) : 0);
					InitList.emplace_back(FBlackboardInitializationData(KeyIndex + It->GetFirstKeyID(), KeyMemory));
				}
			}
		}

		// sort key values by memory size, so they can be packed better
		// it still won't protect against structures, that are internally misaligned (-> uint8, uint32)
		// but since all Engine level keys are good...
		std::sort(InitList.begin(), InitList.end(), FBlackboardInitializationData::FMemorySort());
		uint16_t MemoryOffset = 0;
		for (int32_t Index = 0; Index < InitList.size(); Index++)
		{
			ValueOffsets[InitList[Index].KeyID] = MemoryOffset;
			MemoryOffset += InitList[Index].DataSize;
		}

		ValueMemory.resize(ValueMemory.size() + MemoryOffset);

		// initialize memory
		KeyInstances.resize(KeyInstances.size() + InitList.size());

		for (int32_t Index = 0; Index < InitList.size(); Index++)
		{
			const FBlackboardEntry* KeyData = BlackboardAsset->GetKey(InitList[Index].KeyID);
			KeyData->KeyType->InitializeKey(*this, InitList[Index].KeyID);
		}

		// naive initial synchronization with one of already instantiated blackboards using the same BB asset
		if (BlackboardAsset->HasSynchronizedKeys())
		{
			PopulateSynchronizedKeys();
		}
	}
	else
	{
		bSuccess = false;
	}

	return bSuccess;
}

void UBlackboardComponent::DestroyValues()
{
	for (UBlackboardData* It = BlackboardAsset; It; It = It->Parent)
	{
		for (int32_t KeyIndex = 0; KeyIndex < It->Keys.size(); KeyIndex++)
		{
			UBlackboardKeyType* KeyType = It->Keys[KeyIndex].KeyType;
			if (KeyType)
			{
				const int32_t UseIdx = KeyIndex + It->GetFirstKeyID();
				uint8_t* KeyMemory = GetKeyRawData(UseIdx);
				KeyType->WrappedFree(*this, KeyMemory);
			}
		}
	}

	ValueOffsets.clear();
	ValueMemory.clear();
}

uint8_t* UBlackboardComponent::GetKeyRawData(FBlackboard::FKey KeyID)
{
	return KeyID >= 0 && KeyID < ValueMemory.size() ? (ValueMemory.data() + ValueOffsets[KeyID]) : NULL;
}

void UBlackboardComponent::InitializeParentChain(UBlackboardData* NewAsset)
{
	if (NewAsset)
	{
		InitializeParentChain(NewAsset->Parent);
		NewAsset->UpdateKeyIDs();
	}
}

void UBlackboardComponent::PopulateSynchronizedKeys()
{
	assert(bSynchronizedKeyPopulated == false);
	bSynchronizedKeyPopulated = true;
}

bool UBlackboardComponent::ShouldSyncWithBlackboard(UBlackboardComponent& OtherBlackboardComponent) const
{
	return true;
}
