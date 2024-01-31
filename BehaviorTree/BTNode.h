#pragma once

#include <vector>

class UBehaviorTree;
class UBTCompositeNode;

class UBTNode
{
	UBehaviorTree* TreeAsset;
	UBTCompositeNode* ParentNode;
	uint16_t ExecutionIndex;
	uint16_t MemoryOffset;
	uint8_t TreeDepth;
	uint8_t bIsInstanced : 1;
	uint8_t bIsInjected : 1;
	uint8_t bCreateNodeInstance : 1;
	uint8_t bOwnsGameplayTasks : 1;
};