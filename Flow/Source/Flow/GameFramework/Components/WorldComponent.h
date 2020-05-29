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

	//TODO: Probs not safe lool
	Vector* GetWriteablePosition();
	Rotator* GetWriteableRotation();
	Vector* GetWriteableScale();

	std::vector<Component*> GetChildren() const;

	void SetVisibility(bool Visible);
	bool IsVisible() const;

protected:

	std::vector<Component*> _Children;
	Component* _ParentComponent;

	Transform _RelativeTransform;

	bool _Visible = true;
};