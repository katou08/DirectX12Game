#include "Game/Title.h"
#include "Game/Game.h"
#include "System/AudioSystem.h"
#include "System/InputSystem.h"
#include "Game/Camera.h"
#include "Game/Player.h"
#include "Entity/Entity.h"
#include "Entity/Component.h"
#include "Entity/TransformComponent.h"
#include "Entity/SpriteComponent.h"
#include "Entity/MeshComponent.h"
#include "Resource/Sprite.h"
#include "Entity/TextComponent.h"
#include "Resource/Text.h"
#include "Resource/Mesh.h"
#include "Game/Display.h"

Title::Title(Game* game, const std::wstring& sceneName)
	: Scene(game, sceneName)
{
}

Title::~Title()
{
}

bool Title::LoadData()
{
	m_pCamera = m_pGame->GetCamera();
	m_pCamera->Reset();
	auto display = m_pGame->GetDisplay();
	width = display->getClientSize().width;
	height = display->getClientSize().height;

	m_pGame->GetAudioSystem()->PlayBGM(L"sound/title.wav", 0.1f, 0.0f, 0.0f, true);

	m_pSpriteStart = AddEntity<Entity>();
	m_pSpriteStartComp = m_pSpriteStart->AddComponent<SpriteComponent>();
	m_pSpriteStartComp->SetSprite(L"res/tex/SelectedButton_Gray.dds");
	m_pSpriteStartComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 100.0f));

	m_pSpriteQuit = AddEntity<Entity>();
	m_pSpriteQuitComp = m_pSpriteQuit->AddComponent<SpriteComponent>();
	m_pSpriteQuitComp->SetSprite(L"res/tex/SelectButton.dds");
	m_pSpriteQuitComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 200.0f));

	m_pFontStart = AddEntity<Entity>();
	m_pFontStartComp = m_pFontStart->AddComponent<TextComponent>();
	m_pFontStartComp->SetText(L"res/font/fonttest.spritefont");
	m_pFontStartComp->GetText()->SetTextString(L"Start");
	m_pFontStartComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 100.0f));
	m_pFontStartComp->GetText()->SetTextScale(0.8f);

	m_pFontQuit = AddEntity<Entity>();
	m_pFontQuitComp = m_pFontQuit->AddComponent<TextComponent>();
	m_pFontQuitComp->SetText(L"res/font/fonttest.spritefont");
	m_pFontQuitComp->GetText()->SetTextString(L"Quit");
	m_pFontQuitComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 200.0f));
	m_pFontQuitComp->GetText()->SetTextScale(0.8f);

	GenerateCubes();

	return true;
}

void Title::GenerateCubes()
{

	m_pCubes = AddEntity<Entity>();
	m_pCubesMesh = m_pCubes->AddComponent<MeshComponent>();
	m_pCubesMesh->SetMesh(L"res/box/box.obj");
	m_pCubesMesh->GetMesh()->ChangeInstanceCount(500);
	m_Axis.resize(500);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distributionx(-25.0f, 25.0f);
	std::uniform_real_distribution<float> distributiony(-15.0f, 15.0f);
	std::uniform_real_distribution<float> distributionz(10.0f, 50.0f);
	std::uniform_real_distribution<float> distributionAxisX(0.0f, 1.0f);
	std::uniform_real_distribution<float> distributionAxisY(0.0f, 1.0f);
	std::uniform_real_distribution<float> distributionAxisZ(0.0f, 1.0f);
	std::uniform_real_distribution<float> distributionScale(0.1f, 0.2f);
	for (int i = 0; i < m_pCubesMesh->GetMesh()->GetInstanceCount(); ++i)
	{
		float AxisX = distributionAxisX(gen);
		float AxisY = distributionAxisY(gen);
		float AxisZ = distributionAxisZ(gen);
		float randomNumberx = distributionx(gen);
		float randomNumbery = distributiony(gen);
		float randomNumberz = distributionz(gen);
		float randScale = distributionScale(gen);
		m_pCubesMesh->GetMesh()->SetPosition(Vector3(randomNumberx, randomNumbery, randomNumberz), i);
		m_pCubesMesh->GetMesh()->SetScale(Vector3(randScale, randScale, randScale), i);
		m_Axis[i] = {AxisX, AxisY, AxisZ};
	}
}

void Title::Update(float deltaTime)
{
	auto display = m_pGame->GetDisplay();
	width = display->getClientSize().width;
	height = display->getClientSize().height;

	auto kb = m_pGame->GetInputSystem()->GetKeybord()->GetState();
	auto kbLog = m_pGame->GetInputSystem()->GetKeyLog();
	loadTime += deltaTime;
	m_addDeltaTime += deltaTime;

	m_pFontStartComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 100.0f));
	m_pFontQuitComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 200.0f));

	m_addDeltaTime += deltaTime;
	for (int i = 0; i < m_pCubesMesh->GetMesh()->GetInstanceCount(); ++i)
	{
		auto q = Quaternion::CreateFromAxisAngle(m_Axis[i], DirectX::XM_PIDIV2 * m_addDeltaTime * 0.5f);
		m_pCubesMesh->GetMesh()->SetRotation(q, i);
	}

	if (loadTime > 0.5f)
	{
		if (kb.Up)
		{
			--selectCount;
			if (selectCount <= 0)
				selectCount = 0;
		}
		else if (kb.Down)
		{
			++selectCount;
			if (selectCount >= 1)
				selectCount = 1;
		}

		if (selectCount == 0)
		{
			if (selectAudioCount == 1)
			{
				m_pGame->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
			}
			selectAudioCount = 0;

			m_pSpriteStartComp->SetSprite(L"res/tex/SelectedButton_Gray.dds");
			m_pSpriteStartComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 100.0f));

			m_pSpriteQuitComp->SetSprite(L"res/tex/SelectButton.dds");
			m_pSpriteQuitComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 200.0f));

			if (kbLog.pressed.Enter)
			{
				m_pGame->GetAudioSystem()->StopBGM(L"sound/title.wav");
				if (selectAudioCount = 1)
				{
					m_pGame->GetAudioSystem()->PlaySE(L"sound/click.wav", 0.1f);
				}
				selectAudioCount = 2;
				// ステージ選択シーンをロード
				IsQuit = true;
				m_NextSceneName = L"SelectStage";
			}
		}
		else if (selectCount == 1)
		{
			if (selectAudioCount == 0)
			{
				m_pGame->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
			}
			selectAudioCount = 1;

			m_pSpriteStartComp->SetSprite(L"res/tex/SelectButton.dds");
			m_pSpriteStartComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 100.0f));

			m_pSpriteQuitComp->SetSprite(L"res/tex/SelectedButton_Gray.dds");
			m_pSpriteQuitComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 200.0f));

			if (kb.Enter)
			{
				m_pGame->GetAudioSystem()->StopBGM(L"sound/title.wav");
				if (selectAudioCount == 1)
				{
					m_pGame->GetAudioSystem()->PlaySE(L"sound/click.wav", 0.1f);
				}
				selectAudioCount = 2;

				// ゲーム終了
				IsQuit = true;
			}
		}
	}

	Scene::UpdateInternal(deltaTime); //!< シーンクラスのupdate呼び出し
}
