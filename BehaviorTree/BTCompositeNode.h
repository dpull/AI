#pragma once

#include <vector>

enum class EBTDecoratorLogic
{
	Invalid,
	/** Test decorator conditions. */
	Test,
	/** logic op: AND */
	And,
	/** logic op: OR */
	Or,
	/** logic op: NOT */
	Not,
};

struct FBTDecoratorLogic
{
	EBTDecoratorLogic Operation;
	uint16_t Number;
};
