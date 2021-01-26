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
	safe_delete(spriteCur);
	//safe_delete(particleMan);
	safe_delete(objSkydome);
	safe_delete(objGround);
	safe_delete(objFighter);
	safe_delete(objCur);
	safe_delete(objSphere);
	safe_delete(objSphere2);
	safe_delete(modelSkydome);
	safe_delete(modelGround);
	safe_delete(modelFighter);
	safe_delete(modelCur);
	safe_delete(modelSphere);
	safe_delete(modelSphere2);
	safe_delete(objtri);
	safe_delete(tri);
	safe_delete(modelEnemy);
	safe_delete(objEnemy);
	safe_delete(objMoveEnemy);
	safe_delete(objMoveLeftEnemy);
	safe_delete(objMoveRightEnemy);
	safe_delete(objEnemyBoss);
	safe_delete(modelEnemyBoss);
	for (int i = 0; i < objects.size(); i++) {
		safe_delete(objects[i]);
	}
	for (int i = 0; i < objMoveEnemyList.size(); i++) {
		safe_delete(objMoveEnemyList[i]);
	}
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
	spriteBG = Sprite::Create(1, { bgX,bgY });

	// パーティクルマネージャ生成
	//particleMan = ParticleManager::Create(dxCommon->GetDevice(), camera);

	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/tex1.png");

	Sprite::LoadTexture(3, L"Resources/white1x1.png");
	spriteCur = Sprite::Create(3, { WinApp::window_width / 2 - 50,WinApp::window_height / 2 - 50 });
	spriteCur->SetSize({ 100,100 });
	spriteCur->SetColor({ 1,1,1,0.5f });


	// モデル読み込み
	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelFighter = Model::CreateFromOBJ("chr_sword");
	modelSphere = Model::CreateFromOBJ("F15");
	modelSphere2 = Model::CreateFromOBJ("sphere");
	tri = Model::CreateFromOBJ("triangle");
	modelCur = Model::CreateFromOBJ("Cur");

	modelEnemy = Model::CreateFromOBJ("gripen");
	modelEnemyBoss = Model::CreateFromOBJ("13891_Spaceship_Cargo_Transport_v1_L1");

	// 3Dオブジェクト生成
	objSkydome = Object3d::Create(modelSkydome);
	objGround = Object3d::Create(modelGround);
	objFighter = Object3d::Create(modelFighter);
	objSphere = Object3d::Create(modelSphere);
	objSphere2 = Object3d::Create(modelSphere2);
	for (int i = 0; i < 10; i++) {
		objects.emplace_back(Object3d::Create(modelSphere2));
	}
	objtri = Object3d::Create(tri);
	objCur = Object3d::Create(modelCur);

	objEnemy = Object3d::Create(modelEnemy);
	objEnemyBoss = Object3d::Create(modelEnemyBoss);
	//objMoveEnemy = Object3d::Create(modelEnemy);
	for (int i = 0; i < 3; i++) {
		objMoveEnemyList.emplace_back(Object3d::Create(modelEnemy));	//objMoveEnemyListに複数格納
	}
	objMoveLeftEnemy = Object3d::Create(modelEnemy);
	objMoveRightEnemy = Object3d::Create(modelEnemy);

	objFighter->SetPosition({ +1,0,0 });
	objSphere->SetPosition({ 0,1.0f,0 });
	objSphere->SetRotation({ 0,0,0 });
	objSphere->SetScale({ 0.25f,0.25f,0.25f });
	objtri->SetPosition({ 0,1,0 });
	objtri->SetRotation({ 90,0,-90 });
	objtri->SetScale({ 2,2,2 });
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->SetPosition(objSphere->GetPosition());
		objects[i]->SetScale({ 0.5f,0.5f,0.5f });
	}

	objEnemy->SetPosition({ 0, 1, 15 });
	objEnemy->SetRotation({ 0,-90,0 });
	objEnemy->SetScale({ 0.7f,0.7f,0.7f });

	objEnemyBoss->SetPosition({0, -3, 15});
	objEnemyBoss->SetScale({ 0.025f,0.025f,0.025f });
	objEnemyBoss->SetRotation({ -90,180,0 });

	//objMoveEnemy->SetPosition({ 0,7,15 });
	//objMoveEnemy->SetRotation({ 0,-90,0 });
	//objMoveEnemy->SetScale({ 0.7f,0.7f,0.7f });
	for (int i = 0; i < objMoveEnemyList.size(); i++) {
		objMoveEnemyList[i]->SetPosition({ 0,1 + (float)i,15 });	//objMoveEnemy一体一体に位置など格納
		objMoveEnemyList[i]->SetRotation({ 0,-90,0 });
		objMoveEnemyList[i]->SetScale({ 0.7f,0.7f,0.7f });
	}

	objMoveLeftEnemy->SetPosition({ 17,-1,15 });
	objMoveLeftEnemy->SetScale({ 0.7f,0.7f,0.7f });

	objMoveRightEnemy->SetRotation({ 0,180,0 });
	objMoveRightEnemy->SetPosition({ -17,1,15 });
	objMoveRightEnemy->SetScale({ 0.7f,0.7f,0.7f });

	sphere1.center = XMVectorSet(0, 1, 0, 1);
	sphere1.radius = 0.5f;

	sphere2.center = XMVectorSet(0, 1, 0, 1);
	sphere2.radius = 1.0f;

	enemy.center = XMVectorSet(0, 1, 0, 1);
	enemy.radius = 1.0f;

	moveenemy.center = XMVectorSet(0, 1, 0, 1);
	moveenemy.radius = 1.0f;

	atkmoveenemy.center = XMVectorSet(0, 1, 0, 1);
	atkmoveenemy.radius = 1.0f;

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
	objCur->SetScale(XMFLOAT3(1, 0.2, 0.2));

	hit2 = false;
	hit3 = false;

	enemyhit = false;

	changeCamera = false;

	//screenInCursol = false;
}

void GameScene::Update()
{
	//照準のポジションにZ軸を追加
	curPos = { spriteCur->GetPositon().x,spriteCur->GetPositon().y,curZ };
	//curPos.x = (curPos.x - WinApp::window_width / 2) / (WinApp::window_width / 2);
	//curPos.y = (curPos.y - WinApp::window_height / 2) / (WinApp::window_height / 2);
	curPos.x = curPos.x / WinApp::window_width;
	curPos.x = curPos.x * 2 - 1;
	curPos.y = curPos.y / WinApp::window_height;
	curPos.y = 1 - curPos.y;
	curPos.y = curPos.y * 2 - 1;

	// パーティクル生成
	//CreateParticles();

	camera->Update();
	//particleMan->Update();
	XMFLOAT3 position = objSphere->GetPosition();
	XMFLOAT3 cameraPos = camera->GetTarget();
	XMFLOAT3 enemyPosition = objEnemy->GetPosition();
	//XMFLOAT3 enemyMovePos = objMoveEnemy->GetPosition();
	for (int i = 0; i < objMoveEnemyList.size(); i++) {
		objMoveEnemyPosList.emplace_back(objMoveEnemyList[i]->GetPosition());	//ポジション格納
	}
	XMFLOAT3 enemyMoveLeftPos = objMoveLeftEnemy->GetPosition();
	XMFLOAT3 enemyMoveRightPos = objMoveRightEnemy->GetPosition();
	//XMFLOAT3 position2 = { 1,0,0 };
	objSphere->Update();

	XMFLOAT3 domeRot = objSkydome->GetRotation();
	domeRot.x -= 0.01f;
	domeRot.y += 0.001f;
	domeRot.z += 0.001f;
	objSkydome->SetRotation(domeRot);

	//enemyMovePos.z -= 0.005f;
	//enemyMovePos.y -= 0.0025f;
	//objMoveEnemy->SetPosition(enemyMovePos);
	for (int i = 0; i < objMoveEnemyList.size(); i++) {	//複数更新処理
		objMoveEnemyPosList[i].z -= 0.005f;
		objMoveEnemyPosList[i].y -= 0.0025f;
		objMoveEnemyList[i]->SetPosition(objMoveEnemyPosList[i]);
	}

	enemyMoveLeftPos.x -= 0.005f;
	objMoveLeftEnemy->SetPosition(enemyMoveLeftPos);

	enemyMoveRightPos.x += 0.005f;
	objMoveRightEnemy->SetPosition(enemyMoveRightPos);

	//if (input->PushKey(DIK_K)) {
	//	safe_delete(objSphere2);
	//	safe_delete(modelSphere2);
	//}
	//if (input->PushKey(DIK_L)) {
	//	modelSphere2 = Model::CreateFromOBJ("sphere");
	//	objSphere2 = Object3d::Create(modelSphere2);
	//	objSphere2->SetPosition(position2);
	//	position2.x += 0.2f;
	//}

	if (input->TriggerKey(DIK_X))
	{
		changeCamera = !changeCamera;
	}

	if (changeCamera) {
		debugText.Print("changeCamera TRUE", 50, 400, 1.0f);
		camera->SetTarget({ 0,2,2 });
	}
	else
	{
		camera->SetTarget({ 0, 2, 0 });
	}

	// 球移動
	{
		XMVECTOR moveZ = XMVectorSet(0, 0, 0.01f, 0);
		if (input->PushKey(DIK_W)) { sphere1.center += moveZ; }
		else if (input->PushKey(DIK_S)) { sphere1.center -= moveZ; }
		XMVECTOR moveY = XMVectorSet(0, 0.01f, 0, 0);
		if (input->PushKey(DIK_UP)) { sphere1.center += moveY; }
		else if (input->PushKey(DIK_DOWN)) { sphere1.center -= moveY; }
		XMVECTOR moveX = XMVectorSet(0.01f, 0, 0, 0);
		if (input->PushKey(DIK_D)) { sphere1.center += moveX; }
		else if (input->PushKey(DIK_A)) { sphere1.center -= moveX; }
	}
	XMFLOAT2 curp = spriteCur->GetPositon();
	{
		float curSpeed = 17.0f;
		//if (input->PushKey(DIK_W)) { position.z += 0.01f; }
		//else if (input->PushKey(DIK_S)) { position.z -= 0.01f; }
		if (input->PushKey(DIK_W)) {
			position.y += 0.05f;
			//cameraPos.y += 0.01f; 
			curp.y -= curSpeed;
			curYback -= curSpeed;
			frontY -= curSpeed / 5;
		}
		else if (input->PushKey(DIK_S)) {
			position.y -= 0.05f;
			//cameraPos.y -= 0.01f;
			curp.y += curSpeed;
			curYback += curSpeed;
			frontY += curSpeed / 5;
		}
		else {
			curp.y -= (curYback - frontY) / 20;
			curYback -= (curYback - frontY) / 20;
		}

		if (input->PushKey(DIK_D)) {
			position.x += 0.05f;
			//cameraPos.x += 0.01f;
			curp.x += curSpeed;
			curXback += curSpeed;
			frontX += curSpeed / 3;
		}
		else if (input->PushKey(DIK_A)) {
			position.x -= 0.05f;
			//cameraPos.x -= 0.01f;
			curp.x -= curSpeed;
			curXback -= curSpeed;
			frontX -= curSpeed / 3;
		}
		else {
			curp.x -= (curXback - frontX) / 20;
			curXback -= (curXback - frontX) / 20;
		}
		if (changeCamera) {
			debugText.Print("changeCamera TRUE", 50, 400, 1.0f);
			cameraPos.z = 2;
		}
		else {
			cameraPos.z = 0;
		}
		objSphere->SetPosition(position);
		camera->SetTarget(cameraPos);
	}
	bool up, down, left, right = false;
	Input::MouseMove mouseMove = input->GetMouseMove();


	//if (!right || !left && !up || !down) {
	//	//curp.x += mouseMove.lX*1.0f;
	//	//curp.y += mouseMove.lY*1.0f;
	//	screenInCursol = true;
	//}
	//if (curp.x >= WinApp::window_width - 100) {
	//	curp.x = WinApp::window_width - 100;
	//	right = true;
	//}

	//if (curp.x <= 0) {
	//	curp.x = 0;
	//	left = true;
	//}
	//if (curp.y >= WinApp::window_height - 100) {
	//	curp.y = WinApp::window_height - 100;
	//	down = true;
	//}
	//if (curp.y <= 0) {
	//	curp.y = 0;
	//	up = true;
	//}
	spriteCur->SetPosition({ curp.x,curp.y });

	if (input->PushKey(DIK_B))
	{
		curp.x = WinApp::window_width / 2 - 50;
		curp.y = WinApp::window_height / 2 - 50;
		spriteCur->SetPosition({ curp.x,curp.y });
	}

	//position.z += 0.001f;
	//objSphere->SetPosition(position);


	//弾を撃つ処理
	if (shot > shotInterval) {
		if (shotnumber.size() < objects.size() - 1 && input->PushKey(DIK_SPACE)) {
			shot = 0;
			//if (count < objects.size()) {
			shotnumber.push_back(count);
			objects[shotnumber[shotnumber.size() - 1]]->SetPosition(position);
			targetVec = { (curPos.x + 0.1f) - (position.x / 30) ,(curPos.y - 0.05f) - (position.y / 30) ,curPos.z - position.z };
			bulletVec[shotnumber[shotnumber.size() - 1]] = targetVec;//弾ごとのベクトル

		//}
			if (count == objects.size() - 1) {
				count = 0;
			}
			else {
				count++;
			}
		}
	}

	shot++;
	if (input->ReleaseKey(DIK_SPACE)) {
		shot = shotInterval;
	}
	//弾の更新処理
	if (shotnumber.size() != 0)
	{
		for (int i = 0; i < shotnumber.size(); i++) {
			XMFLOAT3 bulletPos = objects[shotnumber[i]]->GetPosition();
			bulletPos.x += sin(bulletVec[shotnumber[i]].x) * 2.5f;
			bulletPos.y += sin(bulletVec[shotnumber[i]].y) * 1.8f;
			bulletPos.z += bulletVec[shotnumber[i]].z / 10;
			objects[shotnumber[i]]->SetPosition(bulletPos);

			if (BulletEnemyHit(bulletPos, enemyPosition)) {
				//debugText.Print("SphereEnemyHit", 50, 100, 1.0f);
				float rnX = rand() % 10 - 5;
				float rnY = rand() % 5;
				enemyPosition = { rnX,rnY,15 };
				//enemyPosition = { 0,1,15 };
				objEnemy->SetPosition(enemyPosition);
			}

			//if (BulletEnemyHit(bulletPos, enemyMovePos)) {
			//	//debugText.Print("bulletMoveEnemyHit", 50, 50, 1.0f);
			//	float rnX = rand() % 10 - 5;
			//	enemyMovePos = { rnX,7,15 };
			//	objMoveEnemy->SetPosition(enemyMovePos);
			//}
			///

			if (BulletEnemyHit(bulletPos, enemyMoveLeftPos)) {
				float rnY = rand() % 5;
				enemyMoveLeftPos = { 17,rnY,15 };
				objMoveLeftEnemy->SetPosition(enemyMoveLeftPos);
			}

			if (BulletEnemyHit(bulletPos, enemyMoveRightPos)) {
				float rnY = rand() % 5;
				enemyMoveRightPos = { -17,rnY,15 };
				objMoveRightEnemy->SetPosition(enemyMoveRightPos);
			}
			//弾と敵の当たり判定
			//XMVECTOR bulletposition_sub_enemy = XMVectorSet(
			//	bulletPos.x - enemyPosition.x,
			//	bulletPos.y - enemyPosition.y,
			//	bulletPos.z - enemyPosition.z,
			//	0);
			//bulletposition_sub_enemy = XMVector3Length(bulletposition_sub_enemy);
			//float enemyDistance = bulletposition_sub_enemy.m128_f32[0];

			//if (enemyDistance <= enemy.radius + enemy.radius) {
			//	debugText.Print("SphereEnemyHit", 50, 100, 1.0f);
			//	enemyhit = true;
			//}
			///

			//範囲外で消滅
			if (objects[shotnumber[i]]->GetPosition().z > 20) {
				objects[shotnumber[i]]->SetPosition(resetPos);
				shotnumber.erase(shotnumber.begin() + i);
			}
		}
	}

	//複数の弾と敵の当たり判定処理
	if (shotnumber.size() != 0)
	{
		for (int i = 0; i < shotnumber.size(); i++) {
			for (int j = 0; j < objMoveEnemyList.size(); j++)
				if (BulletEnemyHit(objects[shotnumber[i]]->GetPosition(), objMoveEnemyPosList[j])) {
					//debugText.Print("bulletMoveEnemyHit", 50, 50, 1.0f);
					float rnX = rand() % 10 - 5;
					objMoveEnemyPosList[j] = { rnX,7,15 };
					objMoveEnemyList[j]->SetPosition(objMoveEnemyPosList[j]);
				}
		}
	}

	//移動敵の画面外処理
	for (int i = 0; i < objMoveEnemyPosList.size(); i++) {
		if (objMoveEnemyPosList[i].z < 0) {
			float rnX = rand() % 10 - 5;
			objMoveEnemyPosList[i] = { rnX,7,15 };
			//enemyPosition = { 0,1,15 };
			objMoveEnemyList[i]->SetPosition(objMoveEnemyPosList[i]);
		}
	}

	if (enemyMoveLeftPos.x < -18) {
		float rnY = rand() % 5;
		enemyMoveLeftPos = { 17,rnY,15 };
		objMoveLeftEnemy->SetPosition(enemyMoveLeftPos);
	}

	if (enemyMoveRightPos.x > 18) {
		float rnY = rand() % 5;
		enemyMoveRightPos = { -17,rnY,15 };
		objMoveRightEnemy->SetPosition(enemyMoveRightPos);
	}

	//リセットポジションにプレイヤーポジションを更新
	resetPos = position;
	//for (int i = 0; i < shotnumber.size(); i++) {
	//	objects[shotnumber[i]]->SetPosition(resetPos);
	//}

	//if (input->PushKey(DIK_SPACE)) { hit2 = true; }
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


	/*XMVECTOR inter2;
	bool hit3 = Collision::CheckSphere2Triangle(sphere1,
		triangle, &inter2);
	if (hit3) {
		debugText.Print("SphereTriangleHIT", 50, 300, 1.0f);
	}*/

	if (input->PushKey(DIK_SPACE))
	{
		//audio->PlayWave("Resources/Alarm01.wav");
	}


	objSkydome->Update();
	objGround->Update();
	objFighter->Update();
	objSphere->Update();
	objSphere2->Update();
	objtri->Update();
	objCur->Update();
	objEnemy->Update();
	objEnemyBoss->Update();
	//objMoveEnemy->Update();
	for (int i = 0; i < objMoveEnemyList.size(); i++) { //複数更新処理
		objMoveEnemyList[i]->Update();
	}
	objMoveLeftEnemy->Update();
	objMoveRightEnemy->Update();
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->Update();
	}
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//spriteBG->Draw();

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
	//if (!enemyhit) {
	//	objEnemy->Draw();
	//}
	if (objEnemy->GetPosition().z >= -10) {
		objEnemy->Draw();
	}
	for (int i = 0; i < objMoveEnemyList.size(); i++) {	//複数描画処理
		if (objMoveEnemyList[i]->GetPosition().z >= 0) {
			objMoveEnemyList[i]->Draw();
		}
	}
	objMoveLeftEnemy->Draw();
	objMoveRightEnemy->Draw();
	objEnemyBoss->Draw();
	//objtri->Draw();
	//objCur->Draw();
	if (hit2) {
		if (objSphere2 != nullptr)
			objSphere2->Draw();
	}
	for (int i = 0; i < shotnumber.size(); i++) {
		objects[shotnumber[i]]->Draw();
	}
	Object3d::PostDraw();

	// パーティクルの描画
	//particleMan->Draw(cmdList);
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	spriteCur->Draw();

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
bool GameScene::BulletEnemyHit(XMFLOAT3 bulletPos, XMFLOAT3 enemyPos)
{
	//弾と敵の当たり判定
	XMVECTOR bulletposition_sub_enemy = XMVectorSet(
		bulletPos.x - enemyPos.x,
		bulletPos.y - enemyPos.y,
		bulletPos.z - enemyPos.z,
		0);
	bulletposition_sub_enemy = XMVector3Length(bulletposition_sub_enemy);
	float enemyDistance = bulletposition_sub_enemy.m128_f32[0];

	return enemyDistance <= enemy.radius + enemy.radius;
}

