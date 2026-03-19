#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include "ImageToObjectsPopup.hpp"

using namespace geode::prelude;

class $modify(EditorUI) {
    void onImportImage(CCObject* sender) {
        auto popup = ImageToObjectsPopup::create();
        popup->show();
    }

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        auto editorBtnMenu = this->getChildByType<CCMenu>(0);
        if (!editorBtnMenu) return true;

        auto importSpr = CircleButtonSprite::createWithSpriteFrameName("GJ_uploadBtn_001.png", 1.0f, CircleBaseColor::Green, CircleBaseSize::Medium);
        auto importBtn = CCMenuItemSpriteExtra::create(
            importSpr,
            this,
            menu_selector($modify::onImportImage)
        );
        importBtn->setID("import-image-btn");
        importBtn->setScale(0.7f);

        editorBtnMenu->addChild(importBtn);
        editorBtnMenu->updateLayout();

        return true;
    }
};

$execute {
    listenForInstall([](Mod* mod) {
        log::info("Image To Objects mod installed!");
    });
}
