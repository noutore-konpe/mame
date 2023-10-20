#pragma once

#include <DirectXMath.h>
#include <cmath>
#include "OperatorXMFloat3.h"

// XMFLOAT3同士の内積を計算
inline const float XMFloat3Dot(
	const DirectX::XMFLOAT3& v1,
	const DirectX::XMFLOAT3& v2)
{
	return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}

// XMFLOAT3同士の距離を計算
inline const float XMFloat3Length(const DirectX::XMFLOAT3& v)
{
	return ::sqrtf(::XMFloat3Dot(v, v));
}

// XMFLOAT3同士の距離の二乗を計算
inline const float XMFloat3LengthSq(const DirectX::XMFLOAT3& v)
{
	return ::XMFloat3Dot(v, v);
}

// XMFLOAT3の正規化（単位ベクトル化・標準化）
inline const DirectX::XMFLOAT3 XMFloat3Normalize(const DirectX::XMFLOAT3& v)
{
	const float length = ::XMFloat3Length(v);

	if (length <= 0.0f) return DirectX::XMFLOAT3(0, 0, 0);

	return (v / length);
}

// XMFLOAT3の正規化（単位ベクトル化・標準化）
// ※おまけでlengthも取得できるバージョン
inline const DirectX::XMFLOAT3 XMFloat3Normalize(
	const DirectX::XMFLOAT3& v, float* length)
{
	// 正規化ベクトルを求めるためにlengthを求めるのでそれをついでに取得する
	(*length) = ::XMFloat3Length(v);

	if ((*length) <= 0.0f) return DirectX::XMFLOAT3(0, 0, 0);

	return (v / (*length));
}

// XMFLOAT3同士の外積を計算(順番に注意)
inline const DirectX::XMFLOAT3 XMFloat3Cross(
	const DirectX::XMFLOAT3& v1,
	const DirectX::XMFLOAT3& v2)
{
	const DirectX::XMFLOAT3 cross = {
		(v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x),
	};
	return cross;
}

// XMFLOAT3の絶対値を求める
inline const DirectX::XMFLOAT3 XMFloat3Abs(const DirectX::XMFLOAT3& v)
{
	return DirectX::XMFLOAT3(::fabsf(v.x), ::fabsf(v.y), ::fabsf(v.z));
}
