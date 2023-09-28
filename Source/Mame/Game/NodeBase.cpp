#include "NodeBase.h"

#include <stdlib.h>

#include "../../Taki174/Common.h"

#include "JudgmentBase.h"
//#include "EnemyBlueSlime.h"
#include "BehaviorData.h"
#include "ActionBase.h"


NodeBase::NodeBase(
	const std::string& name,
	NodeBase* parent,
	NodeBase* sibling,
	const int priority,
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
	SafeDelete(judgment_);
	SafeDelete(action_);
}

// 子ノードゲッター
NodeBase* NodeBase::GetChild(const int index)  const
{
	if (children_.size() <= index)
	{
		return nullptr;
	}
	return children_.at(index);
}

// 子ノードゲッター(末尾)
NodeBase* NodeBase::GetLastChild() const
{
	if (children_.size() == 0)
	{
		return nullptr;
	}

	return children_.at(children_.size() - 1);
}

// 子ノードゲッター(先頭)
NodeBase* NodeBase::GetTopChild()  const
{
	if (children_.size() == 0)
	{
		return nullptr;
	}

	return children_.at(0);
}


// ノード検索
NodeBase* NodeBase::SearchNode(const std::string& searchName)
{
	// 名前が一致
	if (searchName == name_)
	{
		return this;
	}
	else
	{
		// 子ノードで検索
		for (auto itr = children_.begin(); itr != children_.end(); ++itr)
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
	std::vector<NodeBase*> list = {};
	NodeBase* result = nullptr;

	// childrenの数だけループを行う。
	for (int i = 0; i < children_.size(); ++i)
	{
		// children.at(i)->judgmentがnullptrでなければ
		if (children_.at(i)->judgment_ != nullptr)
		{
			// TODO 04_03 children.at(i)->judgment->Judgment()関数を実行し、tureであれば
			// listにchildren.at(i)を追加していく
			if (children_.at(i)->judgment_->Judgment())
			{
				list.emplace_back(children_.at(i));
			}
		}
		else {
			// TODO 04_03 判定クラスがなければ無条件に追加
			list.emplace_back(children_.at(i));
		}
	}

	// 選択ルールでノード決め
	switch (selectRule_)
	{
		// 優先順位
	case BehaviorTree::SelectRule::Priority:
		result = SelectPriority(&list);
		break;
		// ランダム
	case BehaviorTree::SelectRule::Random:
		result = SelectRandom(&list);
		break;
		// シーケンス
	case BehaviorTree::SelectRule::Sequence:
	case BehaviorTree::SelectRule::SequentialLooping:
		result = SelectSequence(&list, data);
		break;
	}

	if (result != nullptr)
	{
		// 行動があれば終了
		if (true == result->HasAction())
		{
			return result;
		}
		else
		{
			// 決まったノードで推論開始
			result = result->Inference(data);
		}
	}

	return result;
}

// 優先順位でノード選択
NodeBase* NodeBase::SelectPriority(std::vector<NodeBase*>* list)
{
	NodeBase* selectNode  = nullptr;
	unsigned int priority = INT_MAX;

	// TODO 04_04 一番優先順位が高いノードを探してselectNodeに格納
	const size_t nodeCount = list->size();
	for (size_t i = 0; i < nodeCount; ++i)
	{
		NodeBase* node = list->at(i);
		if (node->priority_ < priority)
		{
			priority   = node->priority_;
			selectNode = node;
		}
	}

	return selectNode;
}


// ランダムでノード選択
NodeBase* NodeBase::SelectRandom(std::vector<NodeBase*>* list)
{
	// TODO 04_05 listのサイズで乱数を取得してselectNoに格納
	const size_t nodeCount = list->size();
	const size_t selectNo  = static_cast<size_t>(std::rand()) % nodeCount;

	// listのselectNo番目の実態をリターン
	return (*list).at(selectNo);
}

// シーケンス・シーケンシャルルーピングでノード選択
NodeBase* NodeBase::SelectSequence(
	std::vector<NodeBase*>* list,
	BehaviorData* data)
{
	int step = 0;

	// 指定されている中間ノードのがシーケンスがどこまで実行されたか取得する
	step = data->GetSequenceStep(name_);

	// 中間ノードに登録されているノード数以上の場合、
	if (step >= children_.size())
	{
		// TODO 04_06 ルールによって処理を切り替える
		// ルールがBehaviorTree::SelectRule::SequentialLoopingのときは最初から実行するため、stepに0を代入
		// ルールがBehaviorTree::SelectRule::Sequenceのときは次に実行できるノードがないため、nullptrをリターン
		using SelectRule = BehaviorTree::SelectRule;
		if (SelectRule::SequentialLooping == children_.at(step)->selectRule_)
		{
			step = 0;
		}
		else if (SelectRule::Sequence == children_.at(step)->selectRule_)
		{
			return nullptr;
		}

	}
	// 実行可能リストに登録されているデータの数だけループを行う
	for (auto itr = list->begin(); itr != list->end(); itr++)
	{
		// 子ノードが実行可能リストに含まれているか
		if (children_.at(step)->GetName() == (*itr)->GetName())
		{
			// TODO 04_06 現在の実行ノードの保存、次に実行するステップの保存を行った後、
			// 現在のステップ番号のノードをリターンしなさい
			// ①スタックにはdata->PushSequenceNode関数を使用する。保存するデータは実行中の中間ノード。
			// ②また、次に実行する中間ノードとステップ数を保存する
			// 　保存にはdata->SetSequenceStep関数を使用。
			// 　保存データは中間ノードの名前と次のステップ数です(step + 1)
			// ③ステップ番号目の子ノードを実行ノードとしてリターンする



		}
	}
	// 指定された中間ノードに実行可能ノードがないのでnullptrをリターンする
	return nullptr;
}

// 判定
const bool NodeBase::Judgment()  const
{
	// TODO 04_07 judgmentがあるか判断。あればメンバ関数Judgment()実行した結果をリターン。
	if (judgment_ != nullptr)
	{
		//return judgment_->Judgment();
	}
	return true;
}

// ノード実行
const ActionBase::State NodeBase::Run(const float elapsedTime)
{
	// TODO 04_08 actionがあるか判断。あればメンバ関数Run()実行した結果をリターン。
	if (action_ != nullptr)
	{
		//return action_->Run(elapsedTime);
	}

	return ActionBase::State::Failed;
}
