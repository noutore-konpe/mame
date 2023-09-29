#include "BehaviorTree.h"

#include "../../Taki174/Common.h"

#include "ActionBase.h"
#include "NodeBase.h"
#include "JudgmentBase.h"
//#include "EnemyBlueSlime.h"
#include "BehaviorData.h"


void BehaviorTree::AddNode(
	const std::string& parentName,
	const std::string& entryName,
	const int priority,
	const SelectRule& selectRule,
	JudgmentBase* judgment,
	ActionBase* action)
{
	using std::make_unique;

	if (parentName != "")
	{
		NodeBase* parentNode = root_->SearchNode(parentName);

		if (parentNode != nullptr)
		{
			NodeBase* sibling = parentNode->GetLastChild();
			NodeBase* addNode = {
				new NodeBase(
					entryName, parentNode, sibling,
					priority, selectRule,
					judgment, action,
					parentNode->GetHirerchyNo() + 1)
			};

			parentNode->AddChild(addNode);
		}
		else
		{
			assert("Parent Node is not found (or wrong parent's name)");
		}
	}
	else
	{
		if (nullptr == root_)
		{
			//root_ = new NodeBase(entryName, nullptr, nullptr, priority, selectRule, judgment, action, 1);
			root_ = {
				make_unique<NodeBase>(
				entryName, nullptr, nullptr,
				priority, selectRule,
				judgment, action, 1)
			};
		}
	}
}

// ���_
NodeBase* BehaviorTree::ActiveNodeInference(BehaviorData* data)
{
	// �f�[�^�����Z�b�g���ĊJ�n
	data->Init();
	return root_->Inference(data);
}

// �V�[�P���X�m�[�h����̐��_�J�n
NodeBase* BehaviorTree::SequenceBack(
	NodeBase* sequenceNode,
	BehaviorData* data)
{
	return sequenceNode->Inference(data);
}

// �m�[�h���s
NodeBase* BehaviorTree::Run(
	NodeBase*		actionNode,
	BehaviorData*	data,
	const float		elapsedTime)
{
	using State = ActionBase::State;

	// �m�[�h���s
	State state = actionNode->Run(elapsedTime);

	// ����I��
	if (State::Complete == state)
	{
		// �V�[�P���X�̓r�����𔻒f
		NodeBase* sequenceNode = data->PopSequenceNode();

		// �r������Ȃ��Ȃ�I��
		if (nullptr == sequenceNode)
		{
			return nullptr;
		}
		else
		{
			// �r���Ȃ炻������n�߂�
			return SequenceBack(sequenceNode, data);
		}
		// ���s�͏I��
	}
	else if (State::Failed == state)
	{
		return nullptr;
	}

	// ����ێ�
	return actionNode;
}

// �o�^���ꂽ�m�[�h��S�č폜����
void BehaviorTree::NodeAllClear(NodeBase* delNode)
{
	const size_t count = delNode->children_.size();
	if (count > 0)
	{
		for (NodeBase* node : delNode->children_)
		{
			NodeAllClear(node);
		}
		SafeDeletePtr(delNode);
	}
	else
	{
		SafeDeletePtr(delNode);
	}
}

