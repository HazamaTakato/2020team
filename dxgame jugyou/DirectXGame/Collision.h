#pragma once

#include "CollisionPrimitive.h"

/// <summary>
/// 当たり判定ヘルパークラス
/// </summary>
class Collision
{
public:
	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">交点（平面上の再接近点）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR *inter = nullptr);

	///点と三角形の最近接点を求める
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point,
		const Triangle& triangle, DirectX::XMVECTOR* closest);

	///球と法線付き三角形の当たりチェック
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
		DirectX::XMVECTOR* inter = nullptr);

	///レイと平面の当たり判定
	//static bool CheckLay2Plane(const Lay& lay, const Plane& plane,
	//	float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
};

