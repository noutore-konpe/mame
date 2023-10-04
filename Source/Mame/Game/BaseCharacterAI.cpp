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
//    // 現在実行するノードがあれば、ビヘイビアツリーからノードを実行
//    if (activeNode_ != nullptr)
//    {
//        //activeNode_ = behaviorTree_->Run(activeNode_, behaviorData_.get(), elapsedTime);
//        activeNode_.release();      // リソースの所有権の放棄
//        // (リソースを開放して)新たなリソースの設定
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
//    // 現在実行されているノードがなければ、次に実行するノードを推論する
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
//// ImGui用
//void BaseCharacterAI::DrawDebug()
//{
//#ifdef USE_IMGUI
//    if (ImGui::BeginMenu(GetName()))
//    {
//        std::string str = (activeNode_ != nullptr)
//                        ? activeNode_->GetName()
//                        : u8"なし";
//        ImGui::Text(u8"Behavior：%s", str.c_str());
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
