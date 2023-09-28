#pragma once

#include <vector>
#include <stack>
#include <map>
#include "BehaviorTree.h"

class NodeBase;
//class EnemyBlueSlime;

// Behavior�ۑ��f�[�^
class BehaviorData
{
public:
	// �R���X�g���N�^
	BehaviorData() { Init(); }

	// ������
	void Init();

public:
	// �V�[�P���X�m�[�h�̃v�b�V��
	void PushSequenceNode(NodeBase* node) { sequenceStack_.push(node); }
	// �V�[�P���X�m�[�h�̃|�b�v
	NodeBase* PopSequenceNode();

public:
	// �V�[�P���X�X�e�b�v�̃Q�b�^�[
	const int GetSequenceStep(const std::string& name);
	// �V�[�P���X�X�e�b�v�̃Z�b�^�[
	void SetSequenceStep(const std::string& name, const int step);


private:
	std::stack<NodeBase*> sequenceStack_;				// ���s���钆�ԃm�[�h���X�^�b�N
	std::map<std::string, int> runSequenceStepMap_;		// ���s���̒��ԃm�[�h�̃X�e�b�v���L�^

};
