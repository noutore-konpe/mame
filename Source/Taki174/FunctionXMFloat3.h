#pragma once

#include <DirectXMath.h>
#include <cmath>
#include "OperatorXMFloat3.h"

// XMFLOAT3���m�̓��ς��v�Z
inline const float XMFloat3Dot(
	const DirectX::XMFLOAT3& v1,
	const DirectX::XMFLOAT3& v2)
{
	return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}

// XMFLOAT3���m�̋������v�Z
inline const float XMFloat3Length(const DirectX::XMFLOAT3& v)
{
	return ::sqrtf(::XMFloat3Dot(v, v));
}

// XMFLOAT3���m�̋����̓����v�Z
inline const float XMFloat3LengthSq(const DirectX::XMFLOAT3& v)
{
	return ::XMFloat3Dot(v, v);
}

// XMFLOAT3�̐��K���i�P�ʃx�N�g�����E�W�����j
inline const DirectX::XMFLOAT3 XMFloat3Normalize(const DirectX::XMFLOAT3& v)
{
	const float length = ::XMFloat3Length(v);

	if (length <= 0.0f) return DirectX::XMFLOAT3(0, 0, 0);

	return (v / length);
}

// XMFLOAT3�̐��K���i�P�ʃx�N�g�����E�W�����j
// �����܂���length���擾�ł���o�[�W����
inline const DirectX::XMFLOAT3 XMFloat3Normalize(
	const DirectX::XMFLOAT3& v, float* length)
{
	// ���K���x�N�g�������߂邽�߂�length�����߂�̂ł�������łɎ擾����
	(*length) = ::XMFloat3Length(v);

	if ((*length) <= 0.0f) return DirectX::XMFLOAT3(0, 0, 0);

	return (v / (*length));
}

// XMFLOAT3���m�̊O�ς��v�Z(���Ԃɒ���)
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

// XMFLOAT3�̐�Βl�����߂�
inline const DirectX::XMFLOAT3 XMFloat3Abs(const DirectX::XMFLOAT3& v)
{
	return DirectX::XMFLOAT3(::fabsf(v.x), ::fabsf(v.y), ::fabsf(v.z));
}
