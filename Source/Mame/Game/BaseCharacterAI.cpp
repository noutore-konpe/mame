//#include "BaseCharacterAI.h"
//
//#include "../../Taki174/Common.h"
//
//
//BaseCharacterAI::~BaseCharacterAI()
//{
//    //SafeDeletePtr(activeNode_);
//}
//
//void BaseCharacterAI::Update(const float elapsedTime)
//{
//    using std::make_unique;
//
//    // ���ݎ��s����m�[�h������΁A�r�w�C�r�A�c���[����m�[�h�����s
//    if (activeNode_ != nullptr)
//    {
//        //activeNode_ = behaviorTree_->Run(activeNode_, behaviorData_.get(), elapsedTime);
//        activeNode_.release();      // ���\�[�X�̏��L���̕���
//        // (���\�[�X���J������)�V���ȃ��\�[�X�̐ݒ�
//        activeNode_.reset(behaviorTree_->Run(
//            activeNode_.get(),
//            behaviorData_.get(),
//            elapsedTime)
//        );
//        //activeNode_ = make_unique<NodeBase>(
//        //    behaviorTree_->Run(
//        //        activeNode_.get(),
//        //        behaviorData_.get(),
//        //        elapsedTime)
//        //);
//    }
//    // ���ݎ��s����Ă���m�[�h���Ȃ���΁A���Ɏ��s����m�[�h�𐄘_����
//    else
//    {
//        activeNode_.release();
//        activeNode_.reset(behaviorTree_->ActiveNodeInference(
//            behaviorData_.get())
//        );
//        //activeNode_ = make_unique<NodeBase>(
//        //    behaviorTree_->ActiveNodeInference(
//        //        behaviorData_.get())
//        //);
//    }
//
//}
//
//void BaseCharacterAI::Render(const float elapsedTime, const float scale)
//{
//    //Character::Render(elapsedTime, scale);
//}
//
//// ImGui�p
//void BaseCharacterAI::DrawDebug()
//{
//#ifdef USE_IMGUI
//    if (ImGui::BeginMenu(GetName()))
//    {
//        std::string str = (activeNode_ != nullptr)
//                        ? activeNode_->GetName()
//                        : u8"�Ȃ�";
//        ImGui::Text(u8"Behavior�F%s", str.c_str());
//
//        Character::DrawDebug();
//
//        float range = GetRange();
//        ImGui::DragFloat("range", &range);
//        SetRange(range);
//
//        ImGui::EndMenu();
//    }
//#endif // USE_IMGUI
//}
