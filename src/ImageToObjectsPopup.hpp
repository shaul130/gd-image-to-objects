#pragma once

#include <Geode/Geode.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>
#include <Geode/ui/TextInput.hpp>
#include "ImageProcessor.hpp"

using namespace geode::prelude;

class ImageToObjectsPopup : public FLAlertLayer {
protected:
    LevelEditorLayer* m_editorLayer;
    CCTextInputNode* m_widthInput;
    CCTextInputNode* m_heightInput;
    CCTextInputNode* m_blockSizeInput;
    std::string m_selectedFilePath;
    CCLabelBMFont* m_previewLabel;
    
    bool init() override;
    void onSelectImage(CCObject*);
    void onImportClicked(CCObject*);
    
public:
    static ImageToObjectsPopup* create();
};
