#pragma once

#include <string>
#include <vector>

class UBlackboardComponent;

namespace FBlackboard
{
constexpr char* KeySelf = "SelfActor";

typedef uint8_t FKey;

constexpr FKey InvalidKey = FKey(-1);
} // namespace FBlackboard

enum class EBlackboardKeyOperation
{
	Basic,
	Arithmetic,
	Text,
};

struct FBlackboardInstancedKeyMemory
{
	/** index of instanced key in UBlackboardComponent::InstancedKeys */
	int32_t KeyIdx;
};

class UBlackboardKeyType
{

	uint16_t ValueSize;
	EBlackboardKeyOperation SupportedOp;
	uint8_t bIsInstanced : 1;
	uint8_t bCreateKeyInstance : 1;

  public:
	virtual void PreInitialize(UBlackboardComponent& OwnerComp);
	/** handle instancing if needed */
	void InitializeKey(UBlackboardComponent& OwnerComp, FBlackboard::FKey KeyID);

	uint16_t GetValueSize() const;

	void WrappedFree(UBlackboardComponent& OwnerComp, uint8_t* MemoryBlock);
	UBlackboardKeyType* GetKeyInstance(UBlackboardComponent& OwnerComp, const uint8_t* MemoryBlock) const;
	bool HasInstance() const;
};

struct FBlackboardEntry
{
	std::string EntryName;
	UBlackboardKeyType* KeyType;

	uint32_t bInstanceSynced : 1;
};

class UBlackboardData
{
  public:
	bool HasSynchronizedKeys() const
	{
		return bHasSynchronizedKeys;
	}

	FBlackboard::FKey GetFirstKeyID() const
	{
		return FirstKeyID;
	}

	int32_t GetNumKeys() const;
	void UpdateKeyIDs();
	const FBlackboardEntry* GetKey(FBlackboard::FKey KeyID) const;

	UBlackboardData* Parent;
	std::vector<FBlackboardEntry> Keys;

	uint32_t bHasSynchronizedKeys : 1;

	FBlackboard::FKey FirstKeyID;
};