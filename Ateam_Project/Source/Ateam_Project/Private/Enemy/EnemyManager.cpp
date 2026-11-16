
#include "Enemy/EnemyManager.h"

AEnemyManager::AEnemyManager()
{
	//位置もTickも不要
	PrimaryActorTick.bCanEverTick = false;
}

//敵が攻撃を当てたことを通知する
void AEnemyManager::NotifyHitPlayer(EEnemyAttackType AttackType)
{
	OnHitPlayer.Broadcast(AttackType);
}