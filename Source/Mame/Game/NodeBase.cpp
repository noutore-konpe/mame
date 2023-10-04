#include "NodeBase.h"

#include <stdlib.h>
#include <memory>

#include "../../Taki174/Common.h"

#include "JudgmentBase.h"
//#include "EnemyBlueSlime.h"
#include "BehaviorData.h"
#include "ActionBase.h"
#include "EnemyManager.h"

NodeBase::NodeBase(
	const std::string& name,
	NodeBase* parent,
	NodeBase* sibling,
	const unsigned int priority,
	const BehaviorTree::SelectRule& selectRule,
	JudgmentBase* judgment,
	ActionBase* action,
	const int hierarchyNo)
	: name_(name)
	, parent_(parent)
	, sibling_(sibling)
	, priority_(priority)
	, selectRule_(selectRule)
	, judgment_(judgment)
	, action_(action)
	, hierarchyNo_(hierarchyNo)
	, children_(0)
{
}

// デストラクタ
NodeBase::~NodeBase()
{
	SafeDeletePtr(judgment_);
	SafeDeletePtr(action_);
}

// 子ノードゲッター
NodeBase* NodeBase::GetChild(const int index) const
{
	if (children_.size() <= index) return nullptr;

	return children_.at(index);
}

// 子ノードゲッター(末尾)
NodeBase* NodeBase::GetLastChild() const
{
	if (children_.size() == 0) return nullptr;

	return children_.at(children_.size() - 1);
}

// 子ノードゲッター(先頭)
NodeBase* NodeBase::GetTopChild()  const
{
	if (children_.size() == 0) return nullptr;

	return children_.at(0);
}


// ノード検索
NodeBase* NodeBase::SearchNode(const std::string& searchName)
{
	// 名前が一致（）
	if (searchName == name_)
	{
		return this;
	}
	else
	{
		// 子ノードで検索
		using Iterator = std::vector<NodeBase*>::iterator;
		for (Iterator itr = children_.begin(); itr != children_.end(); ++itr)
		{
			NodeBase* ret = (*itr)->SearchNode(searchName);

			if (ret != nullptr)
			{
				return ret;
			}
		}
	}

	return nullptr;
}

// ノード推論
NodeBase* NodeBase::Inference(BehaviorData* data)
{
	using std::vector;
	using SelectRule = BehaviorTree::SelectRule;

	vector<NodeBase*> candidateList = {}; // 実行するノードの候補リスト
	NodeBase* selectNode = nullptr;	  // 選択された実行ノード

	// childrenの数だけループを行う。
	for (int i = 0; i < children_.size(); ++i)
	{
		NodeBase* node = children_.at(i);
		// children.at(i)->judgmentがnullptrでなければ
		if (node->judgment_ != nullptr)
		{
			// Judgment関数を実行し、trueであれば
			// listにchildren.at(i)を追加していく
			if (node->judgment_->Judgment())
			{
				candidateList.emplace_back(node);
			}
		}
		else
		{
			// 判定クラスがなければ無条件に追加
			candidateList.emplace_back(node);
		}
	}

	// 選択ルールでノード決め
	switch (selectRule_)
	{
		// 優先順位
	case SelectRule::Priority:
		selectNode = SelectPriority(&candidateList);
		break;
		// ランダム
	case SelectRule::Random:
		selectNode = SelectRandom(&candidateList);
		break;
		// シーケンス
	case SelectRule::Sequence:
	case SelectRule::SequentialLooping:
		selectNode = SelectSequence(&candidateList, data);
		break;
	}

	if (selectNode != nullptr)
	{
		// 行動があれば終了
		if (true == selectNode->HasAction())
		{
			// CRA : 3.Inference : 実行ノードが近接攻撃ノードなら近接攻撃行動実行中フラグを立てる
			if (selectNode->name_ == "CloseRangeAttack")
			{
				EnemyManager::Instance().SetIsRunningCRAAction(true);
			}

			return selectNode;
		}
		else
		{
			// 決まったノードで推論開始
			selectNode = selectNode->Inference(data);
		}
	}

	return selectNode;
}

// 優先順位でノード選択
NodeBase* NodeBase::SelectPriority(std::vector<NodeBase*>* candidateList)
{
	NodeBase* selectNode  = nullptr;
	unsigned int priority = UINT_MAX;

	// 一番優先順位が高い(値が小さい)ノードを探してselectNodeに格納
	for (size_t i = 0; i < (*candidateList).size(); ++i)
	{
		NodeBase* node = (*candidateList).at(i);
		if (node->priority_ < priority)
		{
			priority   = node->priority_;
			selectNode = node;
		}
	}

	return selectNode;
}


// ランダムでノード選択
NodeBase* NodeBase::SelectRandom(std::vector<NodeBase*>* candidateList)
{
	// listのサイズで乱数を取得してselectNoに格納
	const size_t lastNode = (*candidateList).size() - 1;
	const size_t selectNo  = static_cast<size_t>(std::rand()) % lastNode;

	// listのselectNo番目の実態をリターン
	return (*candidateList).at(selectNo);
}

// シーケンス・シーケンシャルルーピングでノード選択
NodeBase* NodeBase::SelectSequence(
	std::vector<NodeBase*>* candidateList,
	BehaviorData* data)
{
	int step_ = 0;

	// 指定されている中間ノードのシーケンスがどこまで実行されたか取得する
	step_ = data->GetSequenceStep(name_);

	// 中間ノードに登録されているノード数以上の場合、
	if (step_ >= children_.size())
	{
		// ルールがSequentialLoopingのときは最初から実行するため、stepに0を代入
		using SelectRule = BehaviorTree::SelectRule;
		if (SelectRule::SequentialLooping == children_.at(step_)->selectRule_)
		{
			step_ = 0;
		}
		// ルールがSequenceのときは次に実行できるノードがないため、nullptrをリターン
		else if (SelectRule::Sequence == children_.at(step_)->selectRule_)
		{
			return nullptr;
		}

	}
	// 実行可能リストに登録されているデータの数だけループを行う
	using Iterator = std::vector<NodeBase*>::iterator;
	for (Iterator itr = candidateList->begin(); itr != candidateList->end(); ++itr)
	{
		// 子ノードが実行可能リストに含まれているか
		NodeBase* childNode		= children_.at(step_);
		NodeBase* condidateNode = (*itr);
		if (childNode->GetName() == condidateNode->GetName())
		{
			// 現在の実行ノードの保存
			data->PushSequenceNode(this);

			// 次に実行する中間ノードとステップ数を保存する
			data->SetSequenceStep(childNode->GetName(), step_ + 1);

			// 現在のステップ番号のノードを返す
			return childNode;
		}
	}
	// 指定された中間ノードに実行可能ノードがないのでnullptrをリターンする
	return nullptr;
}

// 判定
const bool NodeBase::Judgment()  const
{
	// judgmentがあるか判断。あればメンバ関数Judgment()実行した結果をリターン。
	if (judgment_ != nullptr)
	{
		return judgment_->Judgment();
	}

	return true;
}

// ノード実行
const ActionBase::State NodeBase::Run(const float elapsedTime)
{
	// actionがあるか判断。あればメンバ関数Run()実行した結果をリターン。
	if (action_ != nullptr)
	{
		return action_->Run(elapsedTime);
	}

	return ActionBase::State::Failed;
}
