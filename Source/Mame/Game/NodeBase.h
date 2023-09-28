#pragma once

#include <vector>
#include <string>
#include "BehaviorTree.h"
#include "ActionBase.h"

class JudgmentBase;
class BehaviorData;

// メモリリーク調査用
#define debug_new new(_NORMAL_BLOCK,__FILE__,__LINE__)

// ノード
class NodeBase
{
public:
	NodeBase(
		const std::string& name,
		NodeBase* parent,
		NodeBase* sibling,
		const int priority,
		const BehaviorTree::SelectRule& selectRule,
		JudgmentBase* judgment,
		ActionBase* action,
		const int hierarchyNo
	);
	~NodeBase();

public:
	// 名前ゲッター
	const std::string& GetName() const { return name_; }

	// 親ノードゲッター
	NodeBase* GetParent() const { return parent_; }
	// 子ノードゲッター
	NodeBase* GetChild(const int index) const;
	// 子ノードゲッター(末尾)
	NodeBase* GetLastChild() const;
	// 子ノードゲッター(先頭)
	NodeBase* GetTopChild() const;
	// 兄弟ノードゲッター
	NodeBase* GetSibling() const { return sibling_; }

	// 階層番号ゲッター
	const int GetHirerchyNo() const { return hierarchyNo_; }
	// 優先順位ゲッター
	const int GetPriority() const { return priority_; }

public:
	// 親ノードセッター
	void SetParent(NodeBase* parent) { this->parent_ = parent; }
	// 兄弟ノードセッター
	void SetSibling(NodeBase* sibling) {this->sibling_ = sibling;}

public:
	// 子ノード追加
	void AddChild(NodeBase* child) { children_.push_back(child); }

public:
	// 行動データを持っているか
	const bool HasAction() const { return (action_ != nullptr); }
	// 実行可否判定
	const bool Judgment() const;

public:
	// 優先順位選択
	NodeBase* SelectPriority(std::vector<NodeBase*>* list);
	// ランダム選択
	NodeBase* SelectRandom(std::vector<NodeBase*>* list);
	// シーケンス選択
	NodeBase* SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data);

public:
	// ノード検索
	NodeBase* SearchNode(const std::string& searchName);
	// ノード推論
	NodeBase* Inference(BehaviorData* data);
	// 実行
	const ActionBase::State Run(const float elapsedTime);

public:
	std::vector<NodeBase*>		children_;		// 子ノード

protected:
	std::string					name_;			// 名前
	BehaviorTree::SelectRule	selectRule_;	// 選択ルール
	JudgmentBase*				judgment_;		// 判定クラス
	ActionBase*					action_;		// 実行クラス
	unsigned int				priority_;		// 優先順位
	NodeBase*					parent_;		// 親ノード
	NodeBase*					sibling_;		// 兄弟ノード
	int							hierarchyNo_;	// 階層番号

};