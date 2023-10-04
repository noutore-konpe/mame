#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>

#include "../Transform.h"

#include "../Resource/skinned_mesh.h"

class Model
{
public:
    Model(ID3D11Device* device, const char* fbx_filename, const char* psFilename=nullptr, bool triangulate = true, float sampling_rate = 0);
    Model(ID3D11Device* device, const char* fbx_filename, std::vector<std::string>& animation_filenames, bool triangulate = true, float sampling_rate = 0);
    ~Model() {}


    void Render(const float& scale, int rasterizeState = 0);
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr, int rasterizeState = 0);
     
    void DrawDebug();

    Transform* GetTransform() { return &transform; }
    
    void SetModelColor(DirectX::XMFLOAT4 c) { color = c; }
    DirectX::XMFLOAT4 GetModelColor() { return color; }
    
public: // �擾�E�ݒ�֐��֘A
    // �A�j���[�V�����f�[�^�擾
    std::vector<animation>* GetAnimation() { return &skinned_meshes->animation_clips; };

    // ���݂̃A�j���[�V�����ԍ��̎擾�E�ݒ�
    const int GetCurrentAnimationIndex()& { return currentAnimationIndex; }
    void SetCurrentAnimationIndex(const int& animationIndex) { currentAnimationIndex = animationIndex; }

    // �A�j���[�V�����Đ����x�ݒ�i�r���ōĐ����x��ς������Ƃ��ȂǂɁj
    void SetAnimationSpeed(const float& speed) { animationSpeed = speed; }

public: // �A�j���[�V�����֐��֘A
    // �A�j���[�V�����Đ��ݒ�
    // (�A�j���[�V�����ԍ��E���[�v���邩�ǂ����E�A�j���[�V�����Đ����x�E�X���[�Y�؂�ւ����ԁi���x�j)
    void PlayAnimation(
        const int&   index,
        const bool&  loop,
        const float& speed        = 1.0f,
        const float& blendSeconds = 1.0f
    );

    void PlayBlendAnimation(
        const int& index1,
        const int& index2,
        const bool& loop,
        const float& speed = 1.0f
    );

    // �u�����h���̌v�Z�X�V����
    void UpdateBlendRate(float blendRate, const float& elapsedTime);

    // �A�j���[�V�����X�V����
    void UpdateAnimation(const float& elapsedTime);

    //�u�����h�A�j���[�V�����X�V�����@�X�V����true��Ԃ�
    bool UpdateBlendAnimation(const float& elapsedTime);

    // �A�j���[�V�������Đ������ǂ���
    bool IsPlayAnimation() const;


public: // �f�o�b�O�m�F�p
    bool isDebugBlendAnimation = true;     // �A�j���[�V�����u�����h�I���I�t

public:
    std::shared_ptr<skinned_mesh> skinned_meshes;
    Transform    transform;

    animation::keyframe keyframe = {};
    DirectX::XMFLOAT4 color = { 1,1,1,1 };

    //�A�j���[�V�����u�����h�̃u�����h��
    float       weight{};
    float       animationSpeed          = 1.0f;     // �A�j���[�V�����Đ����x
private:
    float       currentAnimationSeconds = 0.0f;     // ���݂̃A�j���[�V�����Đ�����

    float       animationBlendTime      = 0.0f;     // ���݂̃u�����h�^�C�}�[
    float       animationBlendSeconds   = 0.0f;     // �u�����h����

    int         currentAnimationIndex   = -1;	    // ���݂̃A�j���[�V�����ԍ�

    int         blendAnimationIndex1   = -1;	    // ��ڃu�����h�p�A�j���[�V�����ԍ�
    int         blendAnimationIndex2   = -1;	    // ��ڃu�����h�p�A�j���[�V�����ԍ�
    float       blendAnimationSeconds = 0.0f;     // ���݂̃A�j���[�V�����Đ�����

    bool        animationLoopFlag       = false;    // �A�j���[�V���������[�v�Đ����邩
    bool        animationEndFlag        = false;    // �A�j���[�V�������I��������

    float       blendThreshold = 0.0f; //�u�����h�A�j���[�V�����̃x�[�X�ƂȂ�A�j���[�V�����̐؂�ւ��������l

    float blendRate;
};

