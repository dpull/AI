#pragma once

#include "BTAuxiliaryNode.h"
#include <vector>

enum class EBTFlowAbortMode
{
	None,
	LowerPriority,
	Self,
	Both,
};

class UBTDecorator : public UBTAuxiliaryNode
{
	/** if set, FlowAbortMode can be set to None */
	uint32_t bAllowAbortNone : 1;

	/** if set, FlowAbortMode can be set to LowerPriority and Both */
	uint32_t bAllowAbortLowerPri : 1;

	/** if set, FlowAbortMode can be set to Self and Both */
	uint32_t bAllowAbortChildNodes : 1;

	/** if set, OnNodeActivation will be used */
	uint32_t bNotifyActivation : 1;

	/** if set, OnNodeDeactivation will be used */
	uint32_t bNotifyDeactivation : 1;

	/** if set, OnNodeProcessed will be used */
	uint32_t bNotifyProcessed : 1;

	/** if set, static description will include default description of inversed condition */
	uint32_t bShowInverseConditionDesc : 1;

  private:
	/** if set, condition check result will be inversed */
	uint32_t bInverseCondition : 1;

	/** flow controller settings */
	EBTFlowAbortMode FlowAbortMode;
};