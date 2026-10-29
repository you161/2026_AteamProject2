
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameJudge.generated.h"

UCLASS()
class ATEAM_PROJECT_API AGameJudge : public AActor
{
	GENERATED_BODY()
	
public:	

	AGameJudge();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

};