#include "Flowpch.h"
#include "Crate.h"
#include "Flow/GameFramework/Components/StaticMeshComponent.h"

Crate::Crate(const std::string& Name)
	: StaticMeshActor(Name)
{
	GetMeshComponent()->SetMeshAndMaterial("Wabble_Crate", "Mat_Wabble_Props");
}
