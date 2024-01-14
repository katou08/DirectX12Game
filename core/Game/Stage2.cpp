#include "Game/Stage2.h"
#include "Game/Game.h"
#include "Graphics/GraphicsEngine.h"
#include "System/ImguiSystem.h";
#include "Game/Display.h"
#include "System/InputSystem.h"
#include "Game/Camera.h"
#include "Game/Player.h"
#include "Game/Enemy.h"
#include "Game/ResultScreen.h"
#include "Entity/Entity.h"
#include "Entity/TextComponent.h"
#include "Entity/SpriteComponent.h"
#include "Entity/TransformComponent.h"
#include "Entity/MeshComponent.h"
#include "Entity/CollisionTypeComponent.h"
#include "Resource/Mesh.h"
#include "Resource/Text.h"
#include "Resource/Sprite.h"
#include "System/AudioSystem.h"
#include "Game/Bullet.h"


Stage2::Stage2(Game* game, const std::wstring& sceneName)
	: Scene(game, sceneName)
{
}

Stage2::~Stage2()
{
}

bool Stage2::LoadData()
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
	m_pMapComp->SetMesh(L"res/plane/planeStage2.obj");

	m_pCircularEnemy = AddEntity<Enemy>();
	m_pCircularEnemy->SetEnemyType(1);
	m_pCircularEnemy->LoadData();
	m_pCircularEnemy->GetComponent<TransformComponent>()->SetPosition(Vector3(10.0f, 0.0f, 10.0f));
	m_pCircularEnemy->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(10.0f, 0.0f, 10.0f), 0);
	m_pCircularEnemy->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));
	m_pCircularEnemy->GetBulletEntity()->GetTransform()->SetPosition(m_pCircularEnemy->GetTransform()->GetPosition());
	for (int i = 0; i < m_pCircularEnemy->GetBulletEntity()->GetComponent<MeshComponent>()->GetMesh()->GetInstanceCount(); ++i)
	{
		m_pCircularEnemy->GetBulletEntity()->GetComponent<MeshComponent>()->GetMesh()->SetPosition(m_pCircularEnemy->GetTransform()->GetPosition(), i);
	}

	m_pPlayer = AddEntity<Player>();
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

	m_pLeftWall = AddEntity<Entity>();
	auto m_pLeftWallMeshComp = m_pLeftWall->AddComponent<MeshComponent>();
	m_pLeftWallMeshComp->SetMesh(L"res/wall/wallScale9rot.obj");
	auto m_pLeftWallCollisionComp = m_pLeftWall->AddComponent<CollisionTypeComponent>();
	m_pLeftWallCollisionComp->SetCollisionType(CollisionType::WallCollision);
	m_pLeftWall->GetComponent<TransformComponent>()->SetPosition(Vector3(-12.0f, 0.0f, 0.0f));
	m_pLeftWallMeshComp->GetMesh()->SetPosition(m_pLeftWall->GetTransform()->GetPosition());

	m_pRightWall = AddEntity<Entity>();
	auto m_pRightWallMeshComp = m_pRightWall->AddComponent<MeshComponent>();
	m_pRightWallMeshComp->SetMesh(L"res/wall/wallScale9rot.obj");
	auto m_pRightWallCollisionComp = m_pRightWall->AddComponent<CollisionTypeComponent>();
	m_pRightWallCollisionComp->SetCollisionType(CollisionType::WallCollision);
	m_pRightWall->GetComponent<TransformComponent>()->SetPosition(Vector3(12.0f, 0.0f, 0.0f));
	m_pRightWallMeshComp->GetMesh()->SetPosition(m_pRightWall->GetTransform()->GetPosition());

	m_pTopWall = AddEntity<Entity>();
	auto m_pTopWallMeshComp = m_pTopWall->AddComponent<MeshComponent>();
	m_pTopWallMeshComp->SetMesh(L"res/wall/wallScale9.obj");
	auto m_pTopWallCollisionComp = m_pTopWall->AddComponent<CollisionTypeComponent>();
	m_pTopWallCollisionComp->SetCollisionType(CollisionType::WallCollision);
	m_pTopWall->GetComponent<TransformComponent>()->SetPosition(Vector3(0.0f, 0.0f, 12.0f));
	m_pTopWallMeshComp->GetMesh()->SetPosition(m_pTopWall->GetTransform()->GetPosition());

	m_pBottomWall = AddEntity<Entity>();
	auto m_pBottomWallMeshComp = m_pBottomWall->AddComponent<MeshComponent>();
	m_pBottomWallMeshComp->SetMesh(L"res/wall/wallScale9.obj");
	auto m_pBottomWallCollisionComp = m_pBottomWall->AddComponent<CollisionTypeComponent>();
	m_pBottomWallCollisionComp->SetCollisionType(CollisionType::WallCollision);
	m_pBottomWallCollisionComp->SetCollisionType(CollisionType::WallCollision);
	m_pBottomWall->GetComponent<TransformComponent>()->SetPosition(Vector3(0.0f, 0.0f, -12.0f));
	m_pBottomWallMeshComp->GetMesh()->SetPosition(m_pBottomWall->GetTransform()->GetPosition());

	GenerateCubes();
	return true;
}

void Stage2::Update(float deltaTime)
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
		m_pLimitTimeTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, 20.0f));

		m_pPauseSpriteComp->SetActive(true);
		m_pPauseTextComp->SetActive(true);

		auto mouse = m_pGame->GetInputSystem()->GetMouse()->GetState();
		auto kb = m_pGame->GetInputSystem()->GetKeybord()->GetState();

#if defined(DEBUG) || defined(_DEBUG)
		m_pGame->GetGraphicsEngine()->GetImguiSystem()->SetText(m_pPlayer->GetComponent<MeshComponent>()->GetMesh()->GetPosition(0));
		m_pGame->GetGraphicsEngine()->GetImguiSystem()->SetText1(CircularShowCount);
#endif
		if (m_pPlayer->GetHealth() == 0)
			m_State = StageState::GameOver;

		SetPlayerPositionForChaseEnemy();

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
		else if (CoreShowCount == 3)
		{
			if (ChaseShowCount == 2)
				AddChaseEnemy();
			ChaseShowCount = 3;
		}
		else if (CoreShowCount == 4)
		{
			if (ChaseShowCount == 3)
				AddChaseEnemy();
			ChaseShowCount = 4;
		}

		if (CircularShowCount == 3)
		{
			if (CircularShowLimitCount == 0)
			{
				m_pCircularEnemy1 = AddEntity<Enemy>();
				m_pCircularEnemy1->SetEnemyType(1);
				m_pCircularEnemy1->LoadData();
				m_pCircularEnemy1->GetComponent<TransformComponent>()->SetPosition(Vector3(-10.0f, 0.0f, 10.0f));
				m_pCircularEnemy1->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(-10.0f, 0.0f, 10.0f), 0);
				m_pCircularEnemy1->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));
			}
			CircularShowLimitCount = 1;
		}
		else if (CircularShowCount == 6)
		{
			if (CircularShowLimitCount == 1)
			{
				m_pCircularEnemy2 = AddEntity<Enemy>();
				m_pCircularEnemy2->SetEnemyType(1);
				m_pCircularEnemy2->LoadData();
				m_pCircularEnemy2->GetComponent<TransformComponent>()->SetPosition(Vector3(-10.0f, 0.0f, -10.0f));
				m_pCircularEnemy2->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(-10.0f, 0.0f, -10.0f), 0);
				m_pCircularEnemy2->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));
			}
			CircularShowLimitCount = 2;
		}
		else if (CircularShowCount == 9)
		{
			if (CircularShowLimitCount == 2)
			{
				m_pCircularEnemy3 = AddEntity<Enemy>();
				m_pCircularEnemy3->SetEnemyType(1);
				m_pCircularEnemy3->LoadData();
				m_pCircularEnemy3->GetComponent<TransformComponent>()->SetPosition(Vector3(10.0f, 0.0f, -10.0f));
				m_pCircularEnemy3->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(10.0f, 0.0f, -10.0f), 0);
				m_pCircularEnemy3->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));
			}
			CircularShowLimitCount = 3;
		}
		else if (CircularShowCount == 12)
		{
			if (CircularShowLimitCount == 3)
			{
				m_pCoreEnemy = AddEntity<Enemy>();
				m_pCoreEnemy->SetEnemyType(3);
				m_pCoreEnemy->LoadData();
				m_pCoreEnemy->GetComponent<TransformComponent>()->SetPosition(Vector3(0.0f, 0.0f, 2.0f));
				m_pCoreEnemy->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(0.0f, 0.0f, 2.0f), 0);
				m_pCoreEnemy->GetComponent<MeshComponent>()->GetMesh()->SetAllBaseColor(Vector3(0.3f, 0.5f, 0.24f));
				m_pCoreEnemy->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));
			}
			CircularShowLimitCount = 4;
			CoreShowCount = 5;
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
		{
			m_State = StageState::Pause;
		}

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

void Stage2::DeleteEnemy()
{
	if (m_pCircularEnemy != nullptr)
	{
		int health = m_pCircularEnemy->GetHealth();
		if (health == 0)
		{
			++CoreShowCount;
			m_pCircularEnemy->RemoveBulletEntity();
			RemoveEntity(m_pCircularEnemy);
			m_pCircularEnemy = nullptr;
		}
	}
	if (m_pCircularEnemy1 != nullptr)
	{
		int health = m_pCircularEnemy1->GetHealth();
		if (health == 0)
		{
			++CoreShowCount;
			m_pCircularEnemy1->RemoveBulletEntity();
			RemoveEntity(m_pCircularEnemy1);
			m_pCircularEnemy1 = nullptr;
		}
	}
	if (m_pCircularEnemy2 != nullptr)
	{
		int health = m_pCircularEnemy2->GetHealth();
		if (health == 0)
		{
			++CoreShowCount;
			m_pCircularEnemy2->RemoveBulletEntity();
			RemoveEntity(m_pCircularEnemy2);
			m_pCircularEnemy2 = nullptr;
		}
	}
	if (m_pCircularEnemy3 != nullptr)
	{
		int health = m_pCircularEnemy3->GetHealth();
		if (health == 0)
		{
			++CoreShowCount;

			m_pCircularEnemy3->RemoveBulletEntity();
			RemoveEntity(m_pCircularEnemy3);
			m_pCircularEnemy3 = nullptr;
		}
	}
	if (m_ChaseEnemy != nullptr)
	{
		int health = m_ChaseEnemy->GetHealth();
		if (health == 0)
		{
			++CircularShowCount;

			m_ChaseEnemy->RemoveBulletEntity();
			RemoveEntity(m_ChaseEnemy);
			m_ChaseEnemy = nullptr;
		}
	}
	if (m_ChaseEnemy1 != nullptr)
	{
		int health = m_ChaseEnemy1->GetHealth();
		if (health == 0)
		{
			++CircularShowCount;

			m_ChaseEnemy1->RemoveBulletEntity();
			RemoveEntity(m_ChaseEnemy1);
			m_ChaseEnemy1 = nullptr;
		}
	}
	if (m_ChaseEnemy2 != nullptr)
	{
		int health = m_ChaseEnemy2->GetHealth();
		if (health == 0)
		{
			++CircularShowCount;

			m_ChaseEnemy2->RemoveBulletEntity();
			RemoveEntity(m_ChaseEnemy2);
			m_ChaseEnemy2 = nullptr;
		}
	}
	if (m_ChaseEnemy3 != nullptr)
	{
		int health = m_ChaseEnemy3->GetHealth();
		if (health == 0)
		{
			++CircularShowCount;

			m_ChaseEnemy3->RemoveBulletEntity();
			RemoveEntity(m_ChaseEnemy3);
			m_ChaseEnemy3 = nullptr;
		}
	}
	if (m_ChaseEnemy4 != nullptr)
	{
		int health = m_ChaseEnemy4->GetHealth();
		if (health == 0)
		{
			++CircularShowCount;
			m_ChaseEnemy4->RemoveBulletEntity();
			RemoveEntity(m_ChaseEnemy4);
			m_ChaseEnemy4 = nullptr;
		}
	}
	if (m_ChaseEnemy5 != nullptr)
	{
		int health = m_ChaseEnemy5->GetHealth();
		if (health == 0)
		{
			++CircularShowCount;
			m_ChaseEnemy5->RemoveBulletEntity();
			RemoveEntity(m_ChaseEnemy5);
			m_ChaseEnemy5 = nullptr;
		}
	}
	if (m_ChaseEnemy6 != nullptr)
	{
		int health = m_ChaseEnemy6->GetHealth();
		if (health == 0)
		{
			++CircularShowCount;

			m_ChaseEnemy6->RemoveBulletEntity();
			RemoveEntity(m_ChaseEnemy6);
			m_ChaseEnemy6 = nullptr;
		}
	}
	if (m_ChaseEnemy7 != nullptr)
	{
		int health = m_ChaseEnemy7->GetHealth();
		if (health == 0)
		{
			++CircularShowCount;
			m_ChaseEnemy7->RemoveBulletEntity();
			RemoveEntity(m_ChaseEnemy7);
			m_ChaseEnemy7 = nullptr;
		}
	}
	if (m_ChaseEnemy8 != nullptr)
	{
		int health = m_ChaseEnemy8->GetHealth();
		if (health == 0)
		{
			++CircularShowCount;
			m_ChaseEnemy8->RemoveBulletEntity();
			RemoveEntity(m_ChaseEnemy8);
			m_ChaseEnemy8 = nullptr;
		}
	}
	if (m_ChaseEnemy9 != nullptr)
	{
		int health = m_ChaseEnemy9->GetHealth();
		if (health == 0)
		{
			++CircularShowCount;
			m_ChaseEnemy9->RemoveBulletEntity();
			RemoveEntity(m_ChaseEnemy9);
			m_ChaseEnemy9 = nullptr;
		}
	}
	if (m_ChaseEnemy10 != nullptr)
	{
		int health = m_ChaseEnemy10->GetHealth();
		if (health == 0)
		{
			++CircularShowCount;
			m_ChaseEnemy10->RemoveBulletEntity();
			RemoveEntity(m_ChaseEnemy10);
			m_ChaseEnemy10 = nullptr;
		}
	}
	if (m_ChaseEnemy11 != nullptr)
	{
		int health = m_ChaseEnemy11->GetHealth();
		if (health == 0)
		{
			++CircularShowCount;
			m_ChaseEnemy11->RemoveBulletEntity();
			RemoveEntity(m_ChaseEnemy11);
			m_ChaseEnemy11 = nullptr;
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

void Stage2::AddChaseEnemy()
{
	if (CoreShowCount == 1)
	{
		m_ChaseEnemy = AddEntity<Enemy>();
		m_ChaseEnemy->LoadData();
		m_ChaseEnemy->GetComponent<TransformComponent>()->SetPosition(Vector3(17.5f, 0.0f, 17.5f));
		m_ChaseEnemy->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(17.5f, 0.0f, 17.5f), 0);
		m_ChaseEnemy->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));

		m_ChaseEnemy1 = AddEntity<Enemy>();
		m_ChaseEnemy1->LoadData();
		m_ChaseEnemy1->GetComponent<TransformComponent>()->SetPosition(Vector3(6.0f, 0.0f, 17.5f));
		m_ChaseEnemy1->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(6.0f, 0.0f, 17.5f), 0);
		m_ChaseEnemy1->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));

		m_ChaseEnemy2 = AddEntity<Enemy>();
		m_ChaseEnemy2->LoadData();
		m_ChaseEnemy2->GetComponent<TransformComponent>()->SetPosition(Vector3(17.5f, 0.0f, 6.0f));
		m_ChaseEnemy2->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(17.5f, 0.0f, 6.0f), 0);
		m_ChaseEnemy2->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));
	}
	else if (CoreShowCount == 2)
	{
		m_ChaseEnemy3 = AddEntity<Enemy>();
		m_ChaseEnemy3->LoadData();
		m_ChaseEnemy3->GetComponent<TransformComponent>()->SetPosition(Vector3(-17.5f, 0.0f, 17.5f));
		m_ChaseEnemy3->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(-17.5f, 0.0f, 17.5f), 0);
		m_ChaseEnemy3->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));

		m_ChaseEnemy4 = AddEntity<Enemy>();
		m_ChaseEnemy4->LoadData();
		m_ChaseEnemy4->GetComponent<TransformComponent>()->SetPosition(Vector3(-6.0f, 0.0f, 17.5f));
		m_ChaseEnemy4->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(-6.0f, 0.0f, 17.5f), 0);
		m_ChaseEnemy4->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));

		m_ChaseEnemy5 = AddEntity<Enemy>();
		m_ChaseEnemy5->LoadData();
		m_ChaseEnemy5->GetComponent<TransformComponent>()->SetPosition(Vector3(-17.5f, 0.0f, 6.0f));
		m_ChaseEnemy5->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(-17.5f, 0.0f, 6.0f), 0);
		m_ChaseEnemy5->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));
	}
	else if (CoreShowCount == 3)
	{
		m_ChaseEnemy6 = AddEntity<Enemy>();
		m_ChaseEnemy6->LoadData();
		m_ChaseEnemy6->GetComponent<TransformComponent>()->SetPosition(Vector3(-17.5f, 0.0f, -17.5f));
		m_ChaseEnemy6->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(-17.5f, 0.0f, -17.5f), 0);
		m_ChaseEnemy6->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));

		m_ChaseEnemy7 = AddEntity<Enemy>();
		m_ChaseEnemy7->LoadData();
		m_ChaseEnemy7->GetComponent<TransformComponent>()->SetPosition(Vector3(-6.0f, 0.0f, -17.5f));
		m_ChaseEnemy7->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(-6.0f, 0.0f, -17.5f), 0);
		m_ChaseEnemy7->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));

		m_ChaseEnemy8 = AddEntity<Enemy>();
		m_ChaseEnemy8->LoadData();
		m_ChaseEnemy8->GetComponent<TransformComponent>()->SetPosition(Vector3(-17.5f, 0.0f, -6.0f));
		m_ChaseEnemy8->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(-17.5f, 0.0f, -6.0f), 0);
		m_ChaseEnemy8->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));
	}
	else if (CoreShowCount == 4)
	{
		m_ChaseEnemy9 = AddEntity<Enemy>();
		m_ChaseEnemy9->LoadData();
		m_ChaseEnemy9->GetComponent<TransformComponent>()->SetPosition(Vector3(17.5f, 0.0f, -17.5f));
		m_ChaseEnemy9->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(17.5f, 0.0f, -17.5f), 0);
		m_ChaseEnemy9->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));

		m_ChaseEnemy10 = AddEntity<Enemy>();
		m_ChaseEnemy10->LoadData();
		m_ChaseEnemy10->GetComponent<TransformComponent>()->SetPosition(Vector3(6.0f, 0.0f, -17.5f));
		m_ChaseEnemy10->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(6.0f, 0.0f, -17.5f), 0);
		m_ChaseEnemy10->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));

		m_ChaseEnemy11 = AddEntity<Enemy>();
		m_ChaseEnemy11->LoadData();
		m_ChaseEnemy11->GetComponent<TransformComponent>()->SetPosition(Vector3(17.5f, 0.0f, -6.0f));
		m_ChaseEnemy11->GetComponent<MeshComponent>()->GetMesh()->SetPosition(Vector3(17.5f, 0.0f, -6.0f), 0);
		m_ChaseEnemy11->GetBulletEntity()->SetOtherBoundBox(m_pMap->GetComponent<MeshComponent>()->GetMesh()->GetBoundBox(0));
	}
}

void Stage2::SetPlayerPositionForChaseEnemy()
{
	if (m_ChaseEnemy)
		m_ChaseEnemy->SetPlayerPos(m_pPlayer->GetTransform()->GetPosition());
	if (m_ChaseEnemy1)
		m_ChaseEnemy1->SetPlayerPos(m_pPlayer->GetTransform()->GetPosition());
	if (m_ChaseEnemy2)
		m_ChaseEnemy2->SetPlayerPos(m_pPlayer->GetTransform()->GetPosition());
	if (m_ChaseEnemy3)
		m_ChaseEnemy3->SetPlayerPos(m_pPlayer->GetTransform()->GetPosition());
	if (m_ChaseEnemy4)
		m_ChaseEnemy4->SetPlayerPos(m_pPlayer->GetTransform()->GetPosition());
	if (m_ChaseEnemy5)
		m_ChaseEnemy5->SetPlayerPos(m_pPlayer->GetTransform()->GetPosition());
	if (m_ChaseEnemy6)
		m_ChaseEnemy6->SetPlayerPos(m_pPlayer->GetTransform()->GetPosition());
	if (m_ChaseEnemy7)
		m_ChaseEnemy7->SetPlayerPos(m_pPlayer->GetTransform()->GetPosition());
	if (m_ChaseEnemy8)
		m_ChaseEnemy8->SetPlayerPos(m_pPlayer->GetTransform()->GetPosition());
	if (m_ChaseEnemy9)
		m_ChaseEnemy9->SetPlayerPos(m_pPlayer->GetTransform()->GetPosition());
	if (m_ChaseEnemy10)
		m_ChaseEnemy10->SetPlayerPos(m_pPlayer->GetTransform()->GetPosition());
	if (m_ChaseEnemy11)
		m_ChaseEnemy11->SetPlayerPos(m_pPlayer->GetTransform()->GetPosition());
}

void Stage2::GenerateCubes()
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

void Stage2::UpdateCubes(float deltaTime)
{
	m_addDeltaTime += deltaTime;
	for (int i = 0; i < m_pCubesMesh->GetMesh()->GetInstanceCount(); ++i)
	{
		auto q = Quaternion::CreateFromAxisAngle(m_Axis[i], DirectX::XM_PIDIV2 * m_addDeltaTime * 0.5f);
		m_pCubesMesh->GetMesh()->SetRotation(q, i);
	}
}

void Stage2::UpdateSpriteAndTextPosition()
{
	auto display = m_pGame->GetDisplay();
	auto width = display->getClientSize().width;
	auto height = display->getClientSize().height;
	m_pStartTitleTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 - 250.0f));
	m_pLimitTimeTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, 20.0f));
	m_pPauseTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(50.0f, height - 20.0f));
	m_pPauseSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(50.0f, height - 60.0f));
}
