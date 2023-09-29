#include "BehaviorData.h"
#include "NodeBase.h"

// 初期化
void BehaviorData::Init()
{
	runSequenceStepMap_.clear();
	while (sequenceStack_.size() > 0)
	{
		sequenceStack_.pop();
	}
}

// シーケンスノードのポップ
NodeBase* BehaviorData::PopSequenceNode()
{
	// 空ならnullptr
	if (true == sequenceStack_.empty()) return nullptr;

	NodeBase* node = sequenceStack_.top();
	if (node != nullptr)
	{
		// 取り出したデータを削除
		sequenceStack_.pop();
	}

	return node;
}

// シーケンスステップのゲッター
const int BehaviorData::GetSequenceStep(const std::string& name)
{
	if (0 == runSequenceStepMap_.count(name))
	{
		runSequenceStepMap_.insert(std::make_pair(name, 0));
	}

	return runSequenceStepMap_.at(name);
}

// シーケンスステップのセッター
void BehaviorData::SetSequenceStep(const std::string& name, const int step)
{
	runSequenceStepMap_.at(name) = step;
}