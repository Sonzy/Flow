#pragma once
#include "Component.h"
#include "Flow\Helper\Maths.h"

class btRigidBody;

class FLOW_API WorldComponent : public Component
{
public:
	WorldComponent();
	WorldComponent(const std::string& Name);
	virtual ~WorldComponent();

	virtual void Tick(float DeltaTime) override;

	void AddChild(WorldComponent* Child);

	Vector GetWorldPosition() const;
	Vector GetRelativePosition() const;
	void SetWorldPosition(Vector NewPosition);
	void SetRelativePosition(Vector NewPosition);
	void AddRelativePosition(Vector Position);

	Rotator GetWorldRotation() const;
	Rotator GetRelativeRotation() const;
	void SetWorldRotation(Rotator NewRotation);
	void SetRelativeRotation(Rotator NewRotation);
	void AddRelativeRotation(Rotator Rotation);

	Vector GetWorldScale() const;
	Vector GetRelativeScale() const;
	void SetWorldScale(Vector NewScale);
	void SetRelativeScale(Vector NewScale);

	Transform GetWorldTransform() const;
	Transform GetRelativeTransform() const;
	void SetWorldTransform(Transform NewTransform);
	void SetRelativeTransform(Transform NewTransform);

	virtual void InitialisePhysics();
	virtual btRigidBody* GetRigidBody();


	virtual void Render();

	Vector* GetWriteablePosition();
	Rotator* GetWriteableRotation();
	Vector* GetWriteableScale();

	void DrawInspectionTree(WorldComponent* CurrentInspectedComponent, bool DontOpenTree = false);
	virtual void DrawDetailsWindow(bool bDontUpdate) override;

	std::vector<WorldComponent*> GetChildren() const;

	void SetVisibility(bool Visible);
	bool IsVisible() const;

protected:

	std::vector<WorldComponent*> _Children;
	WorldComponent* _ParentComponent;

	Transform _RelativeTransform;

	bool _Visible = true;

public:
	std::string _Tag;
};