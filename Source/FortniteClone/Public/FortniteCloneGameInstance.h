#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "StormActor.h"
#if WITH_GAMELIFTCLIENTSDK
#include "GameLiftClientSDK/Public/GameLiftClientObject.h"
#include "GameLiftClientSDK/Public/GameLiftClientApi.h"
#endif
#include "FortniteCloneGameInstance.generated.h"


UCLASS()
class  UFortniteCloneGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UGameLiftClientObject* GameLiftClientObject;

public:

	virtual void Init() override;

	// Create Game Session ///////////////////////////////////////////////////
	void CreateGameSession();
	UFUNCTION()
	void OnGameCreationSuccess(const FString& GameSessionID);
	UFUNCTION()
	void OnGameCreationFailed(const FString& ErrorMessage);

	// Describe Game Session /////////////////////////////////////////////////
	void DescribeGameSession(const FString& GameSessionID);
	UFUNCTION()
	void OnDescribeGameSessionSuccess(const FString& SessionID, EGameLiftGameSessionStatus SessionState);
	UFUNCTION()
	void OnDescribeGameSessionFailed(const FString& ErrorMessage);

	// Create Player Session /////////////////////////////////////////////////
	void CreatePlayerSession(const FString& GameSessionID, const FString UniquePlayerID);
	UFUNCTION()
	void OnPlayerSessionCreateSuccess(const FString& IPAddress, const FString& Port, const FString& PlayerSessionID);
	UFUNCTION()
	void OnPlayerSessionCreateFail(const FString& ErrorMessage);
};