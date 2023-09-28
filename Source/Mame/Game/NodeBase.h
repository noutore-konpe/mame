#pragma once

#include <vector>
#include <string>
#include "BehaviorTree.h"
#include "ActionBase.h"

class JudgmentBase;
class BehaviorData;

// ���������[�N�����p
#define debug_new new(_NORMAL_BLOCK,__FILE__,__LINE__)

// �m�[�h
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
	// ���O�Q�b�^�[
	const std::string& GetName() const { return name_; }

	// �e�m�[�h�Q�b�^�[
	NodeBase* GetParent() const { return parent_; }
	// �q�m�[�h�Q�b�^�[
	NodeBase* GetChild(const int index) const;
	// �q�m�[�h�Q�b�^�[(����)
	NodeBase* GetLastChild() const;
	// �q�m�[�h�Q�b�^�[(�擪)
	NodeBase* GetTopChild() const;
	// �Z��m�[�h�Q�b�^�[
	NodeBase* GetSibling() const { return sibling_; }

	// �K�w�ԍ��Q�b�^�[
	const int GetHirerchyNo() const { return hierarchyNo_; }
	// �D�揇�ʃQ�b�^�[
	const int GetPriority() const { return priority_; }

public:
	// �e�m�[�h�Z�b�^�[
	void SetParent(NodeBase* parent) { this->parent_ = parent; }
	// �Z��m�[�h�Z�b�^�[
	void SetSibling(NodeBase* sibling) {this->sibling_ = sibling;}

public:
	// �q�m�[�h�ǉ�
	void AddChild(NodeBase* child) { children_.push_back(child); }

public:
	// �s���f�[�^�������Ă��邩
	const bool HasAction() const { return (action_ != nullptr); }
	// ���s�۔���
	const bool Judgment() const;

public:
	// �D�揇�ʑI��
	NodeBase* SelectPriority(std::vector<NodeBase*>* list);
	// �����_���I��
	NodeBase* SelectRandom(std::vector<NodeBase*>* list);
	// �V�[�P���X�I��
	NodeBase* SelectSequence(std::vector<NodeBase*>* list, BehaviorData* data);

public:
	// �m�[�h����
	NodeBase* SearchNode(const std::string& searchName);
	// �m�[�h���_
	NodeBase* Inference(BehaviorData* data);
	// ���s
	const ActionBase::State Run(const float elapsedTime);

public:
	std::vector<NodeBase*>		children_;		// �q�m�[�h

protected:
	std::string					name_;			// ���O
	BehaviorTree::SelectRule	selectRule_;	// �I�����[��
	JudgmentBase*				judgment_;		// ����N���X
	ActionBase*					action_;		// ���s�N���X
	unsigned int				priority_;		// �D�揇��
	NodeBase*					parent_;		// �e�m�[�h
	NodeBase*					sibling_;		// �Z��m�[�h
	int							hierarchyNo_;	// �K�w�ԍ�

};