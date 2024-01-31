#include "BlackboardData.h"
#include "BlackboardComponent.h"

void UBlackboardKeyType::PreInitialize(UBlackboardComponent& OwnerComp)
{
}

void UBlackboardKeyType::InitializeKey(UBlackboardComponent& OwnerComp, FBlackboard::FKey KeyID)
{
}

uint16_t UBlackboardKeyType::GetValueSize() const
{
	return 0;
}

void UBlackboardKeyType::WrappedFree(UBlackboardComponent& OwnerComp, uint8_t* MemoryBlock)
{
}

UBlackboardKeyType* UBlackboardKeyType::GetKeyInstance(UBlackboardComponent& OwnerComp, const uint8_t* MemoryBlock) const
{
	return NULL;
}

bool UBlackboardKeyType::HasInstance() const
{
	return bCreateKeyInstance;
}

int32_t UBlackboardData::GetNumKeys() const
{
	return 0;
}

void UBlackboardData::UpdateKeyIDs()
{
}

const FBlackboardEntry* UBlackboardData::GetKey(FBlackboard::FKey KeyID) const
{
	return nullptr;
}
