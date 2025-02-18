#include "main_scene.h"
#include "pressuresand.h"

RZUF3_SceneDefinition* MainScene::m_sceneDef = nullptr;
World* MainScene::m_world = nullptr;

RZUF3_SceneDefinition* MainScene::getSceneDef()
{
    if (m_sceneDef != nullptr) return m_sceneDef;

    m_sceneDef = new RZUF3_SceneDefinition();
    m_sceneDef->name = "pressuresand";

    // World view //
    m_world = new World();
    RZUF3_ObjectDefinition objWorldView;
    objWorldView.name = "world_view";

    WorldView* worldView = new WorldView(m_world);
    CellTypesLoader* cellTypesLoader = new CellTypesLoader(CELL_TYPES_DIR);

    objWorldView.scripts = { cellTypesLoader, worldView };
    objWorldView.pos = RZUF3_Pos(
        PICKER_FULL_WIDTH, 0, 
        WORLD_SCALE, WORLD_SCALE
    );
    m_sceneDef->objects.push_back(objWorldView);

    // UI root //
    RZUF3_ObjectDefinition objUIRoot;
    objUIRoot.name = "ui_root";
    m_sceneDef->objects.push_back(objUIRoot);

    // Type picker //
    RZUF3_ObjectDefinition objTypePicker;
    objTypePicker.name = "type_picker";
    objTypePicker.scripts = {
        new TypePicker(worldView)
	};
    objTypePicker.parentName = "ui_root";
    m_sceneDef->objects.push_back(objTypePicker);

    // Brush size label //
    RZUF3_TextRendererOptions brushSizeLabelOptions;
    brushSizeLabelOptions.style.fontFilepath = "assets/fonts/roboto-regular.ttf";
    brushSizeLabelOptions.text = "brush_size";
    brushSizeLabelOptions.useLangFile = true;
    brushSizeLabelOptions.alignment = RZUF3_Align_Left;

    RZUF3_ObjectDefinition objBrushSizeLabel;
    objBrushSizeLabel.name = "brush_size_label";
    objBrushSizeLabel.scripts = {
        new RZUF3_TextRenderer(brushSizeLabelOptions),
		new RZUF3_WindowAnchor({0, 1, 16, -24})
	};
    objBrushSizeLabel.parentName = "ui_root";
    m_sceneDef->objects.push_back(objBrushSizeLabel);

    // Brush size //
    RZUF3_SliderOptions brushSizeSliderOptions;
    brushSizeSliderOptions.min = 1;
    brushSizeSliderOptions.max = 10;
    brushSizeSliderOptions.value = 1;
    brushSizeSliderOptions.alignment = RZUF3_Align_Left;

    RZUF3_ObjectDefinition objBrushSize;
    objBrushSize.name = "brush_size";
    objBrushSize.scripts = {
        new RZUF3_Slider(brushSizeSliderOptions),
        new BrushSizeSlider(worldView->getBrush()),
        new RZUF3_WindowAnchor({0, 1, 100, -24})
    };
    objBrushSize.parentName = "ui_root";
    m_sceneDef->objects.push_back(objBrushSize);

    // author //
    RZUF3_ObjectDefinition objAuthor;
	objAuthor.name = "author";

    RZUF3_TextRendererOptions authorOptions;
    authorOptions.style.fontFilepath = "assets/fonts/roboto-regular.ttf";
    authorOptions.text = "masuo 2024.09";
    authorOptions.useLangFile = false;
    authorOptions.alignment = RZUF3_Align_BottomRight;

    objAuthor.scripts = {
        new RZUF3_TextRenderer(authorOptions),
        new RZUF3_WindowAnchor({1, 1, -16, -16})
    };
    m_sceneDef->objects.push_back(objAuthor);

    return m_sceneDef;
}

void MainScene::destroy()
{
    if (m_sceneDef == nullptr) return;

    for (auto& obj : m_sceneDef->objects)
    {
        for (auto& script : obj.scripts)
        {
            delete script;
        }
    }

    delete m_sceneDef;
    m_sceneDef = nullptr;

    delete m_world;
    m_world = nullptr;
}