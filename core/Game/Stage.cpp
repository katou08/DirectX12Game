#include "Game/Stage.h"
#include "Game/Game.h"
#include "Graphics/GraphicsEngine.h"
#include "System/ImguiSystem.h";
#include "System/InputSystem.h"
#include "System/AudioSystem.h"
#include "Game/Display.h"
#include "Game/Camera.h"
#include "Game/Player.h"
#include "Game/Enemy.h"
#include "Game/ResultScreen.h"
#include "Game/Bullet.h"
#include "Entity/Entity.h"
#include "Entity/TextComponent.h"
#include "Entity/SpriteComponent.h"
#include "Entity/TransformComponent.h"
#include "Entity/MeshComponent.h"
#include "Entity/CollisionTypeComponent.h"
#include "Resource/Mesh.h"
#include "Resource/Text.h"
#include "Resource/Sprite.h"

Stage::Stage(Game* game, const std::wstring& sceneName)
	: Scene(game, sceneName)
{
}

Stage::~Stage()
{
}

bool Stage::LoadData()
{
	m_pCamera = m_pGame->GetCamera();
	auto display = m_pGame->GetDisplay();
	auto width = display->getClientSize().width;
	auto height = display->getClientSize().height;

	m_pGame->GetAudioSystem()->PlayBGM(L"sound/bgm.wav", 0.1f, 0.0f, 0.0f, true);

	m_pResultScreen = AddEntity<ResultScreen>();
	m_pStartTitleText = AddEntity<Entity>();
	m_pStartTitleTextComp = m_pStartTitleText->AddComponent<TextComponent>();
	m_pStartTitleTextComp->SetText(L"res/font/fonttest.spritefont");
	m_pStartTitleTextComp->GetText()->SetTextString(L"START");
	m_pStartTitleTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 - 250.0f));
	m_pStartTitleTextComp->SetActive(false);

	// ポーズアイコン
	m_pPauseSprite = AddEntity<Entity>();
	m_pPauseSpriteComp = m_pPauseSprite->AddComponent<SpriteComponent>();
	m_pPauseSpriteComp->SetSprite(L"res/tex/RightClickWhite.dds");
	m_pPauseSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(50.0f, height - 60.0f));
	m_pPauseSpriteComp->GetSprite()->SetSpriteScale(0.1f);

	m_pPauseText = AddEntity<Entity>();
	m_pPauseTextComp = m_pPauseText->AddComponent<TextComponent>();
	m_pPauseTextComp->SetText(L"res/font/fonttest.spritefont");
	m_pPauseTextComp->GetText()->SetTextString(L"Pause");
	m_pPauseTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(50.0f, height - 20.0f));
	m_pPauseTextComp->GetText()->SetTextScale(0.5f);

	m_pLimitTimeText = AddEntity<Entity>();
	m_pLimitTimeTextComp = m_pLimitTimeText->AddComponent<TextComponent>();
	m_pLimitTimeTextComp->SetText(L"res/font/fonttest.spritefont");
	std::wstring MinutestimeString = std::to_wstring((int)(m_LimitTime / 60));
	std::wstring SecondstimeString = std::to_wstring((int)(m_LimitTime - (int)(m_LimitTime / 60) * 60));
	if ((int)(m_LimitTime - (int)(m_LimitTime / 60) * 60) < 10)
		m_pLimitTimeTextComp->GetText()->SetTextString(L"0" + MinutestimeString + L" : 0" + SecondstimeString);
	else
		m_pLimitTimeTextComp->GetText()->SetTextString(L"0" + MinutestimeString + L" : " + SecondstimeString);
	m_pLimitTimeTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, 20.0f));
	m_pLimitTimeTextComp->GetText()->SetTextScale(0.5f);

	m_pMap = AddEntity<Entity>();
	auto m_pMapComp = m_pMap->AddComponent<MeshComponent>();
	m_pMapComp->SetMesh(L"res/plane/planeStage1.obj");

	m_pCircularEnemy = AddEntity<Enemy>();
	m_pCircularEnemy->SetEnemyType(1);
	m_pCircularEnemy->LoadData();
	m_pCircularEnemy->GetComponent<TransformComponent>()->SetPosition(Vector3(-10.0f, 0.0f, 0.0f));
	m_pCircularEnemy->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(-10.0f, 0.0f, 0.0f));
	m_pCircularEnemy->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));
	m_pCircularEnemy->GetBulletEntity()->GetTransform()->SetPosition(m_pCircularEnemy->GetTransform()->GetPosition());
	for (int i = 0; i < m_pCircularEnemy->GetBulletEntity()->GetComponent<MeshComponent>()->GetMesh()->GetInstanceCount(); ++i)
	{
		m_pCircularEnemy->GetBulletEntity()->GetComponent<MeshComponent>()->GetMesh()->SetPosition(m_pCircularEnemy->GetTransform()->GetPosition(), i);
	}

	m_pCircularEnemy1 = AddEntity<Enemy>();
	m_pCircularEnemy1->SetEnemyType(1);
	m_pCircularEnemy1->LoadData();
	m_pCircularEnemy1->GetComponent<TransformComponent>()->SetPosition(Vector3(10.0f, 0.0f, 0.0f));
	m_pCircularEnemy1->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(10.0f, 0.0f, 0.0f));
	m_pCircularEnemy1->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));
	m_pCircularEnemy1->GetBulletEntity()->GetTransform()->SetPosition(m_pCircularEnemy1->GetTransform()->GetPosition());
	for (int i = 0; i < m_pCircularEnemy1->GetBulletEntity()->GetComponent<MeshComponent>()->GetMesh()->GetInstanceCount(); ++i)
	{
		m_pCircularEnemy1->GetBulletEntity()->GetComponent<MeshComponent>()->GetMesh()->SetPosition(m_pCircularEnemy1->GetTransform()->GetPosition(), i);
	}

	// プレイヤーの追加
	m_pPlayer = AddEntity<Player>();
	m_pPlayer->GetTransform()->SetPosition(Vector3(0.0f, 0.0f, -12.0f));
	m_pPlayer->GetComponent<MeshComponent>()->GetMesh()->SetPosition(m_pPlayer->GetTransform()->GetPosition());
	// プレイヤーの移動制限
	m_pPlayer->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));
	m_pPlayer->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));
	m_pPlayer->GetBulletEntity()->GetTransform()->SetPosition(m_pPlayer->GetTransform()->GetPosition());
	for (int i = 0; i < m_pPlayer->GetBulletEntity()->GetComponent<MeshComponent>()->GetMesh()->GetInstanceCount(); ++i)
	{
		m_pPlayer->GetBulletEntity()->GetComponent<MeshComponent>()->GetMesh()->SetPosition(m_pPlayer->GetTransform()->GetPosition(), i);
	}

	auto playerPos = m_pPlayer->GetTransform()->GetPosition();
	m_pCamera->SetPositionAndTarget(Vector3(playerPos.x, playerPos.y + 15.0f, playerPos.z - 10.0f), Vector3(playerPos.x, playerPos.y, playerPos.z - 2.0f));

	// キューブ作成
	GenerateCubes();

	return true;
}

void Stage::Update(float deltaTime)
{
	auto display = m_pGame->GetDisplay();
	auto width = display->getClientSize().width;
	auto height = display->getClientSize().height;

	UpdateCubes(deltaTime);
	UpdateSpriteAndTextPosition();
	m_pResultScreen->SetTotalBullets(m_pPlayer->GetTotalFireBulletCount());

	if (m_State == StageState::Playing)
	{
		m_LimitTime -= deltaTime;
		std::wstring MinutestimeString = std::to_wstring((int)(m_LimitTime / 60));
		std::wstring SecondstimeString = std::to_wstring((int)(m_LimitTime - (int)(m_LimitTime / 60) * 60));
		if ((int)(m_LimitTime - (int)(m_LimitTime / 60) * 60) < 10)
			m_pLimitTimeTextComp->GetText()->SetTextString(L"0" + MinutestimeString + L" : 0" + SecondstimeString);
		else
			m_pLimitTimeTextComp->GetText()->SetTextString(L"0" + MinutestimeString + L" : " + SecondstimeString);

		m_pPauseSpriteComp->SetActive(true);
		m_pPauseTextComp->SetActive(true);

		auto mouse = m_pGame->GetInputSystem()->GetMouse()->GetState();
		auto kb = m_pGame->GetInputSystem()->GetKeybord()->GetState();
		auto& kbLog = m_pGame->GetInputSystem()->GetKeyLog();

#if defined(DEBUG) || defined(_DEBUG)
		m_pGame->GetGraphicsEngine()->GetImguiSystem()->SetText(m_pPlayer->GetComponent<MeshComponent>()->GetMesh()->GetPosition(0));
		//m_pGame->GetGraphicsEngine()->GetImguiSystem()->SetText1(CoreShowCount);
		m_pGame->GetGraphicsEngine()->GetImguiSystem()->SetText1(m_pPlayer->GetBulletEntity()->GetComponent<MeshComponent>()->GetMesh()->GetPosition(10).z);
#endif
		if (m_pPlayer->GetHealth() == 0)
		{
			m_State = StageState::GameOver;
		}

		if (m_ChaseEnemy)
			m_ChaseEnemy->SetPlayerPos(m_pPlayer->GetTransform()->GetPosition());
		if (m_ChaseEnemy1)
			m_ChaseEnemy1->SetPlayerPos(m_pPlayer->GetTransform()->GetPosition());

		if (CoreShowCount == 1)
		{
			if (ChaseShowCount == 0)
				AddChaseEnemy();
			ChaseShowCount = 1;
		}
		else if (CoreShowCount == 2)
		{
			if (ChaseShowCount == 1)
				AddChaseEnemy();
			ChaseShowCount = 2;
		}

		if (CircularShowCount == 2)
		{
			m_pCoreEnemy = AddEntity<Enemy>();
			m_pCoreEnemy->SetEnemyType(3);
			m_pCoreEnemy->LoadData();
			m_pCoreEnemy->GetComponent<TransformComponent>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			m_pCoreEnemy->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(0.0f, 0.0f, 0.0f), 0);
			m_pCoreEnemy->GetComponent<MeshComponent>()->GetMesh()->SetAllBaseColor(Vector3(0.3f, 0.5f, 0.24f));
			m_pCoreEnemy->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));

			CircularShowCount = 3;
		}

		DeleteEnemy();

		// カメラ更新処理
		{
			auto playerPos = m_pPlayer->GetTransform()->GetPosition();
			auto cameraPos = Vector3::Lerp(m_pCamera->GetPosition(), Vector3(playerPos.x, playerPos.y + 15.0f, playerPos.z - 10.0f), 8.0f * deltaTime);
			auto cameraTarget = Vector3::Lerp(m_pCamera->GetTarget(), Vector3(playerPos.x, playerPos.y, playerPos.z - 2.0f), 8.0f * deltaTime);
			m_pCamera->SetPositionAndTarget(cameraPos, cameraTarget);
		}

		if (mouse.rightButton)
			m_State = StageState::Pause;

		if (m_LimitTime <= 0.0f)
		{
			m_State = StageState::GameOver;
		}
	}
	else if (m_State == StageState::GameOver)
	{
		m_pPauseSpriteComp->SetActive(false);
		m_pPauseTextComp->SetActive(false);
		IsGameOver = true;
	}
	else if (m_State == StageState::GameCompleted)
	{
		m_pPauseSpriteComp->SetActive(false);
		m_pPauseTextComp->SetActive(false);
		IsClear = true;
	}
	else if (m_State == StageState::Pause)
	{
		IsPause = true;
		m_pPauseSpriteComp->SetActive(false);
		m_pPauseTextComp->SetActive(false);
		if (m_pResultScreen->GetPause() == false)
		{
			m_State = StageState::Playing;
			IsPause = false;
		}
		m_pResultScreen->SetPause(true);
	}
	else if (m_State == StageState::Start)
	{
		IsStart = true;
		m_StartTime += deltaTime;
		if (m_StartTime > 0.1f)
		{
			m_pStartTitleTextComp->SetActive(true);
		}
		if (m_StartTime > 3.0f)
		{
			m_pStartTitleTextComp->SetActive(false);
			m_State = StageState::Playing;
			IsStart = false;
		}
	}

	Scene::UpdateInternal(deltaTime); //!< シーンクラスのupdate呼び出し
}

void Stage::DeleteEnemy()
{
	if (m_pCircularEnemy)
	{
		if (m_pCircularEnemy->GetHealth() == 0)
		{
			m_pCircularEnemy->RemoveBulletEntity();
			RemoveEntity(m_pCircularEnemy);
			m_pCircularEnemy = nullptr;
			++CoreShowCount;
		}
	}
	if (m_pCircularEnemy1)
	{
		if (m_pCircularEnemy1->GetHealth() == 0)
		{
			m_pCircularEnemy1->RemoveBulletEntity();
			RemoveEntity(m_pCircularEnemy1);
			m_pCircularEnemy1 = nullptr;
			++CoreShowCount;
		}
	}
	if (m_ChaseEnemy)
	{
		if (m_ChaseEnemy->GetHealth() == 0)
		{
			m_ChaseEnemy->RemoveBulletEntity();
			RemoveEntity(m_ChaseEnemy);
			m_ChaseEnemy = nullptr;
			++CircularShowCount;
		}
	}
	if (m_ChaseEnemy1)
	{
		if (m_ChaseEnemy1->GetHealth() == 0)
		{
			m_ChaseEnemy1->RemoveBulletEntity();
			RemoveEntity(m_ChaseEnemy1);
			m_ChaseEnemy1 = nullptr;
			++CircularShowCount;
		}
	}
	if (m_pCoreEnemy)
	{
		if (m_pCoreEnemy->GetHealth() == 0)
		{
			m_pCoreEnemy->RemoveBulletEntity();
			RemoveEntity(m_pCoreEnemy);
			m_pCoreEnemy = nullptr;
			m_State = StageState::GameCompleted;
		}
	}
}

void Stage::AddChaseEnemy()
{
	if (CoreShowCount == 1)
	{
		m_ChaseEnemy = AddEntity<Enemy>();
		m_ChaseEnemy->LoadData();
		m_ChaseEnemy->GetComponent<TransformComponent>()->SetPosition(Vector3(-10.0f, 0.0f, 10.0f));
		m_ChaseEnemy->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(-10.0f, 0.0f, 10.0f), 0);
		m_ChaseEnemy->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));
	}
	else if (CoreShowCount == 2)
	{
		m_ChaseEnemy1 = AddEntity<Enemy>();
		m_ChaseEnemy1->LoadData();
		m_ChaseEnemy1->GetComponent<TransformComponent>()->SetPosition(Vector3(10.0f, 0.0f, 10.0f));
		m_ChaseEnemy1->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(10.0f, 0.0f, 10.0f), 0);
		m_ChaseEnemy1->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));
	}
}

void Stage::GenerateCubes()
{
	m_pCubes = AddEntity<Entity>();
	m_pCubesMesh = m_pCubes->AddComponent<MeshComponent>();
	m_pCubesMesh->SetMesh(L"res/box/box.obj");
	m_pCubesMesh->GetMesh()->ChangeInstanceCount(500);
	m_Axis.resize(500);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> selectRangex(0, 1);
	std::uniform_int_distribution<int> selectRangez(0, 1);
	float limit = m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0).Extents.x;
	float limity = std::sqrt(13) / 2 * limit;
	std::uniform_real_distribution<float> distributionAxisX(0.0f, 1.0f);
	std::uniform_real_distribution<float> distributionAxisY(0.0f, 1.0f);
	std::uniform_real_distribution<float> distributionAxisZ(0.0f, 1.0f);

	std::uniform_real_distribution<float> distributionPosX(-50.0f, 50.0f);
	std::uniform_real_distribution<float> distributionPosY(-15.0f, -1.0f);
	std::uniform_real_distribution<float> distributionPosZ(-25.0f, 50.0f);

	std::uniform_real_distribution<float> distributionScale(0.1f, 0.2f);
	for (int i = 0; i < m_pCubesMesh->GetMesh()->GetInstanceCount(); ++i)
	{
		float AxisX = distributionAxisX(gen);
		float AxisY = distributionAxisY(gen);
		float AxisZ = distributionAxisZ(gen);
		float randomPosy = distributionPosY(gen);
		float randomPosx = distributionPosX(gen);
		float randomPosz = distributionPosZ(gen);
		float randScale = distributionScale(gen);
		m_pCubesMesh->GetMesh()->SetPosition(Vector3(randomPosx, randomPosy, randomPosz), i);
		m_pCubesMesh->GetMesh()->SetScale(Vector3(randScale, randScale, randScale), i);
		m_Axis[i] = { AxisX, AxisY, AxisZ };
	}
}

void Stage::UpdateCubes(float deltaTime)
{
	m_addDeltaTime += deltaTime;
	for (int i = 0; i < m_pCubesMesh->GetMesh()->GetInstanceCount(); ++i)
	{
		auto q = Quaternion::CreateFromAxisAngle(m_Axis[i], DirectX::XM_PIDIV2 * m_addDeltaTime * 0.5f);
		m_pCubesMesh->GetMesh()->SetRotation(q, i);
	}
}

void Stage::UpdateSpriteAndTextPosition()
{
	auto display = m_pGame->GetDisplay();
	auto width = display->getClientSize().width;
	auto height = display->getClientSize().height;
	m_pStartTitleTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 - 250.0f));
	m_pLimitTimeTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, 20.0f));
	m_pPauseTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(50.0f, height - 20.0f));
	m_pPauseSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(50.0f, height - 60.0f));
}
