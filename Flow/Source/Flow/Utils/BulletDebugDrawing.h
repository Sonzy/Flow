#pragma once

//= Includes ========================================

#include "Bullet\LinearMath\btIDebugDraw.h"
#include "Flow/Rendering/Core/Bindables/IndexBuffer.h"
#include "Flow\Rendering\Core\Line.h"

//= Class Definitions ===============================

class BulletDebugDraw : public btIDebugDraw
{
public:

	//= Public Enums ===================================

	enum class DebugColor
	{
		Active,
		Deactivated,
		WantsDeactivation,
		DisabledDeactivation,
		DisabledSimulation,
		AABB,
		ContactPoint
	};

public:

	//= Public Functions ===================================

	void				Init();

	virtual void		drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	virtual void		drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	virtual void		reportErrorWarning(const char* warningString);
	virtual void		draw3dText(const btVector3& location, const char* textString);
	virtual void		setDebugMode(int debugMode);
	virtual int			getDebugMode() const;

	virtual btIDebugDraw::DefaultColors		getDefaultColors() const override;
	void									SetDebugColor(DebugColor Option, Vector3 Colour);
	btIDebugDraw::DefaultColors&			GetDebugColors();

private:

	//= Private Variables ===================================

	int					m_DebugMode;
	DefaultColors		m_DebugColors;
};