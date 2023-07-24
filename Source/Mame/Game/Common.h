#pragma once

#include <DirectXMath.h>
#include "OperatorXMFLOAT3.h"

// マクロ定数
// const不要の目印
#define NO_CONST 


// 関数ポインタ
// ラジアン変換関数ポインタ
extern float (* const ToRadian)(float); 


// ポインタ消去関数
template <typename T>
inline void SafeDelete(T*& p)
{
    if (!p) return;

    delete p;
    p = nullptr;
}

// XMFLOAT3同士の内積を計算
inline const float XMFloat3Dot(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
{
	return ( (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) );
}

// XMFLOAT3の正規化（単位ベクトル化・標準化）
inline const DirectX::XMFLOAT3 XMFloat3Normalize(const DirectX::XMFLOAT3& vec)
{
	const float length = sqrtf(XMFloat3Dot(vec, vec));

	if (length <= 0.0f) return DirectX::XMFLOAT3(0,0,0);

	return (vec / length);
}

// XMFLOAT3同士の外積を計算(順番に注意)
inline const DirectX::XMFLOAT3 XMFloat3Cross(const DirectX::XMFLOAT3& v1, const DirectX::XMFLOAT3& v2)
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
	return DirectX::XMFLOAT3(fabsf(v.x), fabsf(v.y), fabsf(v.z));
}