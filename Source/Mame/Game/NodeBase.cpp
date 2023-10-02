#include "NodeBase.h"

#include <stdlib.h>
#include <memory>

#include "../../Taki174/Common.h"

#include "JudgmentBase.h"
//#include "EnemyBlueSlime.h"
#include "BehaviorData.h"
#include "ActionBase.h"


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
	//, judgment_(judgment)
	//, action_(action)
	, hierarchyNo_(hierarchyNo)
	, children_(0)
{
	judgment_.reset(judgment);
	action_.reset(action);
}

// �f�X�g���N�^
NodeBase::~NodeBase()
{
	//SafeDeletePtr(judgment_);
	//SafeDeletePtr(action_);
}

// �q�m�[�h�Q�b�^�[
NodeBase* NodeBase::GetChild(const int index) const
{
	if (children_.size() <= index) return nullptr;

	return children_.at(index);
}

// �q�m�[�h�Q�b�^�[(����)
NodeBase* NodeBase::GetLastChild() const
{
	if (children_.size() == 0) return nullptr;

	return children_.at(children_.size() - 1);
}

// �q�m�[�h�Q�b�^�[(�擪)
NodeBase* NodeBase::GetTopChild()  const
{
	if (children_.size() == 0) return nullptr;

	return children_.at(0);
}


// �m�[�h����
NodeBase* NodeBase::SearchNode(const std::string& searchName)
{
	// ���O����v�i�j
	if (searchName == name_)
	{
		return this;
	}
	else
	{
		// �q�m�[�h�Ō���
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

// �m�[�h���_
NodeBase* NodeBase::Inference(BehaviorData* data)
{
	using std::vector;
	using SelectRule = BehaviorTree::SelectRule;

	vector<NodeBase*> candidateList = {}; // ���s����m�[�h�̌�⃊�X�g
	NodeBase* selectNode = nullptr;	  // �I�����ꂽ���s�m�[�h

	// children�̐��������[�v���s���B
	for (int i = 0; i < children_.size(); ++i)
	{
		NodeBase* node = children_.at(i);
		// children.at(i)->judgment��nullptr�łȂ����
		if (node->judgment_ != nullptr)
		{
			// TODO 04_03 children.at(i)->judgment->Judgment()�֐������s���Atrue�ł����
			// list��children.at(i)��ǉ����Ă���
			if (node->judgment_->Judgment())
			{
				candidateList.emplace_back(node);
			}
		}
		else
		{
			// TODO 04_03 ����N���X���Ȃ���Ζ������ɒǉ�
			candidateList.emplace_back(node);
		}
	}

	// �I�����[���Ńm�[�h����
	switch (selectRule_)
	{
		// �D�揇��
	case SelectRule::Priority:
		selectNode = SelectPriority(&candidateList);
		break;
		// �����_��
	case SelectRule::Random:
		selectNode = SelectRandom(&candidateList);
		break;
		// �V�[�P���X
	case SelectRule::Sequence:
	case SelectRule::SequentialLooping:
		selectNode = SelectSequence(&candidateList, data);
		break;
	}

	if (selectNode != nullptr)
	{
		// �s��������ΏI��
		if (true == selectNode->HasAction())
		{
			return selectNode;
		}
		else
		{
			// ���܂����m�[�h�Ő��_�J�n
			selectNode = selectNode->Inference(data);
		}
	}

	return selectNode;
}

// �D�揇�ʂŃm�[�h�I��
NodeBase* NodeBase::SelectPriority(std::vector<NodeBase*>* candidateList)
{
	NodeBase* selectNode  = nullptr;
	unsigned int priority = UINT_MAX;

	// TODO 04_04 ��ԗD�揇�ʂ�����(�l��������)�m�[�h��T����selectNode�Ɋi�[
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


// �����_���Ńm�[�h�I��
NodeBase* NodeBase::SelectRandom(std::vector<NodeBase*>* candidateList)
{
	// TODO 04_05 list�̃T�C�Y�ŗ������擾����selectNo�Ɋi�[
	const size_t lastNode = (*candidateList).size() - 1;
	const size_t selectNo  = static_cast<size_t>(std::rand()) % lastNode;

	// list��selectNo�Ԗڂ̎��Ԃ����^�[��
	return (*candidateList).at(selectNo);
}

// �V�[�P���X�E�V�[�P���V�������[�s���O�Ńm�[�h�I��
NodeBase* NodeBase::SelectSequence(
	std::vector<NodeBase*>* candidateList,
	BehaviorData* data)
{
	int step = 0;

	// �w�肳��Ă��钆�ԃm�[�h�̃V�[�P���X���ǂ��܂Ŏ��s���ꂽ���擾����
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
	using Iterator = std::vector<NodeBase*>::iterator;
	for (Iterator itr = candidateList->begin(); itr != candidateList->end(); ++itr)
	{
		// �q�m�[�h�����s�\���X�g�Ɋ܂܂�Ă��邩
		NodeBase* childNode		= children_.at(step);
		NodeBase* condidateNode = (*itr);
		if (childNode->GetName() == condidateNode->GetName())
		{
			// TODO 04_06 ���݂̎��s�m�[�h�̕ۑ��A���Ɏ��s����X�e�b�v�̕ۑ����s������A
			// ���݂̃X�e�b�v�ԍ��̃m�[�h�����^�[�����Ȃ���
			// �@�X�^�b�N�ɂ�data->PushSequenceNode�֐����g�p����B�ۑ�����f�[�^�͎��s���̒��ԃm�[�h�B
			data->PushSequenceNode(this);

			// �A�܂��A���Ɏ��s���钆�ԃm�[�h�ƃX�e�b�v����ۑ�����
			// �@�ۑ��ɂ�data->SetSequenceStep�֐����g�p�B
			// �@�ۑ��f�[�^�͒��ԃm�[�h�̖��O�Ǝ��̃X�e�b�v���ł�(step + 1)
			data->SetSequenceStep(childNode->GetName(), step + 1);

			// �B�X�e�b�v�ԍ��ڂ̎q�m�[�h�����s�m�[�h�Ƃ��ă��^�[������
			return childNode;
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
		return judgment_->Judgment();
	}

	return true;
}

// �m�[�h���s
const ActionBase::State NodeBase::Run(const float elapsedTime)
{
	// TODO 04_08 action�����邩���f�B����΃����o�֐�Run()���s�������ʂ����^�[���B
	if (action_ != nullptr)
	{
		return action_->Run(elapsedTime);
	}

	return ActionBase::State::Failed;
}
