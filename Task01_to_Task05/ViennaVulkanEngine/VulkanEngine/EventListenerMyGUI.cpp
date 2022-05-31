

//Task05-------------------Added by Me-------------------Task05//
#include "VEInclude.h"

namespace ve {

	void EventListenerMyGUI::onDrawOverlay(veEvent event) {
		VESubrender_Nuklear* pSubrender = (VESubrender_Nuklear*)getEnginePointer()->getRenderer()->getOverlay();
		if (pSubrender == nullptr)
			return;

		struct nk_context* ctx = pSubrender->getContext();

		/* GUI */
		std::vector<VESceneNode*> sceneNodes = getSceneManagerPointer()->getSceneNodes();

		if (nk_begin(ctx, "List of all SceneNodes", nk_rect(0, 0, 600, 400), NK_WINDOW_BORDER | NK_WINDOW_MINIMIZABLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_TITLE)) {
			for (VESceneNode* sceneNode : sceneNodes) {
				nk_layout_row_dynamic(ctx, 30, 1);
				if (nk_option_label(ctx, sceneNode->getName().c_str(), selectedNode == sceneNode->getName().c_str())) {
					selectedNode = sceneNode->getName().c_str();

					if (sceneNode->getNodeType() == VESceneNode::VE_NODE_TYPE_SCENENODE) {
						nk_label(ctx, "VE_NODE_TYPE_SCENENODE", NK_TEXT_LEFT);
					}
					else if (sceneNode->getNodeType() == VESceneNode::VE_NODE_TYPE_SCENEOBJECT) {
						nk_label(ctx, "VE_NODE_TYPE_SCENEOBJECT", NK_TEXT_LEFT);

						auto object = dynamic_cast<VESceneObject*>(sceneNode);
						nk_layout_row_dynamic(ctx, 30, 1);
						if (object->getObjectType() == VESceneObject::VE_OBJECT_TYPE_ENTITY) {
							nk_label(ctx, "VE_OBJECT_TYPE_ENTITY", NK_TEXT_LEFT);
						}
						else if (object->getObjectType() == VESceneObject::VE_OBJECT_TYPE_CAMERA) {
							nk_label(ctx, "VE_OBJECT_TYPE_CAMERA", NK_TEXT_LEFT);
						}
						else if (object->getObjectType() == VESceneObject::VE_OBJECT_TYPE_LIGHT) {
							nk_label(ctx, "VE_OBJECT_TYPE_LIGHT", NK_TEXT_LEFT);
						}
					}

					nk_layout_row_dynamic(ctx, 30, 1);
					nk_label(ctx, "Position:", NK_TEXT_LEFT);

					float x = sceneNode->getPosition().x;
					float y = sceneNode->getPosition().y;
					float z = sceneNode->getPosition().z;

					nk_layout_row_static(ctx, 30, 150, 1);
					nk_property_float(ctx, "x:", -100, &x, 100, 1, 1);
					nk_property_float(ctx, "y:", -100, &y, 100, 1, 1);
					nk_property_float(ctx, "z:", -100, &z, 100, 1, 1);

					sceneNode->setPosition(glm::vec3(x, y, z));

					// Light color picker
					auto object = dynamic_cast<VESceneObject*>(sceneNode);
					if (object != nullptr && object->getObjectType() == VESceneObject::VE_OBJECT_TYPE_LIGHT) {
						auto light = dynamic_cast<VELight*>(object);

						nk_layout_row_dynamic(ctx, 30, 1);
						nk_label(ctx, "Ambient light color:", NK_TEXT_LEFT);

						ambientLightColor.r = light->m_col_ambient.r * 255;
						ambientLightColor.g = light->m_col_ambient.g * 255;
						ambientLightColor.b = light->m_col_ambient.b * 255;
						ambientLightColor.a = light->m_col_ambient.a * 255;

						if (nk_combo_begin_color(ctx, ambientLightColor, nk_vec2(nk_widget_width(ctx), 200))) {
							nk_layout_row_dynamic(ctx, 120, 1);
							ambientLightColor = nk_color_picker(ctx, ambientLightColor, NK_RGBA);

							nk_layout_row_static(ctx, 30, 150, 1);
							ambientLightColor.r = (nk_byte)nk_propertyi(ctx, "#R:", 0, ambientLightColor.r, 255, 1, 1);
							ambientLightColor.g = (nk_byte)nk_propertyi(ctx, "#G:", 0, ambientLightColor.g, 255, 1, 1);
							ambientLightColor.b = (nk_byte)nk_propertyi(ctx, "#B:", 0, ambientLightColor.b, 255, 1, 1);
							ambientLightColor.a = (nk_byte)nk_propertyi(ctx, "#A:", 0, ambientLightColor.a, 255, 1, 1);

							light->m_col_ambient.r = ambientLightColor.r / 255.f;
							light->m_col_ambient.g = ambientLightColor.g / 255.f;
							light->m_col_ambient.b = ambientLightColor.b / 255.f;
							light->m_col_ambient.a = ambientLightColor.a / 255.f;

							nk_combo_end(ctx);
						}

						nk_layout_row_dynamic(ctx, 30, 1);
						nk_label(ctx, "Diffuse light color:", NK_TEXT_LEFT);

						diffuseLightColor.r = light->m_col_diffuse.r * 255;
						diffuseLightColor.g = light->m_col_diffuse.g * 255;
						diffuseLightColor.b = light->m_col_diffuse.b * 255;
						diffuseLightColor.a = light->m_col_diffuse.a * 255;

						if (nk_combo_begin_color(ctx, diffuseLightColor, nk_vec2(nk_widget_width(ctx), 200))) {

							nk_layout_row_dynamic(ctx, 120, 1);
							diffuseLightColor = nk_color_picker(ctx, diffuseLightColor, NK_RGBA);

							nk_layout_row_static(ctx, 30, 150, 1);
							diffuseLightColor.r = (nk_byte)nk_propertyi(ctx, "#R:", 0, diffuseLightColor.r, 255, 1, 1);
							diffuseLightColor.g = (nk_byte)nk_propertyi(ctx, "#G:", 0, diffuseLightColor.g, 255, 1, 1);
							diffuseLightColor.b = (nk_byte)nk_propertyi(ctx, "#B:", 0, diffuseLightColor.b, 255, 1, 1);
							diffuseLightColor.a = (nk_byte)nk_propertyi(ctx, "#A:", 0, diffuseLightColor.a, 255, 1, 1);

							light->m_col_diffuse.r = diffuseLightColor.r / 255.f;
							light->m_col_diffuse.g = diffuseLightColor.g / 255.f;
							light->m_col_diffuse.b = diffuseLightColor.b / 255.f;
							light->m_col_diffuse.a = diffuseLightColor.a / 255.f;

							nk_combo_end(ctx);
						}

						nk_layout_row_dynamic(ctx, 30, 1);
						nk_label(ctx, "Specular light color:", NK_TEXT_LEFT);

						specularLightColor.r = light->m_col_specular.r * 255;
						specularLightColor.g = light->m_col_specular.g * 255;
						specularLightColor.b = light->m_col_specular.b * 255;
						specularLightColor.a = light->m_col_specular.a * 255;

						if (nk_combo_begin_color(ctx, specularLightColor, nk_vec2(nk_widget_width(ctx), 200))) {
							nk_layout_row_dynamic(ctx, 120, 1);
							specularLightColor = nk_color_picker(ctx, specularLightColor, NK_RGBA);

							nk_layout_row_static(ctx, 30, 150, 1);
							specularLightColor.r = (nk_byte)nk_propertyi(ctx, "#R:", 0, specularLightColor.r, 255, 1, 1);
							specularLightColor.g = (nk_byte)nk_propertyi(ctx, "#G:", 0, specularLightColor.g, 255, 1, 1);
							specularLightColor.b = (nk_byte)nk_propertyi(ctx, "#B:", 0, specularLightColor.b, 255, 1, 1);
							specularLightColor.a = (nk_byte)nk_propertyi(ctx, "#A:", 0, specularLightColor.a, 255, 1, 1);

							light->m_col_specular.r = specularLightColor.r / 255.f;
							light->m_col_specular.g = specularLightColor.g / 255.f;
							light->m_col_specular.b = specularLightColor.b / 255.f;
							light->m_col_specular.a = specularLightColor.a / 255.f;

							nk_combo_end(ctx);
						}
					}
				}


			}
			nk_end(ctx);
		}
	}
}

//-------------------------------------------------------------//