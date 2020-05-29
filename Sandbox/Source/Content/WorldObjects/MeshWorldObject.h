#pragma once
#include "Flow\GameFramework\WorldObject.h"

class StaticMeshComponent;

/* WorldObject with a single static mesh component */
class MeshWorldObject : public WorldObject
{
public:
	MeshWorldObject();
	MeshWorldObject(const std::string& NewName);
	MeshWorldObject(const std::string& NewName, const std::string& MeshName, const std::string& MaterialName);
	~MeshWorldObject() = default;

	[[nodiscard]] StaticMeshComponent* GetMeshComponent() const;

	void InitPhysics(bool Simulate);

protected:

	std::string MeshName;
	std::string MaterialName;

	std::shared_ptr<StaticMeshComponent> MeshComponent;


};