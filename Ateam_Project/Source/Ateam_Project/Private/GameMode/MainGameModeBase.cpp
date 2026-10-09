
#include "GameMode/MainGameModeBase.h"
#include "Input/InputReceiver.h"

AMainGameModeBase::AMainGameModeBase()
{
    //プレイヤー用Controllerを指定
    PlayerControllerClass = AInputReceiver::StaticClass();

    //レベルに配置したPawnを使用
    DefaultPawnClass = nullptr;
}