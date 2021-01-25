#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "Object3d.h"
//#include "ParticleManager.h"
#include "DebugText.h"
#include "Audio.h"
#include "DebugCamera.h"
#include"CollisionPrimitive.h"
#include<vector>
#include<unordered_map>

using namespace std;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public: // メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクル生成
	/// </summary>
	void CreateParticles();

	//当たり判定弾敵
	bool BulletEnemyHit(XMFLOAT3 bulletPos, XMFLOAT3 enemyPos);

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	DebugCamera* camera = nullptr;
	
	bool changeCamera;

	Sprite* spriteBG = nullptr;
	Sprite* spriteCur = nullptr;
	//ParticleManager* particleMan = nullptr;

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelFighter = nullptr;
	Model* modelSphere = nullptr;
	Model* modelSphere2 = nullptr;
	Model* tri = nullptr;
	Model* modelCur = nullptr;

	Model* modelEnemy = nullptr;

	Object3d* objSkydome = nullptr;
	Object3d* objGround = nullptr;
	Object3d* objFighter = nullptr;
	Object3d* objSphere = nullptr;
	Object3d* objSphere2 = nullptr;
	Object3d* objtri = nullptr;
	Object3d* objCur = nullptr;

	Object3d* objEnemy = nullptr;
	Object3d* objMoveEnemy = nullptr;
	Object3d* objMoveLeftEnemy = nullptr;
	Object3d* objMoveRightEnemy = nullptr;

	Sphere sphere1;
	Sphere sphere2;

	Sphere enemy;
	Sphere moveenemy;
	Sphere atkmoveenemy;

	Plane plane;
	Triangle triangle;

	XMMATRIX matWorld;
	XMMATRIX matWorld2;

	bool hit2;

	bool hit3;

	bool enemyhit;

	XMFLOAT3 position2 = { 1,1,0 };

	vector<Object3d*> objects;
	vector<Object3d*> objMoveEnemyList;	//MoveEnemy複数格納用リスト
	vector<XMFLOAT3> objMoveEnemyPosList;	//各MoveEnemyのポジション格納用リスト
	vector<int> shotnumber;//撃たれてる弾の数
	unordered_map<int,XMFLOAT3>bulletVec;

	XMFLOAT3 posZ;

	XMFLOAT3 resetPos;

	int count = 0;//撃たれてる弾の数
	float shot = 0;	
	float shotInterval = 10;//射撃間隔

	const float curZ = 20;	//照準用の仮想Z軸
	XMFLOAT3 curPos;
	XMFLOAT3 targetVec;	//射撃ベクトル

	float curXback = 0;
	float curYback = 0;
	float frontX = 0;//正面X
	float frontY = 0;//正面Y

	float bgX;
	float bgY;

	bool NotKeyAD = false;
	bool NotKeyWS = false;


	//bool screenInCursol = false;
};

