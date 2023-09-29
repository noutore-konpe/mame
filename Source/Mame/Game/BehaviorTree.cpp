#include "BehaviorTree.h"

#include "../../Taki174/Common.h"

#include "ActionBase.h"
#include "NodeBase.h"
#include "JudgmentBase.h"
//#include "EnemyBlueSlime.h"
#include "BehaviorData.h"


void BehaviorTree::AddNode(
	const std::string& parentName,
	const std::string& entryName,
	const int priority,
	const SelectRule& selectRule,
	JudgmentBase* judgment,
	ActionBase* action)
{
	using std::make_unique;

	if (parentName != "")
	{
		NodeBase* parentNode = root_->SearchNode(parentName);

		if (parentNode != nullptr)
		{
			NodeBase* sibling = parentNode->GetLastChild();
			NodeBase* addNode = {
				new NodeBase(
					entryName, parentNode, sibling,
					priority, selectRule,
					judgment, action,
					parentNode->GetHirerchyNo() + 1)
			};

			parentNode->AddChild(addNode);
		}
		else
		{
			assert("Parent Node is not found (or wrong parent's name)");
		}
	}
	else
	{
		if (nullptr == root_)
		{
			//root_ = new NodeBase(entryName, nullptr, nullptr, priority, selectRule, judgment, action, 1);
			root_ = {
				make_unique<NodeBase>(
				entryName, nullptr, nullptr,
				priority, selectRule,
				judgment, action, 1)
			};
		}
	}
}

// 推論
NodeBase* BehaviorTree::ActiveNodeInference(BehaviorData* data)
{
	// データをリセットして開始
	data->Init();
	return root_->Inference(data);
}

// シーケンスノードからの推論開始
NodeBase* BehaviorTree::SequenceBack(
	NodeBase* sequenceNode,
	BehaviorData* data)
{
	return sequenceNode->Inference(data);
}

// ノード実行
NodeBase* BehaviorTree::Run(
	NodeBase*		actionNode,
	BehaviorData*	data,
	const float		elapsedTime)
{
	using State = ActionBase::State;

	// ノード実行
	State state = actionNode->Run(elapsedTime);

	// 正常終了
	if (State::Complete == state)
	{
		// シーケンスの途中かを判断
		NodeBase* sequenceNode = data->PopSequenceNode();

		// 途中じゃないなら終了
		if (nullptr == sequenceNode)
		{
			return nullptr;
		}
		else
		{
			// 途中ならそこから始める
			return SequenceBack(sequenceNode, data);
		}
		// 失敗は終了
	}
	else if (State::Failed == state)
	{
		return nullptr;
	}

	// 現状維持
	return actionNode;
}

// 登録されたノードを全て削除する
void BehaviorTree::NodeAllClear(NodeBase* delNode)
{
	const size_t count = delNode->children_.size();
	if (count > 0)
	{
		for (NodeBase* node : delNode->children_)
		{
			NodeAllClear(node);
		}
		SafeDeletePtr(delNode);
	}
	else
	{
		SafeDeletePtr(delNode);
	}
}

