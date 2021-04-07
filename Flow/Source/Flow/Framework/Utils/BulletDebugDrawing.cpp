#include "pch.h"
#include "BulletDebugDrawing.h"
#include "Rendering\Core\DebugDrawing\LineBatcher.h"
#include "GameFramework\World.h"
#include "Framework/Utils/DebugDraw.h"

void BulletDebugDraw::Init()
{
	m_LogDebug = false;
}

void BulletDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	DebugDraw::DrawLine(from, to, color);
}

void BulletDebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}

void BulletDebugDraw::reportErrorWarning(const char* warningString)
{
	FLOW_ENGINE_WARNING(warningString);
}

void BulletDebugDraw::draw3dText(const btVector3& location, const char* textString)
{
}

void BulletDebugDraw::setDebugMode(int debugMode)
{
	m_DebugMode |= debugMode;
}

int BulletDebugDraw::getDebugMode() const
{
	return m_DebugMode;
}

btIDebugDraw::DefaultColors BulletDebugDraw::getDefaultColors() const
{
	return m_DebugColors;
}

void BulletDebugDraw::SetDebugColor(BulletDebugDraw::DebugColor Option, Vector3 Color)
{
	switch (Option)
	{
	case DebugColor::Active:
		m_DebugColors.m_activeObject = Color;
		break;
	case DebugColor::Deactivated:
		m_DebugColors.m_activeObject = Color;
		break;
	case DebugColor::WantsDeactivation:
		m_DebugColors.m_activeObject = Color;
		break;
	case DebugColor::DisabledDeactivation:
		m_DebugColors.m_activeObject = Color;
		break;
	case DebugColor::DisabledSimulation:
		m_DebugColors.m_activeObject = Color;
		break;
	case DebugColor::AABB:
		m_DebugColors.m_activeObject = Color;
		break;
	case DebugColor::ContactPoint:
		m_DebugColors.m_activeObject = Color;
		break;
	default:
		FLOW_ENGINE_ERROR("BulletDebugDraw::SetDebugColor: Case Error");
		break;
	}
}

btIDebugDraw::DefaultColors& BulletDebugDraw::GetDebugColors()
{
	return m_DebugColors;
}
