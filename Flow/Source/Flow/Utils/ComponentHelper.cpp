#include "pch.h"
#include "ComponentHelper.h"

void ComponentHelper::BuildComponentArray(WorldComponent* root, std::vector<WorldComponent*>& outArray)
{
	ComponentHelper::ComponentRecursion([&outArray](WorldComponent* comp) {	outArray.push_back(comp); }, root);
}
