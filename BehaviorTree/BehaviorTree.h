#pragma once

#include <vector>

class UBTCompositeNode;
class UBlackboardData;
class UBTDecorator;
class FBTDecoratorLogic;

class UBehaviorTree
{
	UBTCompositeNode* RootNode;
	UBlackboardData* BlackboardAsset;
	std::vector<UBTDecorator*> RootDecorators;
	std::vector<FBTDecoratorLogic> RootDecoratorOps;
	uint16_t InstanceMemorySize;
};