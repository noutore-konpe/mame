#include "Collision.h"
#include "OperatorXMFLOAT3.h"
#include "Common.h"

#if 0
#define ADJUST      (0.125f)


// 球と球の交差判定
bool Collision::IntersectSphereVsSphere(
    const DirectX::XMFLOAT3& positionA, const float& radiusA,
    const DirectX::XMFLOAT3& positionB, const float& radiusB,
    DirectX::XMFLOAT3& outPositionB)
{
    // A→Bの単位ベクトルを算出
    const DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    const DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    const DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
    const DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
    float lengthSq = DirectX::XMVectorGetX(LengthSq);

    // 距離判定
    const float range = radiusA + radiusB;    // AとBの半径を足したもの

    if (lengthSq > (range * range)) return false; // 距離よりAB半径が小さければめり込んでいない

    // AがBを押し出す
    NO_CONST DirectX::XMVECTOR Vec_n = DirectX::XMVector3Normalize(Vec); // 正規化
    Vec_n = DirectX::XMVectorScale(Vec_n, range);
    const    DirectX::XMVECTOR OutPositionB = DirectX::XMVectorAdd(PositionA, Vec);
    DirectX::XMStoreFloat3(&outPositionB, OutPositionB);

    return true;
}


bool Collision::IntersectCylinderVsCylinder(
    const DirectX::XMFLOAT3& positionA,
    const float& radiusA,
    const float& heightA,
    const DirectX::XMFLOAT3& positionB,
    const float& radiusB,
    const float& heightB,
    DirectX::XMFLOAT3& outPositionB)
{
    // Aの足元がBの頭より上か、Aの頭がBの足元より下なら当たっていない
    if (positionA.y > positionB.y + heightB) return false;
    if (positionA.y + heightA < positionB.y) return false;


    // XZ平面での範囲チェック
    NO_CONST float vx = positionB.x - positionA.x;
    NO_CONST float vz = positionB.z - positionA.z;
    const float range = radiusA + radiusB;
    const float dist = sqrtf((vx * vx) + (vz * vz));

    if (dist > range) return false;

    // AがBを押し出す
    vx /= dist;
    vz /= dist;

    outPositionB.x = positionA.x + (vx * range);
    outPositionB.y = positionB.y; // ※AではなくB
    outPositionB.z = positionA.z + (vz * range);

    return true;
}


bool Collision::IntersectSphereVsCylinder(
    const DirectX::XMFLOAT3& spherePosition,
    const float& sphereRadius,
    const DirectX::XMFLOAT3& cylinderPosition,
    const float& cylinderRadius,
    const float& cylinderHeight,
    DirectX::XMFLOAT3& outCylinderPosition)
{
    // 弾の足元が円柱の頭より上か、弾の頭が円柱の足元より下なら当たっていない
    const float sphereDown = spherePosition.y - sphereRadius;
    const float sphereUp = spherePosition.y + sphereRadius;
    const float cylinderUp = cylinderPosition.y + cylinderHeight;
    const float cylinderDown = cylinderPosition.y;

    if (sphereDown > cylinderUp) return false;
    if (sphereUp < cylinderDown) return false;


    // XZ平面での範囲チェック
    NO_CONST float vx = cylinderPosition.x - spherePosition.x;
    NO_CONST float vz = cylinderPosition.z - spherePosition.z;
    const float range = sphereRadius + cylinderRadius;
    const float dist = sqrtf(vx * vx + vz * vz);

    if (dist > range) return false;

    // AがBを押し出す
    vx /= dist;
    vz /= dist;

    outCylinderPosition.x = cylinderPosition.x + (vx * range);
    outCylinderPosition.y = cylinderPosition.y;
    outCylinderPosition.z = cylinderPosition.z + (vz * range);

    return true;
}


//bool Collision::IntersectRayVsModel(
//    const DirectX::XMFLOAT3& start, 
//    const DirectX::XMFLOAT3& end, 
//    const Model* model, 
//    HitResult& result)
//{
//    const DirectX::XMVECTOR WorldStart     = DirectX::XMLoadFloat3(&start);
//    const DirectX::XMVECTOR WorldEnd       = DirectX::XMLoadFloat3(&end);
//    const DirectX::XMVECTOR WorldRayVec    = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
//    const DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);
//
//    // ワールド空間のレイの長さ
//    DirectX::XMStoreFloat(&result.distance, WorldRayLength);
//
//    NO_CONST bool hit = false;
//    const ModelResource* resource = model->GetResource();
//    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
//    {
//        // メッシュノード取得
//        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
//
//        // レイをワールド空間からローカル空間へ変換
//        const DirectX::XMMATRIX WorldTransform        = DirectX::XMLoadFloat4x4(&node.worldTransform);
//        const DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);
//
//        const DirectX::XMVECTOR S  = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
//        const DirectX::XMVECTOR E  = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
//        const DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
//        const DirectX::XMVECTOR V  = DirectX::XMVector3Normalize(SE);
//        const DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);
//
//        // レイの長さ
//        NO_CONST float neart =  DirectX::XMVectorGetX(Length);
//
//        // 三角形（面）との交差判定
//        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
//        const std::vector<UINT> indices = mesh.indices;
//
//        NO_CONST int materialIndex = -1;
//        NO_CONST DirectX::XMVECTOR HitPosition = {};
//        NO_CONST DirectX::XMVECTOR HitNormal   = {};
//        for (const ModelResource::Subset& subset : mesh.subsets)
//        {
//            for (UINT i = 0; i < subset.indexCount; i += 3)
//            {
//                const UINT index = subset.startIndex + i;
//                
//                // 三角形の頂点を抽出
//                const ModelResource::Vertex& a = vertices.at(indices.at(index));
//                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
//                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));
//
//                const DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
//                const DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
//                const DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);
//
//                // 三角形の三辺ベクトルを算出
//                const DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
//                const DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
//                const DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);
//
//                // 三角形の法線ベクトルを算出
//                const DirectX::XMVECTOR N  = DirectX::XMVector3Cross(AB, BC);
//
//                // 内積の結果がプラスならば裏向き
//                const DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
//                const float dot = DirectX::XMVectorGetX(Dot);
//
//                // 裏向きなら衝突しない
//                if (dot >= 0.0f) continue; 
//
//
//                // レイと平面の交点を算出
//                const DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
//                const DirectX::XMVECTOR X  = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), Dot);
//                const float x = DirectX::XMVectorGetX(X);
//
//                // 交点までの距離が今までに計算した最近距離より大きいときはスキップ
//                if (x < 0.0f || x > neart) continue;
//
//                // 三角形の交点P
//                const DirectX::XMVECTOR P = DirectX::XMVectorMultiplyAdd(V, X, S);
//
//                // 交点が三角形の内側にあるか判定
//                {
//                    // 1つ目
//                    const DirectX::XMVECTOR PA     = DirectX::XMVectorSubtract(A, P);
//                    const DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
//                    const DirectX::XMVECTOR Dot1   = DirectX::XMVector3Dot(Cross1, N);
//                    const float dot1 = DirectX::XMVectorGetX(Dot1);
//                    if (dot1 < 0.0f) continue;
//
//                    // 2つ目
//                    const DirectX::XMVECTOR PB     = DirectX::XMVectorSubtract(B, P);
//                    const DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
//                    const DirectX::XMVECTOR Dot2   = DirectX::XMVector3Dot(Cross2, N);
//                    const float dot2 = DirectX::XMVectorGetX(Dot2);
//                    if (dot2 < 0.0f) continue;
//
//                    // 3つ目
//                    const DirectX::XMVECTOR PC     = DirectX::XMVectorSubtract(C, P);
//                    const DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
//                    const DirectX::XMVECTOR Dot3   = DirectX::XMVector3Dot(Cross3, N);
//                    const float dot3 = DirectX::XMVectorGetX(Dot3);
//                    if (dot3 < 0.0f) continue;
//                }
//
//                // 最短距離を保存
//                neart = x;
//
//                // 交点と法線を更新
//                HitPosition   = P;
//                HitNormal     = N;
//                materialIndex = subset.materialIndex;
//            }
//        }
//
//        if (materialIndex >= 0)
//        {
//            // ローカル空間からワールド空間へ変換
//            const DirectX::XMVECTOR WorldPosition    = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
//            const DirectX::XMVECTOR WorldCrossVec    = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
//            const DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
//            const float distance = DirectX::XMVectorGetX(WorldCrossLength);
//
//            // ヒット情報保存
//            if (result.distance > distance)
//            {
//                const DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);
//
//                result.distance      = distance;
//                result.materialIndex = materialIndex;
//                DirectX::XMStoreFloat3(&result.position, WorldPosition);
//                DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
//                hit = true;
//            }
//        }
//    }
//
//    return hit;
//}


bool Collision::IntersectAABBVsAABB(
    AABB* aabb1, AABB* aabb2,
    DirectX::XMFLOAT3& resultPos)
{
    const float aabb1Right = aabb1->center.x + aabb1->range.x;
    const float aabb1Left = aabb1->center.x - aabb1->range.x;
    const float aabb1Top = aabb1->center.y + aabb1->range.y;
    const float aabb1Bottom = aabb1->center.y - aabb1->range.y;

    const float aabb2Right = aabb2->center.x + aabb2->range.x;
    const float aabb2Left = aabb2->center.x - aabb2->range.x;
    const float aabb2Top = aabb2->center.y + aabb2->range.y;
    const float aabb2Bottom = aabb2->center.y - aabb2->range.y;

    // 当たっていない
    if (aabb1Right < aabb2Left)   return false;
    if (aabb1Left > aabb2Right)  return false;
    if (aabb1Top < aabb2Bottom) return false;
    if (aabb1Bottom > aabb2Top)    return false;


    if (aabb1Right > aabb2Left || aabb1Left < aabb2Right)
    {
        if (aabb1Right > aabb2Right)
            resultPos.x = aabb2Left - aabb1Right - ADJUST;    // checkRight
        if (aabb1Left < aabb2Left)
            resultPos.x = aabb2Right - aabb1Left - ADJUST;    // checkLeft
    }

    if (aabb1Top > aabb2Bottom || aabb1Bottom < aabb2Top)
    {
        if (aabb1Top > aabb2Bottom)
            resultPos.y = aabb2Bottom - aabb1Top - ADJUST;    // checkTop
        if (aabb1Bottom < aabb2Bottom)
            resultPos.y = aabb2Top - aabb1Bottom - ADJUST;    // checkDown
    }

    return true;
}

#endif


bool Collision::IntersectAABBVsAABB(const AABB& aabb1, const AABB& aabb2)
{
    // X軸方向の衝突判定
    {
        const float box1Right   = aabb1.max.x;
        const float box1Left    = aabb1.min.x;    
        const float box2Right   = aabb2.max.x;
        const float box2Left    = aabb2.min.x;

        const bool isHitX = (box1Right >= box2Left && box1Left <= box2Right);

        if (!isHitX) return false;  // X軸方向での衝突なし
    }

    // Y軸方向の衝突判定
    {
        const float box1Up      = aabb1.max.y;
        const float box1Bottom  = aabb1.min.y;
        const float box2Up      = aabb2.max.y;
        const float box2Bottom  = aabb2.min.y;

        const bool isHitY = (box1Up >= box2Bottom && box1Bottom <= box2Up);

        if (!isHitY) return false;  // Y軸方向での衝突なし
    }

    // Z軸方向の衝突判定
    {
        const float box1Back = aabb1.max.z;
        const float box1Front = aabb1.min.z;
        const float box2Back = aabb2.max.z;
        const float box2Front = aabb2.min.z;

        const bool isHitZ = (box1Back >= box2Front && box1Front <= box2Back);

        if (!isHitZ) return false;  // Z軸方向での衝突なし
    }

    return true;  // 3軸方向での衝突あり
}