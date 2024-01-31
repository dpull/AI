#include "BehaviorTreeComponent.h"
const float KINDA_SMALL_NUMBER = 0.0001f;

void UBrainComponent::InitializeComponent()
{
}

void UBrainComponent::StartLogic()
{
}

void UBrainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (MessagesToProcess.size() > 0)
	{
		const auto NumMessages = MessagesToProcess.size();
		for (auto Idx = 0; Idx < NumMessages; Idx++)
		{
			// create a copy of message in case MessagesToProcess is changed during loop
			const FAIMessage MessageCopy(MessagesToProcess[Idx]);

			for (int32_t ObserverIndex = 0; ObserverIndex < MessageObservers.size(); ObserverIndex++)
			{
				MessageObservers[ObserverIndex]->OnMessage(MessageCopy);
			}
		}
		MessagesToProcess.erase(MessagesToProcess.begin(), MessagesToProcess.begin() + NumMessages);
	}
}

void UBehaviorTreeComponent::StartLogic()
{
	if (TreeHasBeenStarted())
	{
		return;
	}

	if (TreeStartInfo.IsSet() == false)
	{
		// TreeStartInfo.Asset = DefaultBehaviorTreeAsset;
	}

	if (TreeStartInfo.IsSet())
	{
		TreeStartInfo.bPendingInitialize = true;
		ProcessPendingInitialize();
	}
}

void UBehaviorTreeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}

bool UBehaviorTreeComponent::TreeHasBeenStarted() const
{
	return false;
}

void UBehaviorTreeComponent::ProcessPendingInitialize()
{
	if (((int)SuspendedBranchActions & (int)EBTBranchAction::ProcessPendingInitialize) != (int)EBTBranchAction::None)
	{
		return;
	}

	StopTree(EBTStopMode::Safe);
}

void UBehaviorTreeComponent::StopTree(EBTStopMode StopMode)
{
	const bool bForcedStop = StopMode == EBTStopMode::Forced;
	if (((int)SuspendedBranchActions & (int)EBTBranchAction::StopTree) != (int)EBTBranchAction::None)
	{
		return;
	}

	FBTSuspendBranchActionsScoped ScopedSuspend(*this, EBTBranchAction::All);
	if (!bRequestedStop)
	{
		bRequestedStop = true;

		for (int32_t InstanceIndex = (int32_t)InstanceStack.size() - 1; InstanceIndex >= 0; InstanceIndex--)
		{
			FBehaviorTreeInstance& InstanceInfo = InstanceStack[InstanceIndex];

			// notify active aux nodes
			{
				InstanceInfo.ExecuteOnEachAuxNode([&InstanceInfo, this](const UBTAuxiliaryNode& AuxNode) {});
			}
			InstanceInfo.ResetActiveAuxNodes();

			// notify active parallel tasks
			//
			// calling OnTaskFinished with result other than InProgress will unregister parallel task,
			// modifying array we're iterating on - iterator needs to be moved one step back in that case
			//
			InstanceInfo.ExecuteOnEachParallelTask([&InstanceInfo, this](const FBehaviorTreeParallelTask& ParallelTaskInfo, const int32_t ParallelIndex) {});

			// notify active task
		}
	}
}

void UBehaviorTreeComponent::ScheduleNextTick(float NextNeededDeltaTime)
{
}

bool UBehaviorTreeComponent::TrackPendingLatentAborts()
{
	return false;
}

void UBehaviorTreeComponent::ScheduleExecutionUpdate()
{
	ScheduleNextTick(0.0f);
	bRequestedFlowUpdate = true;
}

void UBehaviorTreeComponent::ProcessExecutionRequest()
{
}

void FBehaviorTreeInstance::ExecuteOnEachAuxNode(std::function<void(const UBTAuxiliaryNode&)> ExecFunc)
{
}

void FBehaviorTreeInstance::ResetActiveAuxNodes()
{
}

void FBehaviorTreeInstance::ExecuteOnEachParallelTask(std::function<void(const FBehaviorTreeParallelTask&, const int32_t)> ExecFunc)
{
}
