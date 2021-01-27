#pragma once

// Includes /////////////////////////////////////////////////

#include <string>
#include <typeinfo>
#include "Flow\Core.h"
#include "Typedefs.h"

// TypeDefs /////////////////////////////////////////////////

namespace YAML { class Emitter; }
namespace YAML { class Node; }

// Class Definiton /////////////////////////////////////////////////

class FLOW_API GameObject
{
public:

	// Public Functions /////////////////////////////////////////////////

							GameObject();
							GameObject(const std::string& Name);
	virtual					~GameObject();

	//= Editor Interface =

#if WITH_EDITOR
	virtual void			EditorBeginPlay() {};
	virtual void			OnViewportSelected() {};
	virtual void			OnViewportDeselected() {};
#endif

	virtual void			Tick(float DeltaTime);

	//To be overriden, assumes Imgui::Begin and Imgui::End are being handled externally. Draws widgets for itself in here
	//Typically DontUpdate is true if we have swapped objects, since it means that the references for object properties
	//to update are incorrect, so we shouldnt write to any values
	virtual void			DrawDetailsWindow(bool bDontUpdate);
	const std::string&		GetName() const;
	std::string&			GetWritableName();
	void					SetName(const std::string& NewName);

	FGUID					GetGuid() const			{ return m_guid; }
	void					SetGuid(FGUID guid)		{ m_guid = guid; }


	//= Serialisation =

	virtual void			Serialize(YAML::Emitter& Archive);
	virtual void			Deserialize(YAML::Node& Archive);

	virtual const char*		GetClassNameID() const  { return typeid(GameObject).name(); }
	virtual const char*		GetStaticName() const { return "GameObject"; }

	// If this is owned, dont handle it's own saving and loading. Something else will manage it
	bool					IsOwned() const			{ return m_owned; }
	void					SetOwned(bool owned)	{ m_owned = owned; }

protected:

	// Protected Variables /////////////////////////////////////////////////

	std::string				m_name;
	FGUID					m_guid;
	bool					m_owned;
};