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

// �f�X�g���N�^
NodeBase::~NodeBase()
{
	SafeDelete(judgment_);
	SafeDelete(action_);
}

// �q�m�[�h�Q�b�^�[
NodeBase* NodeBase::GetChild(const int index)  const
{
	if (children_.size() <= index)
	{
		return nullptr;
	}
	return children_.at(index);
}

// �q�m�[�h�Q�b�^�[(����)
NodeBase* NodeBase::GetLastChild() const
{
	if (children_.size() == 0)
	{
		return nullptr;
	}

	return children_.at(children_.size() - 1);
}

// �q�m�[�h�Q�b�^�[(�擪)
NodeBase* NodeBase::GetTopChild()  const
{
	if (children_.size() == 0)
	{
		return nullptr;
	}

	return children_.at(0);
}


// �m�[�h����
NodeBase* NodeBase::SearchNode(const std::string& searchName)
{
	// ���O����v
	if (searchName == name_)
	{
		return this;
	}
	else
	{
		// �q�m�[�h�Ō���
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

// �m�[�h���_
NodeBase* NodeBase::Inference(BehaviorData* data)
{
	std::vector<NodeBase*> list = {};
	NodeBase* result = nullptr;

	// children�̐��������[�v���s���B
	for (int i = 0; i < children_.size(); ++i)
	{
		// children.at(i)->judgment��nullptr�łȂ����
		if (children_.at(i)->judgment_ != nullptr)
		{
			// TODO 04_03 children.at(i)->judgment->Judgment()�֐������s���Ature�ł����
			// list��children.at(i)��ǉ����Ă���
			if (children_.at(i)->judgment_->Judgment())
			{
				list.emplace_back(children_.at(i));
			}
		}
		else {
			// TODO 04_03 ����N���X���Ȃ���Ζ������ɒǉ�
			list.emplace_back(children_.at(i));
		}
	}

	// �I�����[���Ńm�[�h����
	switch (selectRule_)
	{
		// �D�揇��
	case BehaviorTree::SelectRule::Priority:
		result = SelectPriority(&list);
		break;
		// �����_��
	case BehaviorTree::SelectRule::Random:
		result = SelectRandom(&list);
		break;
		// �V�[�P���X
	case BehaviorTree::SelectRule::Sequence:
	case BehaviorTree::SelectRule::SequentialLooping:
		result = SelectSequence(&list, data);
		break;
	}

	if (result != nullptr)
	{
		// �s��������ΏI��
		if (true == result->HasAction())
		{
			return result;
		}
		else
		{
			// ���܂����m�[�h�Ő��_�J�n
			result = result->Inference(data);
		}
	}

	return result;
}

// �D�揇�ʂŃm�[�h�I��
NodeBase* NodeBase::SelectPriority(std::vector<NodeBase*>* list)
{
	NodeBase* selectNode  = nullptr;
	unsigned int priority = INT_MAX;

	// TODO 04_04 ��ԗD�揇�ʂ������m�[�h��T����selectNode�Ɋi�[
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


// �����_���Ńm�[�h�I��
NodeBase* NodeBase::SelectRandom(std::vector<NodeBase*>* list)
{
	// TODO 04_05 list�̃T�C�Y�ŗ������擾����selectNo�Ɋi�[
	const size_t nodeCount = list->size();
	const size_t selectNo  = static_cast<size_t>(std::rand()) % nodeCount;

	// list��selectNo�Ԗڂ̎��Ԃ����^�[��
	return (*list).at(selectNo);
}

// �V�[�P���X�E�V�[�P���V�������[�s���O�Ńm�[�h�I��
NodeBase* NodeBase::SelectSequence(
	std::vector<NodeBase*>* list,
	BehaviorData* data)
{
	int step = 0;

	// �w�肳��Ă��钆�ԃm�[�h�̂��V�[�P���X���ǂ��܂Ŏ��s���ꂽ���擾����
	step = data->GetSequenceStep(name_);

	// ���ԃm�[�h�ɓo�^����Ă���m�[�h���ȏ�̏ꍇ�A
	if (step >= children_.size())
	{
		// TODO 04_06 ���[���ɂ���ď�����؂�ւ���
		// ���[����BehaviorTree::SelectRule::SequentialLooping�̂Ƃ��͍ŏ�������s���邽�߁Astep��0����
		// ���[����BehaviorTree::SelectRule::Sequence�̂Ƃ��͎��Ɏ��s�ł���m�[�h���Ȃ����߁Anullptr�����^�[��
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
	// ���s�\���X�g�ɓo�^����Ă���f�[�^�̐��������[�v���s��
	for (auto itr = list->begin(); itr != list->end(); itr++)
	{
		// �q�m�[�h�����s�\���X�g�Ɋ܂܂�Ă��邩
		if (children_.at(step)->GetName() == (*itr)->GetName())
		{
			// TODO 04_06 ���݂̎��s�m�[�h�̕ۑ��A���Ɏ��s����X�e�b�v�̕ۑ����s������A
			// ���݂̃X�e�b�v�ԍ��̃m�[�h�����^�[�����Ȃ���
			// �@�X�^�b�N�ɂ�data->PushSequenceNode�֐����g�p����B�ۑ�����f�[�^�͎��s���̒��ԃm�[�h�B
			// �A�܂��A���Ɏ��s���钆�ԃm�[�h�ƃX�e�b�v����ۑ�����
			// �@�ۑ��ɂ�data->SetSequenceStep�֐����g�p�B
			// �@�ۑ��f�[�^�͒��ԃm�[�h�̖��O�Ǝ��̃X�e�b�v���ł�(step + 1)
			// �B�X�e�b�v�ԍ��ڂ̎q�m�[�h�����s�m�[�h�Ƃ��ă��^�[������



		}
	}
	// �w�肳�ꂽ���ԃm�[�h�Ɏ��s�\�m�[�h���Ȃ��̂�nullptr�����^�[������
	return nullptr;
}

// ����
const bool NodeBase::Judgment()  const
{
	// TODO 04_07 judgment�����邩���f�B����΃����o�֐�Judgment()���s�������ʂ����^�[���B
	if (judgment_ != nullptr)
	{
		//return judgment_->Judgment();
	}
	return true;
}

// �m�[�h���s
const ActionBase::State NodeBase::Run(const float elapsedTime)
{
	// TODO 04_08 action�����邩���f�B����΃����o�֐�Run()���s�������ʂ����^�[���B
	if (action_ != nullptr)
	{
		//return action_->Run(elapsedTime);
	}

	return ActionBase::State::Failed;
}
