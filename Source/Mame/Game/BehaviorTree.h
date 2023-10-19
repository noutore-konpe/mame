#pragma once

#include <string>
#include <memory>

class BehaviorData;
class NodeBase;
class ActionBase;
class JudgmentBase;
class BaseEnemyAI;

// �r�w�C�r�A�c���[
class BehaviorTree
{
public:
	// �I�����[��
	enum class SelectRule
	{
		Non,				// �������[�m�[�h�p
		Priority,			// �D�揇��
		Sequence,			// �V�[�P���X
		SequentialLooping,	// �V�[�P���V�������[�s���O
		Random,				// �����_��
	};

public:
	BehaviorTree() {}
	BehaviorTree(BaseEnemyAI* enemy) : owner_(enemy) {}
	~BehaviorTree() {}

	// ���s�m�[�h�𐄘_����
	NodeBase* ActiveNodeInference(BehaviorData* data);

	// �V�[�P���X�m�[�h���琄�_�J�n
	NodeBase* SequenceBack(NodeBase* sequenceNode, BehaviorData* data);

	// �m�[�h�ǉ�
	void AddNode(
		const std::string& parentName,
		const std::string& entryName,
		const int priority,
		const SelectRule& selectRule,
		JudgmentBase* judgment,
		ActionBase* action
	);

	// ���s
	NodeBase* Run(
		NodeBase* actionNode,
		BehaviorData* data,
		const float elapsedTime
	);

	const std::unique_ptr<NodeBase>& GetRoot() const { return root_; }

private:
	// �m�[�h�S�폜
	void NodeAllClear(NodeBase* delNode);

private:
	// ���[�g�m�[�h
	//NodeBase*		root_	= nullptr;
	std::unique_ptr<NodeBase>	root_	= nullptr;
	BaseEnemyAI*			owner_	= nullptr;

};
