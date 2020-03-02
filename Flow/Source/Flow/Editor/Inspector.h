#pragma once

namespace Flow
{
	class WorldObject;
	class World;

	class Inspector
	{
	public:
		Inspector();

		void RenderInspector();
		void RenderHeirarchy();

		void SetCurrentWorld(World* WorldReference);
	private:

		World* m_CurrentWorld;
		WorldObject* m_FocusedItem;
	};

}