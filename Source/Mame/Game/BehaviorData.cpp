#include "BehaviorData.h"
#include "NodeBase.h"

// ������
void BehaviorData::Init()
{
	runSequenceStepMap_.clear();
	while (sequenceStack_.size() > 0)
	{
		sequenceStack_.pop();
	}
}

// �V�[�P���X�m�[�h�̃|�b�v
NodeBase* BehaviorData::PopSequenceNode()
{
	// ��Ȃ�nullptr
	if (true == sequenceStack_.empty()) return nullptr;

	NodeBase* node = sequenceStack_.top();
	if (node != nullptr)
	{
		// ���o�����f�[�^���폜
		sequenceStack_.pop();
	}

	return node;
}

// �V�[�P���X�X�e�b�v�̃Q�b�^�[
const int BehaviorData::GetSequenceStep(const std::string& name)
{
	if (0 == runSequenceStepMap_.count(name))
	{
		runSequenceStepMap_.insert(std::make_pair(name, 0));
	}

	return runSequenceStepMap_.at(name);
}

// �V�[�P���X�X�e�b�v�̃Z�b�^�[
void BehaviorData::SetSequenceStep(const std::string& name, const int step)
{
	runSequenceStepMap_.at(name) = step;
}