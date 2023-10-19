#include "BehaviorTree.h"

#include "../../Taki174/Common.h"

#include "ActionBase.h"
#include "NodeBase.h"
#include "JudgmentBase.h"
#include "BehaviorData.h"

#include <assert.h>

/// <summary>
/// ノード追加関数
/// </summary>
/// <param name="parentName：親の名前"></param>
/// <param name="entryName：追加するノードの名前"></param>
/// <param name="priority：優先順位"></param>
/// <param name="selectRule：選択ルール"></param>
/// <param name="judgment：行動に移る判定クラス"></param>
/// <param name="action：行動クラス"></param>
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
			assert("Parent Node Not Found (or wrong parentName)");
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
	std::vector<NodeBase*>* children = delNode->GetChildren();

	const size_t count = children->size();
	if (count > 0)
	{
		for (NodeBase* node : (*children))
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

