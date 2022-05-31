

//Task05-------------------Added by Me-------------------Task05//
#ifndef EVENTLISTENERMYGUI_H
#define EVENTLISTENERMYGUI_H

// Event listener for GUI
// Lists all VESceneNode in the scene
// When selecting any object in the list :
// Display its Type as text
// Position as editable value(beware that getPosition only returns a copy of the data!)
// If it’s a light
// Display light colors and make them editable via a color picker.

namespace ve {
	class EventListenerMyGUI : public VEEventListener {
	protected:
		std::string selectedNode;

		struct nk_color ambientLightColor;
		struct nk_color diffuseLightColor;
		struct nk_color specularLightColor;

		virtual void onDrawOverlay(veEvent event);

	public:
		EventListenerMyGUI(std::string name) : VEEventListener(name) {};
		virtual ~EventListenerMyGUI() {};
	};
}

#endif
//-------------------------------------------------------------//
