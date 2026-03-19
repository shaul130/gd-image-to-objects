#include "ImageToObjectsPopup.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

bool ImageToObjectsPopup::init() {
    m_editorLayer = LevelEditorLayer::get();
    if (!m_editorLayer) return false;
    
    if (!FLAlertLayer::init(0.8f)) return false;
    
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto center = winSize / 2;
    
    auto bg = CCScale9Sprite::create("GJ_square01.png");
    bg->setContentSize(CCSize(360, 420));
    bg->setPosition(center);
    m_mainLayer->addChild(bg);
    
    auto title = CCLabelBMFont::create("Image To Objects", "bigFont.fnt");
    title->setScale(0.7f);
    title->setPosition(center.x, center.y + 185);
    m_mainLayer->addChild(title);
    
    auto selectSpr = ButtonSprite::create("Select Image");
    auto selectBtn = CCMenuItemSpriteExtra::create(
        selectSpr, this, menu_selector(ImageToObjectsPopup::onSelectImage)
    );
    
    auto menu = CCMenu::create(selectBtn, nullptr);
    menu->setPosition(center.x, center.y + 130);
    m_mainLayer->addChild(menu);
    
    m_previewLabel = CCLabelBMFont::create("No image selected", "goldFont.fnt");
    m_previewLabel->setScale(0.4f);
    m_previewLabel->setPosition(center.x, center.y + 90);
    m_mainLayer->addChild(m_previewLabel);
    
    auto sizeTitle = CCLabelBMFont::create("Target Size:", "goldFont.fnt");
    sizeTitle->setScale(0.4f);
    sizeTitle->setPosition(center.x - 80, center.y + 45);
    m_mainLayer->addChild(sizeTitle);
    
    m_widthInput = CCTextInputNode::create(50, 30, "50", "bigFont.fnt");
    m_widthInput->setLabelPlaceholderColor(ccc3(0, 0, 0));
    m_widthInput->setPosition(center.x + 20, center.y + 55);
    m_mainLayer->addChild(m_widthInput);
    
    auto xLabel = CCLabelBMFont::create("x", "goldFont.fnt");
    xLabel->setScale(0.4f);
    xLabel->setPosition(center.x + 75, center.y + 45);
    m_mainLayer->addChild(xLabel);
    
    m_heightInput = CCTextInputNode::create(50, 30, "50", "bigFont.fnt");
    m_heightInput->setLabelPlaceholderColor(ccc3(0, 0, 0));
    m_heightInput->setPosition(center.x + 110, center.y + 55);
    m_mainLayer->addChild(m_heightInput);
    
    auto blockTitle = CCLabelBMFont::create("Block Size:", "goldFont.fnt");
    blockTitle->setScale(0.4f);
    blockTitle->setPosition(center.x - 80, center.y);
    m_mainLayer->addChild(blockTitle);
    
    m_blockSizeInput = CCTextInputNode::create(60, 30, "30", "bigFont.fnt");
    m_blockSizeInput->setLabelPlaceholderColor(ccc3(0, 0, 0));
    m_blockSizeInput->setPosition(center.x + 65, center.y + 10);
    m_mainLayer->addChild(m_blockSizeInput);
    
    auto importSpr = ButtonSprite::create("Import Image");
    auto importBtn = CCMenuItemSpriteExtra::create(
        importSpr, this, menu_selector(ImageToObjectsPopup::onImportClicked)
    );
    
    auto importMenu = CCMenu::create(importBtn, nullptr);
    importMenu->setPosition(center.x, center.y - 55);
    m_mainLayer->addChild(importMenu);
    
    auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    auto closeBtn = CCMenuItemSpriteExtra::create(
        closeSpr, this, menu_selector(FLAlertLayer::keyBackClicked)
    );
    auto closeMenu = CCMenu::create(closeBtn, nullptr);
    closeMenu->setPosition(center.x + 165, center.y + 195);
    m_mainLayer->addChild(closeMenu);
    
    return true;
}

ImageToObjectsPopup* ImageToObjectsPopup::create() {
    auto p = new ImageToObjectsPopup();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    delete p;
    return nullptr;
}

void ImageToObjectsPopup::onSelectImage(CCObject*) {
    file::FilePickers::create(
        file::FilePickerMode::Open,
        file::PickerType::Image,
        [this](file::FilePickResult result) {
            if (result.isOk()) {
                m_selectedFilePath = result.getValue();
                std::string filename = m_selectedFilePath;
                size_t pos = filename.find_last_of("/\\");
                if (pos != std::string::npos) {
                    filename = filename.substr(pos + 1);
                }
                m_previewLabel->setString(filename.c_str());
                m_previewLabel->limitLabelWidth(280.0f, 0.4f, 0.1f);
            }
        }
    );
}

void ImageToObjectsPopup::onImportClicked(CCObject*) {
    if (m_selectedFilePath.empty()) {
        FLAlertLayer::create(nullptr, "Please select an image first.", "OK")->show();
        return;
    }
    
    int targetWidth = atoi(m_widthInput->getString());
    int targetHeight = atoi(m_heightInput->getString());
    float blockSize = atof(m_blockSizeInput->getString());
    
    if (targetWidth <= 0 || targetHeight <= 0) {
        FLAlertLayer::create(nullptr, "Invalid target size.", "OK")->show();
        return;
    }
    
    if (blockSize <= 0) blockSize = 30.0f;
    
    auto image = ImageProcessor::loadImage(m_selectedFilePath);
    if (image.width == 0 || image.height == 0) {
        FLAlertLayer::create(nullptr, "Failed to load image.", "OK")->show();
        return;
    }
    
    auto scaled = ImageProcessor::scaleDown(image, targetWidth, targetHeight);
    
    CCArray* createdObjects = CCArray::create();
    
    CCPoint startPos = m_editorLayer->getGridPos(
        ccp(m_editorLayer->getLastObjectX() + 30, 0)
    );
    
    int created = 0;
    for (int y = 0; y < scaled.height; y++) {
        for (int x = 0; x < scaled.width; x++) {
            PixelColor color = scaled.getPixel(x, y);
            if (color.isTransparent()) continue;
            
            auto obj = GameObject::create();
            obj->setPosition(startPos + CCPoint(x * blockSize, -y * blockSize));
            obj->setColor(ccc3(color.r, color.g, color.b));
            obj->setObjectSize(GameObjectSize::kGameObjectSize32);
            
            m_editorLayer->addObject(obj);
            createdObjects->addObject(obj);
            created++;
        }
    }
    
    if (created > 0) {
        auto undoObj = UndoObject::create(createdObjects, UndoCommand::Create);
        m_editorLayer->addToUndoList(undoObj);
    }
    
    char buffer[128];
    snprintf(buffer, sizeof(buffer), 
        "Created <cy>%d</c> objects!\nSize: <cy>%d x %d</c> pixels", 
        created, targetWidth, targetHeight);
    
    FLAlertLayer::create(nullptr, buffer, "OK")->show();
    
    this->onClose(nullptr);
}
