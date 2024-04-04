#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LevelPage.hpp>
#include <Geode/modify/InfoLayer.hpp>
#include <Geode/cocos/CCDirector.h>

using namespace geode::prelude;

class $modify(MyLayer, PauseLayer) {
	static void onModify(auto& self) {
        self.setHookPriority("PauseLayer::create", -100);}
    
	static PauseLayer* create(bool p0) {
		auto pl = PauseLayer::create(p0);
		auto winsize = CCDirector::sharedDirector()->getWinSize();
		//auto gm = GameManager::sharedState();
		auto menu = pl->getChildByID("better-pause-node");
		if (menu){

			auto playLayer = PlayLayer::get();
			bool plat = playLayer->m_level->isPlatformer();

			// secondary-menu-buttons
			auto eyes = menu->getChildByID("secondary-menu-buttons");
			eyes->setPosition(CCPoint(winsize.width-20.f, 20.f));
			eyes->setAnchorPoint(CCPoint(0.f, 0.f));
			eyes->setScale(0.7);

			if (Mod::get()->getSettingValue<bool>("show-info")){
				auto infoImage = cocos2d::CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
				//infoImage->setScale(0.6);
				auto infoButton = CCMenuItemSpriteExtra::create(infoImage, pl, menu_selector(MyLayer::onInfoAlert));
				eyes->addChild(infoButton);
			}
			else{
				static_cast<CCNode* >(eyes->getChildren()->objectAtIndex(1))->setPosition(0.f, 0.f);
				
			}
			static_cast<CCNode* >(eyes->getChildren()->objectAtIndex(0))->setVisible(false);
			static_cast<CCNode* >(eyes->getChildren()->objectAtIndex(2))->setVisible(false);
			static_cast<CCNode* >(eyes->getChildren()->objectAtIndex(3))->setVisible(false);

			//font
			auto creator = menu->getChildByID("creator-name");
			auto pointc = creator->getPosition();
			creator->setPosition(CCPoint(pointc.x, pointc.y+4));

			auto star = static_cast<CCNode* >(menu->getChildren()->objectAtIndex(5));
			auto points = star->getPosition();
			star->setPosition(CCPoint(points.x, points.y+4));

			auto volfont = menu->getChildByID("volume-settings-title-label");
			volfont->setAnchorPoint(CCPoint(0.5, 0.f));
			volfont->setPosition(CCPoint(winsize.width-145.f, 280.f));

			//two buttons
			menu->getChildByID("layer-menu-scroll-buttons")->setVisible(false);
			//scroll layer

			float commony = winsize.height-35.f;
			//float commonx = winsize.width/2-244.f > 40 ? winsize.width/2-244.f : 40;
			float commonx = 40.f;
			auto newmenu = CCMenu::create();
			newmenu->setPosition(0.f, 0.f);
			newmenu->setAnchorPoint(CCPoint(0.f, 0.f));
			newmenu->setContentSize(CCSize(80.f, winsize.height));
			newmenu->setZOrder(2);
			newmenu->setID("flukesuki");

			auto resumeButtonImage = cocos2d::CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
			resumeButtonImage->setScale(0.5f);			
			auto resumeButton = CCMenuItemSpriteExtra::create(resumeButtonImage, pl, menu_selector(PauseLayer::onResume));
			resumeButton->setPosition(CCPoint(commonx, commony));
			newmenu->addChild(resumeButton);
			commony -= 45.f;

			auto replayButtonImage = cocos2d::CCSprite::createWithSpriteFrameName("GJ_replayBtn_001.png");
			replayButtonImage->setScale(0.5f);			
			auto replayButton = CCMenuItemSpriteExtra::create(replayButtonImage, pl, menu_selector(PauseLayer::onRestart));			
			replayButton->setPosition(CCPoint(commonx, commony));
			newmenu->addChild(replayButton);
			commony -= 40.f;

			if (PlayLayer::get()->m_level->isPlatformer()) {
				auto replayFullButtonImage = cocos2d::CCSprite::createWithSpriteFrameName("GJ_replayFullBtn_001.png");
				replayFullButtonImage->setScale(0.5f);						
				auto replayFullButton = CCMenuItemSpriteExtra::create(replayFullButtonImage, pl, menu_selector(PauseLayer::onRestartFull));
				replayFullButton->setPosition(CCPoint(commonx, commony));
				newmenu->addChild(replayFullButton);
				commony -= 40.f;
			}

			auto practiceButtonImage = cocos2d::CCSprite::createWithSpriteFrameName(PlayLayer::get()->m_isPracticeMode ? "GJ_normalBtn_001.png" : "GJ_practiceBtn_001.png");
			practiceButtonImage->setScale(0.5f);			
			auto practiceButton = CCMenuItemSpriteExtra::create(practiceButtonImage, pl, (PlayLayer::get()->m_isPracticeMode ? menu_selector(PauseLayer::onNormalMode) : menu_selector(PauseLayer::onPracticeMode)));			
			practiceButton->setPosition(CCPoint(commonx, commony));
			newmenu->addChild(practiceButton);
			commony -= 40.f;

			auto quitButtonImage = cocos2d::CCSprite::createWithSpriteFrameName("GJ_menuBtn_001.png");
			quitButtonImage->setScale(0.5f);				
			cocos2d::SEL_MenuHandler handler;
			#ifdef GEODE_IS_MACOS
				handler = menu_selector(PauseLayer::onQuit);
			#else
				handler = menu_selector(PauseLayer::tryQuit);
			#endif
				auto quitButton = CCMenuItemSpriteExtra::create(quitButtonImage, pl, handler);
				quitButton->setPosition(CCPoint(commonx, commony));
				newmenu->addChild(quitButton);
				commony -= 40.f;

			if (PlayLayer::get()->m_level->m_levelType == GJLevelType::Editor || Mod::get()->getSettingValue<bool>("level-edit")) {
				auto editButtonImage = cocos2d::CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png");
				editButtonImage->setScale(0.38);		
				auto editButton = CCMenuItemSpriteExtra::create(editButtonImage, pl, menu_selector(PauseLayer::onEdit));
				editButton->setPosition(CCPoint(commonx, commony));
				newmenu->addChild(editButton);
				commony -= 40.f;
			}
			if (Mod::get()->getSettingValue<bool>("show-comments")){
				auto showCommentsImage = cocos2d::CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png");
				showCommentsImage->setScale(0.7f);
				auto showCommentsButton = CCMenuItemSpriteExtra::create(showCommentsImage, pl, menu_selector(MyLayer::onLevelInfoLayer));			
				showCommentsButton->setPosition(CCPoint(commonx, 35.f));			
				newmenu->addChild(showCommentsButton);
			}

			menu->addChild(newmenu);
			menu->updateLayout();
		}
		return pl;
	}
	void onLevelInfoLayer(cocos2d::CCObject* pSender) {
		if (PlayLayer::get()) {
			auto infoLayer = InfoLayer::create(PlayLayer::get()->m_level, nullptr, nullptr);
			infoLayer->show();
		}
	}
	void onInfoAlert(cocos2d::CCObject* pSender) {
		static LevelPage* lvlInfo = nullptr;
		if (!lvlInfo) {
			lvlInfo = LevelPage::create(0);
		}
		lvlInfo->m_level = PlayLayer::get()->m_level;
		lvlInfo->onInfo(pSender);
		lvlInfo->m_level = 0;
	}
};