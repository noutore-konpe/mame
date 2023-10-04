#include "Model.h"
#include "ResourceManager.h"
#include "Graphics.h"

// TODO: ���\�[�X�}�l�[�W���[�̎g�p
#define USE_RESOURCE_MANAGER

Model::Model(ID3D11Device* device, const char* fbx_filename, const char* psFilename, bool triangulate, float sampling_rate)
{   
#ifdef USE_RESOURCE_MANAGER // ���\�[�X�}�l�[�W���[����
    skinned_meshes = ResourceManager::Instance().LoadModelResource(
        device, 
        fbx_filename, 
        psFilename,
        triangulate, 
        sampling_rate
    );
#else // ���\�[�X�}�l�[�W���[�Ȃ�
    skinned_meshes = std::make_unique<skinned_mesh>(device, fbx_filename, triangulate, sampling_rate);
#endif

}

Model::Model(ID3D11Device* device, const char* fbx_filename, std::vector<std::string>& animation_filenames, bool triangulate, float sampling_rate)
{
    skinned_meshes = ResourceManager::Instance().LoadModelResource(
        device, fbx_filename, 
        animation_filenames, 
        triangulate, 
        sampling_rate
    );
}

// �`��
void Model::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    // world�s��X�V
    DirectX::XMFLOAT4X4 world = {};
    DirectX::XMStoreFloat4x4(&world, GetTransform()->CalcWorldMatrix(scale));

    // Model�`��
    if (&keyframe)
    {
        skinned_meshes->render(graphics.GetDeviceContext(), world, GetModelColor(), &keyframe, psShader);
    }
    else
    {
        skinned_meshes->render(graphics.GetDeviceContext(), world, GetModelColor(), nullptr, psShader);
    }
}

void Model::DrawDebug()
{
    GetTransform()->DrawDebug();

    if (ImGui::TreeNode("Animation"))
    {
        ImGui::SliderFloat("weight", &weight, 0.0f, 1.0f);
        ImGui::SliderFloat("threshold", &blendThreshold, 0.0f, 1.0f);
        ImGui::InputInt("Current Index", &currentAnimationIndex);

        ImGui::SliderFloat("Animation Speed", &animationSpeed, 0.0f, 3.0f);

        ImGui::SliderFloat("BlendRate", &blendRate, 0.0f, 1.0f);

        ImGui::TreePop();
    }
}


void Model::PlayAnimation(
    const int& index, const bool& loop,
    const float& speed, const float& blendSeconds)
{
    // �ݒ�p�̃A�j���[�V�����ԍ������݂̃A�j���[�V�����ԍ��Ɠ����ꍇ��return
    if (currentAnimationIndex == index) return;

    currentAnimationIndex   = index;    // �Đ�����A�j���[�V�����ԍ���ݒ�
    currentAnimationSeconds = 0.0f;     // �A�j���[�V�����Đ����ԃ��Z�b�g

    animationLoopFlag       = loop;     // ���[�v�����邩
    animationEndFlag        = false;    // �Đ��I���t���O�����Z�b�g

    animationSpeed          = speed;    // �A�j���[�V�����Đ����x

    animationBlendTime      = 0.0f;
    animationBlendSeconds   = blendSeconds;
}

void Model::PlayBlendAnimation(const int& index1, const int& index2, const bool& loop, const float& speed)
{
    // �ݒ�p�̃A�j���[�V�����ԍ������݂̃A�j���[�V�����ԍ��Ɠ����ꍇ��return
    if (blendAnimationIndex1 == index1 && blendAnimationIndex2 == index2) return;

    blendAnimationIndex1 = index1;    // �Đ�����A�j���[�V�����ԍ���ݒ�
    blendAnimationIndex2 = index2;    // �Đ�����A�j���[�V�����ԍ���ݒ�
    blendAnimationSeconds = 0.0f;     // �A�j���[�V�����Đ����ԃ��Z�b�g

    animationLoopFlag = loop;     // ���[�v�����邩
    animationEndFlag = false;    // �Đ��I���t���O�����Z�b�g

    animationSpeed = speed;    // �A�j���[�V�����Đ����x

    animationBlendTime = 0.0f;
}


void Model::UpdateBlendRate(float blendRate, const float& elapsedTime)
{
    if (animationBlendTime < animationBlendSeconds)
    {
        animationBlendTime += elapsedTime;
        if (animationBlendTime >= animationBlendSeconds)
        {
            animationBlendTime = animationBlendSeconds;
        }
        blendRate = animationBlendTime / animationBlendSeconds;
        blendRate *= blendRate;
    }
}


void Model::UpdateAnimation(const float& elapsedTime)
{
    // �Đ����łȂ��Ȃ珈�����Ȃ�
    if (!IsPlayAnimation()) return;

    // �ŏI�t���[������(�Đ��I���t���O�������Ă���΍Đ��I��)
    if (animationEndFlag)
    {
        animationEndFlag = false; // �I���t���O�����Z�b�g
        currentAnimationIndex = -1;    // �A�j���[�V�����ԍ����Z�b�g
        return;
    }

    //�u�����h�A�j���[�V�����̍Đ��i�Đ����̏ꍇ����ȍ~�̒ʏ�Đ��̏����͂��Ȃ��j
    if (UpdateBlendAnimation(elapsedTime))return;

    // �A�j���[�V�����Đ����Ԍo��
    currentAnimationSeconds += elapsedTime;

    // �w��̃A�j���[�V�����f�[�^���擾
    animation& animation = GetAnimation()->at(currentAnimationIndex);

    // ���݂̃t���[�����擾
    const float  frameIndex_float = (currentAnimationSeconds * animation.sampling_rate) * animationSpeed; // �x��������܂Ȃ̂ňꎞ�I��float�ϐ��Ɋi�[
    const size_t frameIndex = static_cast<const size_t>(frameIndex_float);

    // �Ō�̃t���[�����擾
    const size_t frameEnd = (animation.sequence.size() - 1);


    // �A�j���[�V�������Đ��������Ă����ꍇ
    if (frameIndex > frameEnd)
    {
        // ���[�v�t���O�������Ă���΍Đ����Ԃ������߂�
        if (animationLoopFlag)
        {
            currentAnimationSeconds = 0.0f;
            return;
        }
        // ���[�v�Ȃ��Ȃ�Đ��I���t���O�𗧂Ă�
        else
        {
            animationEndFlag = true;
            return;
        }
    }
    // �L�[�t���[�����X�V����Ă��ăA�j���[�V�������Đ��������Ă��Ȃ��Ƃ��̓A�j���[�V�������X���[�Y�ɐ؂�ւ���
    else if (isDebugBlendAnimation && (keyframe.nodes.size() > 0) && frameIndex < frameEnd)
    {
        // �u�����h���̌v�Z
        float blendRate = 1.0f;
        UpdateBlendRate(blendRate, elapsedTime);

        // �L�[�t���[���擾
        const std::vector<animation::keyframe>& keyframes = animation.sequence;

        // ���݂̑O��̃L�[�t���[�����擾
        const animation::keyframe* keyframeArr[2] = {
            &keyframe,
            &keyframes.at(frameIndex + 1)
        };

        // �A�j���[�V���������炩�ɐ؂�ւ���
        skinned_meshes->blend_animations(keyframeArr, blendRate, keyframe);

        // �A�j���[�V�����g�����X�t�H�[���X�V
        skinned_meshes->update_animation(keyframe);
    }
    // �L�[�t���[������x���X�V����Ă��Ȃ��ăA�j���[�V�������Đ��������Ă��Ȃ���Ό��݂̃t���[����ۑ�
    else
    {
        keyframe = animation.sequence.at(frameIndex);
    }
}

bool Model::UpdateBlendAnimation(const float& elapsedTime)
{
    if(blendAnimationIndex1 < 0)return false;

    if (animationEndFlag)
    {
        animationEndFlag = false; // �I���t���O�����Z�b�g
        blendAnimationIndex1 = -1;    // �A�j���[�V�����ԍ����Z�b�g
        blendAnimationIndex2 = -1;
        return false;
    }

    //�A�j���[�V�����Đ����ԍX�V
    blendAnimationSeconds += elapsedTime;

    animation& bAnimation1 = GetAnimation()->at(blendAnimationIndex1);
    animation& bAnimation2 = GetAnimation()->at(blendAnimationIndex2);

    const size_t frameCount1 = bAnimation1.sequence.size();
    const size_t frameCount2 = bAnimation2.sequence.size();

    //��ƂȂ�A�j���[�V�����Ƀt���[���������킹��
    size_t maxFrameCount = frameCount1;
    if (blendThreshold < weight)maxFrameCount = frameCount2;

    const float frameIndex = (blendAnimationSeconds * bAnimation1.sampling_rate) * animationSpeed;

    //�A�j���[�V�������Đ��������Ă���ꍇ
    if (frameIndex > maxFrameCount)
    {
        if (animationLoopFlag)
        {
            blendAnimationSeconds = 0.0f;
            return true;
        }
        else
        {
            animationEndFlag = true;
            return true;
        }
    }

    //�Đ��t���[���𐳋K�����čĐ����Ԃ̒��������킹��
    UINT64 frameIndex1 = static_cast<UINT64>(frameIndex / maxFrameCount * (frameCount1 - 1));
    UINT64 frameIndex2 = static_cast<UINT64>(frameIndex / maxFrameCount * (frameCount2 - 1));

#if 0

    const animation::keyframe* keyframeArr[2] = {
             &bAnimation1.sequence.at(frameIndex1),
             &bAnimation2.sequence.at(frameIndex2)
    };
    animation::keyframe keyframe1;
    skinned_meshes->blend_animations(keyframeArr, weight, keyframe1);

    //�t���[����Ԃ��邽�߂̃L�[�t���[��
    const animation::keyframe* lerpKeyframeArr[2] = {
             &bAnimation1.sequence.at(frameIndex1 + 1),
             &bAnimation2.sequence.at(frameIndex2 + 1)
    };
    animation::keyframe keyframe2;
    skinned_meshes->blend_animations(lerpKeyframeArr, weight, keyframe2);

    // �u�����h���̌v�Z
    blendRate = frameIndex - static_cast<int>(frameIndex);


    const animation::keyframe* resultKeyframeArr[2] = { &keyframe1 ,&keyframe2 };
    skinned_meshes->blend_animations(resultKeyframeArr, blendRate, keyframe);

#else//�Ȃ񂩏�肭�ł����̂ŕʃo�[�W����

    blendRate = 1.0f;
    if (animationBlendTime < animationBlendSeconds)
    {
        animationBlendTime += elapsedTime;
        if (animationBlendTime >= animationBlendSeconds)
        {
            animationBlendTime = animationBlendSeconds;
        }
        blendRate = animationBlendTime / animationBlendSeconds;
        blendRate *= blendRate;
    }

    // �L�[�t���[���擾
    const std::vector<animation::keyframe>& keyframes1 = bAnimation1.sequence;

    // ���݂̑O��̃L�[�t���[�����擾
    const animation::keyframe* keyframeArr1[2] = {
        &keyframes1.at(frameIndex1),
        &keyframes1.at(frameIndex1 + 1)
    };

    // �A�j���[�V���������炩�ɐ؂�ւ���
    animation::keyframe keyframe1;
    skinned_meshes->blend_animations(keyframeArr1, blendRate, keyframe1);

    // �L�[�t���[���擾
    const std::vector<animation::keyframe>& keyframes2 = bAnimation2.sequence;

    // ���݂̑O��̃L�[�t���[�����擾
    const animation::keyframe* keyframeArr2[2] = {
        &keyframes2.at(frameIndex2),
        &keyframes2.at(frameIndex2 + 1)
    };

    // �A�j���[�V���������炩�ɐ؂�ւ���
    animation::keyframe keyframe2;
    skinned_meshes->blend_animations(keyframeArr2, blendRate, keyframe2);

    const animation::keyframe* resultKeyframeArr[2] = { &keyframe1 ,&keyframe2 };
    skinned_meshes->blend_animations(resultKeyframeArr, weight, keyframe);
#endif // 0



    skinned_meshes->update_animation(keyframe);

    return true;
}


bool Model::IsPlayAnimation() const
{
    if (currentAnimationIndex < 0 && blendAnimationIndex1 < 0) return false;

    const int animationIndexEnd = static_cast<int>(skinned_meshes->animation_clips.size());
    if (currentAnimationIndex >= animationIndexEnd && blendAnimationIndex1 >= animationIndexEnd) return false;

    return true;
}
