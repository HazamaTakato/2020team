#include "GameScene.h"
#include"Model.h"
#include"Collision.h"
#include <cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	safe_delete(spriteBG);
	//safe_delete(particleMan);
	safe_delete(objSkydome);
	safe_delete(objGround);
	safe_delete(objFighter);
	safe_delete(modelSkydome);
	safe_delete(modelGround);
	safe_delete(modelFighter);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio * audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/background.png")) {
		assert(0);
		return;
	}
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// パーティクルマネージャ生成
	//particleMan = ParticleManager::Create(dxCommon->GetDevice(), camera);

	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/tex1.png");

	// モデル読み込み
	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelFighter = Model::CreateFromOBJ("chr_sword");
	modelSphere = Model::CreateFromOBJ("sphere");
	modelSphere2 = Model::CreateFromOBJ("sphere");
	tri = Model::CreateFromOBJ("triangle");
	modelCur = Model::CreateFromOBJ("Cur");

	// 3Dオブジェクト生成
	objSkydome = Object3d::Create(modelSkydome);
	objGround = Object3d::Create(modelGround);
	objFighter = Object3d::Create(modelFighter);
	objSphere = Object3d::Create(modelSphere);
	objSphere2 = Object3d::Create(modelSphere2);
	objtri = Object3d::Create(tri);
	objCur = Object3d::Create(modelCur);

	objFighter->SetPosition({ +1,0,0 });
	objSphere->SetPosition({ 0,1,0 });
	objSphere2->SetPosition({ -1,1,-1 });
	objtri->SetPosition({ 0,1,0 });
	objtri->SetRotation({90,0,-90});
	objtri->SetScale({ 2,2,2 });

	sphere1.center = XMVectorSet(0, 1, 0, 1);
	sphere1.radius = 1.0f;

	sphere2.center = XMVectorSet(-1, 1, -1, 1);
	sphere2.radius = 1.0f;

	plane.normal = XMVectorSet(0, 1, 0, 0); // 法線ベクトル
	plane.distance = 0.0f; // 原点(0,0,0)からの距離

	// 三角形の初期値を設定
	triangle.p0 = XMVectorSet(-1.0f, 1, -1.0f, 1);//左手前
	triangle.p1 = XMVectorSet(-1.0f, 1, +1.0f, 1);//左奥
	triangle.p2 = XMVectorSet(+1.0f, 1, -1.0f, 1);//右手前
	triangle.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0);//上向き

	// カメラ注視点をセット
	camera->SetTarget({ 0, 2, 0 });
	camera->SetDistance(3.0f);

	objCur->SetRotation(XMFLOAT3(0, 90, 0));
	objCur->SetScale(XMFLOAT3(1,0.2,0.2));

	hit2 = false;
	hit3 = false;
}

void GameScene::Update()
{
	// パーティクル生成
	//CreateParticles();

	camera->Update();
	//particleMan->Update();
	XMFLOAT3 position = objSphere->GetPosition();
	XMFLOAT3 position2 = objSphere2->GetPosition();
	objSphere->Update();
	objSphere2->Update();

	// 球移動
	{
		XMVECTOR moveZ = XMVectorSet(0, 0, 0.01f, 0);
		if (input->PushKey(DIK_W)) { sphere1.center += moveZ;}
		else if (input->PushKey(DIK_S)) { sphere1.center -= moveZ;}
		XMVECTOR moveY = XMVectorSet(0, 0.01f, 0, 0);
		if (input->PushKey(DIK_UP)) {sphere1.center += moveY;}
		else if (input->PushKey(DIK_DOWN)) { sphere1.center -= moveY; }
		XMVECTOR moveX = XMVectorSet(0.01f, 0, 0, 0);
		if (input->PushKey(DIK_D)) { sphere1.center += moveX;}
		else if (input->PushKey(DIK_A)) { sphere1.center -= moveX;}
	}
	{
		if (input->PushKey(DIK_W)) { position.z += 0.01f; }
		else if (input->PushKey(DIK_S)) { position.z -= 0.01f; }
		if (input->PushKey(DIK_UP)) { position.y += 0.01f; }
		else if (input->PushKey(DIK_DOWN)) { position.y -= 0.01f; }
		if (input->PushKey(DIK_D)) { position.x += 0.01f; }
		else if (input->PushKey(DIK_A)) { position.x -= 0.01f; }

		objSphere->SetPosition(position); 
	}
	XMFLOAT3 curp = objCur->GetPosition();
	Input::MouseMove mouseMove = input->GetMouseMove();
	curp.x += mouseMove.lX*0.01f;
	curp.y += -mouseMove.lY*0.01f;
	objCur->SetPosition(curp);

	if (input->PushKey(DIK_B))
	{
		curp.x = 0;
		curp.y = 2;
		objCur->SetPosition(curp);
	}

	//position.z += 0.001f;
	//objSphere->SetPosition(position);

	if(input->PushKey(DIK_SPACE)){ hit2=true;}
	//XMVECTOR inter;
	//bool hit = Collision::CheckSphere2Plane(sphere1, plane, &inter);
	//if (hit) {
	//	debugText.Print("SpherePlaneHIT", 50, 200, 1.0f);
	//}

	//球と球の当たり判定
	XMVECTOR position_sub = XMVectorSet(
		position.x - position2.x,
		position.y - position2.y,
		position.z - position2.z,
		0);

	position_sub = XMVector3Length(position_sub);
	float distance = position_sub.m128_f32[0];

	if (distance <= sphere1.radius + sphere2.radius) {
		debugText.Print("Sphere*2Hit", 50, 100, 1.0f);
		//position.z += 0.001f;
		//objSphere->SetPosition(position);
	}
	else
	{
		debugText.Print("Sphere*2NotHit", 50, 200, 1.0f);
	}
	//

	XMVECTOR inter2;
	bool hit3 = Collision::CheckSphere2Triangle(sphere1,
		triangle, &inter2);
	if (hit3) {
		debugText.Print("SphereTriangleHIT", 50, 300, 1.0f);
	}

	if (input->PushKey(DIK_SPACE))
	{
		audio->PlayWave("Alarm01.wav");
	}


	objSkydome->Update();
	objGround->Update();
	objFighter->Update();
	objSphere->Update();
	objSphere2->Update();
	objtri->Update();
	objCur->Update();
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D描画
	// 3Dオブジェクトの描画
	Object3d::PreDraw(cmdList);
	objSkydome->Draw();
	//objGround->Draw();
	//objFighter->Draw();
	objSphere->Draw();
	objtri->Draw();
	objCur->Draw();
	if (hit2) {
		objSphere2->Draw();
	}
	Object3d::PostDraw();

	// パーティクルの描画
	//particleMan->Draw(cmdList);
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>


	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScene::CreateParticles()
{
	for (int i = 0; i < 10; i++) {
		// X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		// 追加
		//particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
	}
}
