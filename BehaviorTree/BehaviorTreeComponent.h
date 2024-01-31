#pragma once

#include <functional>
#include <vector>

class UBehaviorTree;
class UBTCompositeNode;

struct FActorComponentTickFunction
{
};
struct FAIMessage
{
};
struct FAIMessageObserver
{
	void OnMessage(const FAIMessage& Message)
	{
	}
};
class UBrainComponent
{
  public:
	virtual void InitializeComponent();
	virtual void StartLogic();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	std::vector<FAIMessage> MessagesToProcess;
	std::vector<FAIMessageObserver*> MessageObservers;
};

enum class EBTExecutionMode
{
	SingleRun,
	Looped,
};

enum class EBTBranchAction : uint16_t
{
	None = 0x0,
	DecoratorEvaluate = 0x1,
	DecoratorActivate_IfNotExecuting = 0x2,
	DecoratorActivate_EvenIfExecuting = 0x4,
	DecoratorActivate = DecoratorActivate_IfNotExecuting | DecoratorActivate_EvenIfExecuting,
	DecoratorDeactivate = 0x8,
	UnregisterAuxNodes = 0x10,
	StopTree_Safe = 0x20,
	StopTree_Forced = 0x40,
	ActiveNodeEvaluate = 0x80,
	SubTreeEvaluate = 0x100,
	ProcessPendingInitialize = 0x200,
	Cleanup = 0x400,
	UninitializeComponent = 0x800,
	StopTree = StopTree_Safe | StopTree_Forced,
	Changing_Topology_Actions = UnregisterAuxNodes | StopTree | ProcessPendingInitialize | Cleanup | UninitializeComponent,
	All = DecoratorEvaluate | DecoratorActivate_IfNotExecuting | DecoratorActivate_EvenIfExecuting | DecoratorDeactivate | Changing_Topology_Actions | ActiveNodeEvaluate |
		  SubTreeEvaluate,
};

enum class EBTStopMode
{
	Safe,
	Forced,
};

struct FBTTreeStartInfo
{
	UBehaviorTree* Asset;
	EBTExecutionMode ExecuteMode;
	uint8_t bPendingInitialize : 1;

	FBTTreeStartInfo() : Asset(nullptr), ExecuteMode(EBTExecutionMode::Looped), bPendingInitialize(0)
	{
	}
	bool IsSet() const
	{
		return Asset != nullptr;
	}
	bool HasPendingInitialize() const
	{
		return bPendingInitialize && IsSet();
	}
};

struct UBTAuxiliaryNode
{
};

struct FBehaviorTreeParallelTask
{
};

enum class EBTTaskStatus
{
	Active,
	Aborting,
	Inactive,
};

struct FBehaviorTreeInstance
{
	void ExecuteOnEachAuxNode(std::function<void(const UBTAuxiliaryNode&)> ExecFunc);
	void ResetActiveAuxNodes();
	void ExecuteOnEachParallelTask(std::function<void(const FBehaviorTreeParallelTask&, const int32_t)> ExecFunc);
};

struct FBTNodeIndex
{
	bool IsSet() const
	{
		return false;
	}
};

struct FBTNodeExecutionInfo
{
	/** index of first task allowed to be executed */
	FBTNodeIndex SearchStart;

	/** index of last task allowed to be executed */
	FBTNodeIndex SearchEnd;

	const UBTCompositeNode* ExecuteNode;

	/** subtree index */
	uint16_t ExecuteInstanceIdx;

	/** if set, tree will try to execute next child of composite instead of forcing branch containing SearchStart */
	uint8_t bTryNextChild : 1;

	/** if set, request was not instigated by finishing task/initialization but is a restart (e.g. decorator) */
	uint8_t bIsRestart : 1;
};

class UBehaviorTreeComponent : public UBrainComponent
{
	virtual void StartLogic() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool TreeHasBeenStarted() const;
	void ProcessPendingInitialize();
	void StopTree(EBTStopMode StopMode = EBTStopMode::Safe);
	void ExecuteOnEachAuxNode(std::function<void(const UBTAuxiliaryNode&)> ExecFunc);
	void ScheduleNextTick(float NextDeltaTime);
	bool TrackPendingLatentAborts();
	void ScheduleExecutionUpdate();
	void ProcessExecutionRequest();

	struct FBTSuspendBranchActionsScoped
	{
		FBTSuspendBranchActionsScoped(UBehaviorTreeComponent& InBTComp, EBTBranchAction BranchActions = EBTBranchAction::All) : BTComp(InBTComp)
		{
		}
		~FBTSuspendBranchActionsScoped()
		{
		}
		UBehaviorTreeComponent& BTComp;
	};

	FBTTreeStartInfo TreeStartInfo;
	EBTBranchAction SuspendedBranchActions;
	std::vector<FBehaviorTreeInstance> InstanceStack;
	uint8_t bRequestedStop : 1;
	uint8_t bRequestedFlowUpdate : 1;
	uint8_t bIsRunning : 1;
	uint8_t bIsPaused : 1;
	bool bTickedOnce = false;
	float NextTickDeltaTime = 0.0f;
	float LastRequestedDeltaTimeGameTime = 0;
	float AccumulatedTickDeltaTime = 0.0f;
	FBTNodeExecutionInfo ExecutionRequest;
	uint16_t ActiveInstanceIdx;
};
