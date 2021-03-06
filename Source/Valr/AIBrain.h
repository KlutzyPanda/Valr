// Code by Sultan Iljasov, 2020.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FightManager.h"
#include "ANN/ArtificialNN.h"

#include "AIBrain.generated.h"

class Replay
{
public:
	TArray<double> States;	//Save this
	double Reward = 0;

	Replay(double s1, double s2, double s3, double s4, double s5, double s6, float r)
	{
		States.Add(s1);
		States.Add(s2);
		States.Add(s3);
		States.Add(s4);
		States.Add(s5);
		States.Add(s6);
		Reward = r;
	}
};

UCLASS()
class VALR_API AAIBrain : public AAIController
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
		AFighterPawn* Fighter = nullptr;

	ArtificialNN* Network = nullptr;

	double currentReward = 0.0f;						//reward to associate with actions
	TArray<Replay*> replayMemory;					//memory - list of past actions and rewards
	int maxCapacity = 5000;						//memory capacity

	float Discount = 0.99f;						//how much future states affect rewards
	float exploreRate = 100.f;					//chance of picking random action
	float maxExploreRate = 100.0f;					//max chance value
	float minExploreRate = 0.01f;					//min chance value
	float exploreDecay = 0.01f;					//chance decay amount for each update

	float Timer = 0;						//timer to keep track of balancing

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
		AFightManager* Manager = nullptr;

	uint8 enemyHealth = 0;
	int attackTimer = 0;
	int holdMaxQIndex = 0;
	int record = 0;

public:
	AAIBrain();

	TArray<double> SoftMax(TArray<double> values);

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

};