#pragma once

#include <vector>
#include <stack>
#include <map>
#include "BehaviorTree.h"

class NodeBase;
//class EnemyBlueSlime;

// Behavior保存データ
class BehaviorData
{
public:
	// コンストラクタ
	BehaviorData() { Init(); }

	// 初期化
	void Init();

public:
	// シーケンスノードのプッシュ
	void PushSequenceNode(NodeBase* node) { sequenceStack_.push(node); }
	// シーケンスノードのポップ
	NodeBase* PopSequenceNode();

public:
	// シーケンスステップのゲッター
	const int GetSequenceStep(const std::string& name);
	// シーケンスステップのセッター
	void SetSequenceStep(const std::string& name, const int step);


private:
	std::stack<NodeBase*> sequenceStack_;				// 実行する中間ノードをスタック
	std::map<std::string, int> runSequenceStepMap_;		// 実行中の中間ノードのステップを記録

};
