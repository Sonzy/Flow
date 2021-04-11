#pragma once

#if WITH_EDITOR

//= Includes =================================

#include "Framework/Layers/Layer.h"
#include "Framework/Events/KeyEvent.h"
#include "Framework/Events/MouseEvent.h"
#include "Framework/Events/ApplicationEvent.h"

//= Class Definition =========================

enum class ImGuiTheme
{
	Unity = 0,
	Slate = 1,
	RayTeak = 2
};

class FLOW_API ImGuiLayer : public Layer
{
public:

	//= Public Functions ===================================

					ImGuiLayer();
					~ImGuiLayer();

	virtual void	OnAttach() override;
	virtual void	OnDetach() override;
	virtual void	OnImGuiRender(bool DrawEditor) override;
	void			OnEvent(Event& e) override;

	void			Begin();
	void			End();

	static void		SetTheme(ImGuiTheme theme);

private:

	//= Private Functions =================================

	bool			OnMouseButtonPressed(MouseButtonEvent& e);
	bool			OnMouseButtonReleased(MouseButtonReleasedEvent& e);
	bool			OnMouseMoved(MouseMovedEvent& e);
	bool			OnMouseScrolled(MouseScrolledEvent& e);
	bool			OnKeyPressed(KeyPressedEvent& e);
	bool			OnKeyTyped(KeyTypedEvent& e);
	bool			OnKeyReleased(KeyReleasedEvent& e);
	bool			OnWindowResized(WindowResizedEvent& e);

	//= Themes =

	static void		SetBaseTheme();
	static void		SetUnityTheme();
	static void		SetSlateTheme();
	static void		SetRayTeakTheme();

private:

	//= Private Variables ==================================

	const float		m_MenuBarSize = 20.0f;
	static ImGuiTheme m_theme;
};

#endif // WITH_EDITOR