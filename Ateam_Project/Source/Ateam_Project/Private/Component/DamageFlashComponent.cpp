
#include "Component/DamageFlashComponent.h"
#include "Components/MeshComponent.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"

namespace
{
	//点滅の最大値
	static constexpr float FlashMaxValue = 1.0f;
	//点滅の最小値
	static constexpr float FlashMinValue = 0.0f;
	//波(-1〜1)を0〜1に変換するための係数
	static constexpr float WaveScale = 0.5f;
	//波(-1〜1)を0〜1に変換するための加算値
	static constexpr float WaveOffset = 1.0f;
	//円周率
	static constexpr float PiValue = 3.14159265f;
	//1周期あたりの角度
	static constexpr float CycleAngle = PiValue * 2.0f;
}

UDamageFlashComponent::UDamageFlashComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//再生中だけTickする
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

//ゲーム開始時にオーナーの全メッシュの全スロットへDMIを作成する。
void UDamageFlashComponent::BeginPlay()
{
	Super::BeginPlay();

	const AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		return;
	}

	TArray<UMeshComponent*> Meshes = {};
	OwnerActor->GetComponents<UMeshComponent>(Meshes);

	for (UMeshComponent* Mesh : Meshes)
	{
		const int32 NumMaterials = Mesh->GetNumMaterials();
		for (int32 Index = 0; Index < NumMaterials; ++Index)
		{
			UMaterialInstanceDynamic* DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamic(Index);
			if (DynamicMaterial == nullptr)
			{
				continue;
			}

			DynamicMaterial->SetVectorParameterValue(FlashColorParamName, FlashColor);
			DynamicMaterial->SetScalarParameterValue(FlashParamName, FlashMinValue);
			DynamicMaterials.Add(DynamicMaterial);
		}
	}
}

//点滅を開始する
void UDamageFlashComponent::PlayFlash()
{
	Elapsed = 0.0f;
	bIsPlaying = true;
	SetFlashValue(FlashMaxValue);
	SetComponentTickEnabled(true);
}

//点滅の値を設定した速さで増減させ設定した時間で終了する
void UDamageFlashComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsPlaying)
	{
		return;
	}

	Elapsed += DeltaTime;

	//設定した時間を過ぎたら点滅を消して終了する
	if (Elapsed >= FlashDuration)
	{
		SetFlashValue(FlashMinValue);
		bIsPlaying = false;
		SetComponentTickEnabled(false);
		return;
	}

	//経過時間と速さから現在の角度を求める
	const float Angle = Elapsed * FlashSpeed * CycleAngle;

	//Cosの波を0〜1に変換する
	const float Wave = (FMath::Cos(Angle) + WaveOffset) * WaveScale;
	SetFlashValue(FMath::Lerp(FlashMinValue, FlashMaxValue, Wave));
}

//全ダイナミックマテリアルに値を設定する
void UDamageFlashComponent::SetFlashValue(float Value)
{
	for (UMaterialInstanceDynamic* DynamicMaterial : DynamicMaterials)
	{
		if (DynamicMaterial != nullptr)
		{
			DynamicMaterial->SetScalarParameterValue(FlashParamName, Value);
		}
	}
}