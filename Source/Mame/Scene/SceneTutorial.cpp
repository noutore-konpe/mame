#include "SceneTutorial.h"

#include "../../Taki174/NumeralManager.h"

#include "../Input/Input.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/EffectManager.h"

#include "../Other/misc.h"

#include "../Resource/texture.h"

#include "../Game/PlayerManager.h"
#include "../Game/ItemManager.h"
#include "../Game/Book.h"
#include "../Game/AbilityManager.h"
#include "../Game/BlackHole.h"

#include "../Game/EnemyManager.h"
#include "../Game/EnemyTestAI.h"
#include "../Game/EnemyAI_1.h"
#include "../Game/EnemyAI_2.h"
#include "../Game/EnemyAI_3.h"

#include "../Game/ProjectileManager.h"
#include "../Game/ProjectileHorming.h"

#include "../Game/ExperiencePointManager.h"

#include "../Game/UserInterface.h"

#include "../Game/TutorialManager.h"

#include "../Game/SlowMotionManager.h"

#include "../framework.h"

#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneGame.h"

DirectX::XMFLOAT3 SceneTutorial::stageCenter = {};

// ���\�[�X����
void SceneTutorial::CreateResource()
{
    Graphics& graphics = Graphics::Instance();

    // stage
    stageBase = std::make_unique<Stage>();
    stageWall = std::make_unique<Stage>(
        "./Resources/Model/Stage/stageWall.fbx",
        "./Resources/Shader/StageWallPS.cso"
    );

    // player
    PlayerManager::Instance().GetPlayer() = std::make_unique<Player>();


    // graphics
    {
        framebuffers[0] = std::make_unique<FrameBuffer>(graphics.GetDevice(), 1280, 720);
        bitBlockTransfer = std::make_unique<FullscreenQuad>(graphics.GetDevice());

        // bloom
        bloomer = std::make_unique<Bloom>(graphics.GetDevice(), 1280 / 1, 720 / 1);
        CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/finalPassPs.cso", bloomPS.GetAddressOf());

        // fog
        framebuffers[1] = std::make_unique<FrameBuffer>(graphics.GetDevice(), 1280 / 1, 720 / 1, false);
        CreatePsFromCso(graphics.GetDevice(), "./Resources/Shader/FogPS.cso", fogPS.GetAddressOf());

        // shadow
        shadow.shadowMap = std::make_unique<ShadowMap>(graphics.GetDevice(),
            shadow.shadowMapWidth, shadow.shadowMapHeight);

        HRESULT hr{ S_OK };
        D3D11_BUFFER_DESC desc{};
        desc.ByteWidth = sizeof(Shader::SceneConstants);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        hr = graphics.GetDevice()->CreateBuffer(&desc, nullptr, shadowConstantBuffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

        // �萔�o�b�t�@�[
        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.ByteWidth = sizeof(Shader::SceneConstants);
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;
        // SceneConstants
        hr = graphics.GetDevice()->CreateBuffer(&bufferDesc, nullptr,
            ConstantBuffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

}

// ������
void SceneTutorial::Initialize()
{
    // player
    PlayerManager::Instance().Initialize();

    // �J����
    Camera::Instance().Initialize();

    // item
    ItemManager::Instance().Initialize();

    // enemy
    EnemyManager& enemyManager = EnemyManager::Instance();
    enemyManager.Initialize();
    //enemyAura->Initialize();

    // stage
    stageBase->Initialize();
    stageCenter = stageBase->GetTransform()->GetPosition();
    stageWall->Initialize();

    // Exp
    ExperiencePointManager& expManager = ExperiencePointManager::Instance();
    expManager.Initialize();

    UserInterface::Instance().Initialize();

    // �`���[�g���A��������
    TutorialManager::Instance().Initialize();
}

void SceneTutorial::Finalize()
{
    // Exp
    ExperiencePointManager& expManager = ExperiencePointManager::Instance();
    expManager.Finalize();

    EnemyManager& enemyManager = EnemyManager::Instance();
    enemyManager.Finalize();

    ItemManager::Instance().Finalize();

    PlayerManager::Instance().Finalize();
}

void SceneTutorial::Begin()
{
}

void SceneTutorial::Update(const float& elapsedTime)
{
    Mame::Scene::SceneManager& sceneManager = Mame::Scene::SceneManager::Instance();
    TutorialManager&    tutorialManager = TutorialManager::Instance();
    PlayerManager&      plManager       = PlayerManager::Instance();
    EnemyManager&       enemyManager    = EnemyManager::Instance();
    NumeralManager&     numeralManager  = NumeralManager::Instance();
    UserInterface&      userInterface   = UserInterface::Instance();
    SlowMotionManager&  slowMotion      = SlowMotionManager::Instance();
    GamePad&            gamePad         = Input::Instance().GetGamePad();

    //�J�[�h���o���͂ق��̏������X�V���Ȃ�
    Player* player = plManager.GetPlayer().get();
    if (true == player->isSelectingSkill)
    {
        player->SelectSkillUpdate(elapsedTime);
        return;
    }

    // �X���[���[�V�����X�V
    slowMotion.Update(elapsedTime);

    // �X���[���[�V������K�p�����o�ߎ���
    const float slowMotionElapsedTime = {
        (true == slowMotion.GetSlowMotionFlag())
        ? elapsedTime * slowMotion.GetCurrentPercentage()
        : elapsedTime
    };

    // �`���[�g���A���}�l�[�W���[�X�V
    {
        // �`���[�g���A���X�V
        const bool tutorialExecuteFlag = tutorialManager.Update(slowMotionElapsedTime);

        // �`���[�g���A�����s���t���O��false�Ȃ�`���[�g���A�����I������
        if (false == tutorialExecuteFlag)
        {
            // �Q�[���V�[���Ɉڂ�
            sceneManager.ChangeScene(new SceneLoading(new SceneGame));
            return;
        }
    }

    // �v���C���[�X�V
    plManager.Update(slowMotionElapsedTime);

    // �J�����X�V
    Camera::Instance().Update(slowMotionElapsedTime);

    // �A�C�e���X�V
    ItemManager::Instance().Update(slowMotionElapsedTime);

    // �G�l�~�[�X�V
    enemyManager.Update(slowMotionElapsedTime);

    // �o���l�X�V
    ExperiencePointManager& expManager = ExperiencePointManager::Instance();
    expManager.Update(slowMotionElapsedTime);

    // �G�t�F�N�g�X�V
    EffectManager::Instance().Update(slowMotionElapsedTime);

    // UI
    {
        // �����\���X�V
        numeralManager.Update(slowMotionElapsedTime);

        // UI�X�V
        userInterface.Update(slowMotionElapsedTime);
    }

    //�J�[�h���o������Update�O��return�Ă�ł邩�璍�ӁI�I
}

void SceneTutorial::End()
{
}

void SceneTutorial::Render(const float& /*elapsedTime*/)
{
    Graphics& graphics = Graphics::Instance();
    Shader* shader = graphics.GetShader();

    Shader::SceneConstants sceneConstants{};

    // �`��̏����ݒ�
    {
        // �`��̏����ݒ聦�K���ĂԂ��ƁI�I�I
        Mame::Scene::BaseScene::RenderInitialize();


        // SHADOW : make shadow map
        {
            const float aspectRatio = shadow.shadowMap->viewport.Width / shadow.shadowMap->viewport.Height;
            DirectX::XMVECTOR F =
            {
                DirectX::XMLoadFloat4(&shadow.lightViewFocus)
            };
            DirectX::XMVECTOR E =
            {
                DirectX::XMVectorSubtract(F,
                DirectX::XMVectorScale(
                    DirectX::XMVector3Normalize(
                        DirectX::XMLoadFloat4(&graphics.GetShader()->view.position)),shadow.lightViewDistance))
            };
            DirectX::XMVECTOR U =
            {
                DirectX::XMVectorSet(0.0f,1.0f,0.0f,0.0f)
            };
            DirectX::XMMATRIX V =
            {
                DirectX::XMMatrixLookAtLH(E,F,U)
            };
            DirectX::XMMATRIX P =
            {
                DirectX::XMMatrixOrthographicLH(shadow.lightViewSize * aspectRatio,
                shadow.lightViewSize,shadow.lightViewNearZ,shadow.lightViewFarZ)
            };

            DirectX::XMStoreFloat4x4(&sceneConstants.viewProjection, V * P);
            sceneConstants.lightViewProjection = sceneConstants.viewProjection;
            graphics.GetDeviceContext()->UpdateSubresource(shadowConstantBuffer.Get(), 0, 0, &sceneConstants, 0, 0);
            graphics.GetDeviceContext()->VSSetConstantBuffers(1, 1, shadowConstantBuffer.GetAddressOf());

            shadow.shadowMap->Clear(graphics.GetDeviceContext(), 1.0f);
            shadow.shadowMap->Activate(graphics.GetDeviceContext());

            // �X�e�[�g�Z�b�g
            shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_ON_ZW_ON));
            shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::NONE));
            shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::SOLID));

            // SHADOW : �e���������f���͂�����Render����
            {
            }

            shadow.shadowMap->Deactivete(graphics.GetDeviceContext());
        }
    }

    Camera& camera = Camera::Instance();

    shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_ON_ZW_ON));
    shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::NONE));
    shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::SOLID));

    camera.SetPerspectiveFov(graphics.GetDeviceContext());
    DirectX::XMStoreFloat4x4(&sceneConstants.viewProjection, camera.GetViewMatrix() * camera.GetProjectionMatrix());
    sceneConstants.lightDirection = shader->GetViewPosition();
    sceneConstants.cameraPosition = shader->GetViewCamera();

    DirectX::XMStoreFloat4x4(&sceneConstants.inverseProjection, DirectX::XMMatrixInverse(NULL, camera.GetProjectionMatrix()));
    DirectX::XMStoreFloat4x4(&sceneConstants.inverseViewProjection, DirectX::XMMatrixInverse(NULL, camera.GetViewMatrix() * camera.GetProjectionMatrix()));
    sceneConstants.time = framework::tictoc.time_stamp();

    graphics.GetDeviceContext()->UpdateSubresource(ConstantBuffer.Get(), 0, 0, &sceneConstants, 0, 0);
    graphics.GetDeviceContext()->VSSetConstantBuffers(1, 1, ConstantBuffer.GetAddressOf());
    graphics.GetDeviceContext()->PSSetConstantBuffers(1, 1, ConstantBuffer.GetAddressOf());

    {
        // �J�����֌W
        RenderContext rc = {};
        rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };

        shader->Begin(graphics.GetDeviceContext(), rc);
    }

    // EMISSIVE
    //graphics.GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());

    // SHADOW : bind shadow map at slot 8
    graphics.GetDeviceContext()->PSSetShaderResources(8, 1, shadow.shadowMap->shaderResourceView.GetAddressOf());

    framebuffers[0]->Clear(graphics.GetDeviceContext());
    framebuffers[0]->Activate(graphics.GetDeviceContext());

    // ���f���`��
    {
        // stage
        {
            stageBase->Render(0.01f);
            stageWall->Render(0.01f);
        }

        // player
        {
            PlayerManager::Instance().Render(0.01f);
        }

        // Exp
        ExperiencePointManager& expManager = ExperiencePointManager::Instance();
        expManager.Render(1.0f);

    }

    // 3D�G�t�F�N�g�`��
    {
        Camera& camera = Camera::Instance();
        DirectX::XMFLOAT4X4 view, projection;
        DirectX::XMStoreFloat4x4(&view, camera.GetViewMatrix());
        DirectX::XMStoreFloat4x4(&projection, camera.GetProjectionMatrix());

        EffectManager::Instance().Render(view, projection);
    }

    //�u���[������QD
    {
        UserInterface::Instance().BloomRender();
    }

    framebuffers[0]->Deactivate(graphics.GetDeviceContext());

    // FOG
    {
        framebuffers[1]->Clear(graphics.GetDeviceContext());
        framebuffers[1]->Activate(graphics.GetDeviceContext());
        shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_OFF_ZW_OFF));
        shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::CULL_NONE));
        shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::NONE));

        bitBlockTransfer->Blit(graphics.GetDeviceContext(), framebuffers[0]->shaderResourceViews[1].GetAddressOf()/*Depth*/, 0, 1, fogPS.Get());

        framebuffers[1]->Deactivate(graphics.GetDeviceContext());

        shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_OFF_ZW_OFF));
        shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::CULL_NONE));
        ID3D11ShaderResourceView* shaderResourceView[]
        {
            framebuffers[0]->shaderResourceViews[0].Get(),
            framebuffers[0]->shaderResourceViews[0].Get(),/*dummy*/
            framebuffers[1]->shaderResourceViews[0].Get(),
        };
    }



    // BLOOM
    bloomer->Make(graphics.GetDeviceContext(), framebuffers[0]->shaderResourceViews[0].Get());

    shader->SetDepthStencileState(static_cast<UINT>(Shader::DEPTH_STATE::ZT_OFF_ZW_OFF));
    shader->SetRasterizerState(static_cast<UINT>(Shader::RASTER_STATE::CULL_NONE));
    shader->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::ALPHA));

    ID3D11ShaderResourceView* shaderResourceViews[] =
    {
        framebuffers[0]->shaderResourceViews[0].Get(),
        bloomer->ShaderResourceView(),
        framebuffers[1]->shaderResourceViews[0].Get(),
        framebuffers[0]->shaderResourceViews[1].Get(),
    };

    bitBlockTransfer->Blit(graphics.GetDeviceContext(), shaderResourceViews, 0, _countof(shaderResourceViews), bloomPS.Get());

    //�u���[������
    {
        PlayerManager::Instance().SkillImagesRender();
        UserInterface::Instance().Render();
    }

    // 2D�`��
    {
        // �`���[�g���A���X�v���C�g�`��
        TutorialManager::Instance().Render();
    }
}

void SceneTutorial::DrawDebug()
{
}
