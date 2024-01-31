#pragma once

#include "BTNode.h"
#include <vector>

class UBTAuxiliaryNode : public UBTNode
{
	/** if set, OnBecomeRelevant will be used */
	uint8_t bNotifyBecomeRelevant : 1;

	/** if set, OnCeaseRelevant will be used */
	uint8_t bNotifyCeaseRelevant : 1;

	/** if set, OnTick will be used */
	uint8_t bNotifyTick : 1;

	/** if set, conditional tick will use remaining time from node's memory */
	uint8_t bTickIntervals : 1;

	/** child index in parent node */
	uint8_t ChildIndex;
};