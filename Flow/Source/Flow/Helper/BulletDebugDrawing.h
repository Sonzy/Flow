#pragma once
#include "ThirdParty\Bullet\LinearMath\btIDebugDraw.h"
#include "Flow/Rendering/Core/Bindables/IndexBuffer.h"

#include "Flow\Rendering\Core\Line.h"

namespace Flow
{
	class BulletDebugDraw : public btIDebugDraw
	{
	public:

		void Init();

		virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

		virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

		virtual void reportErrorWarning(const char* warningString);

		virtual void draw3dText(const btVector3& location, const char* textString);

		virtual void setDebugMode(int debugMode);

		virtual int getDebugMode() const;

		int DebugMode;

		static Line* Get();

	private:
		static Line* Line_;
	};
}