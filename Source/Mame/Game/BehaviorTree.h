#pragma once

#include <string>
#include <memory>

class ActionBase;
class JudgmentBase;
class NodeBase;
class BehaviorData;
//class EnemyBlueSlime;
class BaseCharacterAI;

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
	BehaviorTree(BaseCharacterAI* enemy) : owner_(enemy) {}
	~BehaviorTree();

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

private:
	// �m�[�h�S�폜
	void NodeAllClear(NodeBase* delNode);

private:
	// ���[�g�m�[�h
	//NodeBase*		root_	= nullptr;
	std::unique_ptr<NodeBase>	root_	= nullptr;
	BaseCharacterAI*			owner_	= nullptr;

};
