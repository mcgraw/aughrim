// Fill out your copyright notice in the Description page of Project Settings.

#include "Aughrim.h"
#include "SoundDefinitions.h"
#include "ASoundNodeLocalPlayer.h"


void UASoundNodeLocalPlayer::ParseNodes(FAudioDevice* AudioDevice, const UPTRINT NodeWaveInstanceMesh, FActiveSound& ActiveSound, const FSoundParseParameters& ParseParams, TArray<FWaveInstance*>& WaveInstances)
{
	// The access to the pawn will be unsafe once we thread audio, deal with this at that point
	check(IsInGameThread());

	AActor* SoundOwner = ActiveSound.GetAudioComponent() ? ActiveSound.GetAudioComponent()->GetOwner() : nullptr;
	APlayerController* PCOwner = Cast<APlayerController>(SoundOwner);
	APawn* PawnOwner = (PCOwner ? PCOwner->GetPawn() : Cast<APawn>(SoundOwner));

	const bool bLocallyControlled = PawnOwner && PawnOwner->IsLocallyControlled() && Cast<APlayerController>(PawnOwner->Controller);
	const int32 PlayIndex = bLocallyControlled ? 0 : 1;

	if (PlayIndex < ChildNodes.Num() && ChildNodes[PlayIndex])
	{
		ChildNodes[PlayIndex]->ParseNodes(AudioDevice, GetNodeWaveInstanceHash(NodeWaveInstanceMesh, ChildNodes[PlayIndex], PlayIndex), ActiveSound, ParseParams, WaveInstances);
	}
}

void UASoundNodeLocalPlayer::CreateStartingConnectors(void)
{
	InsertChildNode(ChildNodes.Num());
	InsertChildNode(ChildNodes.Num());
}

int32 UASoundNodeLocalPlayer::GetMaxChildNodes() const
{
	return 2;
}

int32 UASoundNodeLocalPlayer::GetMinChildNodes() const
{
	return 2;
}

#if WITH_EDITOR

FText UASoundNodeLocalPlayer::GetInputPinName(int32 PinIndex) const
{
	return (PinIndex == 0) ? NSLOCTEXT("Battle Arena", "InputPinName", "Local") : NSLOCTEXT("Battle Arena", "InputPinName", "Remote");
}

#endif
