//
//  impl.cpp
//  Game Engine
//
//  Created by Indi Kernick on 24/1/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "impl.hpp"

void Game::HumanViewImpl::init() {
  PROFILE(HumanViewImpl init);

  Game::HumanView::init();
  
  Platform::Window::Desc windowDesc;
  windowDesc.title = "Game Engine";
  windowDesc.size = {1280, 720};
  windowDesc.resizable = false;
  window = app->windowManager->openWindow(windowDesc);
  renderWindow = Platform::createRenderWindow(window, app->root.get());
  scene = app->root->createSceneManager(Ogre::ST_GENERIC);
    
  camera = scene->createCamera("Main Camera");
  camera->setPosition(0, 0, 80);
  camera->lookAt(0, 0, 0);
  camera->setNearClipDistance(5);
  
  viewport = renderWindow->addViewport(camera);
  viewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
  camera->setAspectRatio(
    static_cast<Ogre::Real>(viewport->getActualWidth()) /
    viewport->getActualHeight()
  );
  camera->setAutoAspectRatio(true);
  
  uiRoot = std::make_unique<UI::Root>("test_ui", window, viewport, scene);
  
  Ogre::Entity *ogreEntity = scene->createEntity("ogrehead.mesh");
  ogrehead = scene->getRootSceneNode()->createChildSceneNode();
  ogrehead->attachObject(ogreEntity);
  
  scene->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  
  Ogre::Light *light = scene->createLight("Main Light");
  Ogre::SceneNode *lightNode = scene->getRootSceneNode()->createChildSceneNode();
  lightNode->attachObject(light);
  lightNode->setPosition(20, 80, 50);
  
  /*
  camera = std::make_shared<Scene::Camera>(0xDEADBEEF, glm::mat4());
  Speed speed;
  speed.move = 25.0f;
  speed.vert = 25.0f;
  speed.look = 1.0f;
  controller = std::make_shared<CamControlFlyMouseless>(Bindings(),speed,glm::vec3(0.0f, 0.0f, 100.0f));
  
  mat = glm::translate({}, glm::vec3(0.0f, 30.0f, 25.0f));
  Scene::Light::CommonProps props;
  props.type = Scene::Light::SPHERE;
  props.color = {1.0f, 1.0f, 1.0f};
  props.intensity = 600.0f;
  Scene::Light::Ptr light = std::make_shared<Scene::Light>(42, mat, props);
  
  scene->addChild(mesh);
  scene->addChild(camera);
  scene->addChild(light);
  scene->setActiveCamera(0xDEADBEEF);*/
  
  UI::Button::Ptr button = std::make_shared<UI::Button>();
  {
    UI::AABB bounds;
    bounds.setOrigin(UI::Origin::CENTER);
    bounds.setAspectRatio(2.0f);//400.0f / 200.0f
    bounds.setHeight(0.25f);
    bounds.setSizeSpace(UI::Space::REL);
    bounds.setSizeAxis(UI::Axis::VERT);
    button->setBounds(bounds);
  }
  {
    UI::Button::Textures textures;
    textures.out = "Out";
    textures.down = "Down";
    textures.hover = "Hover";
    button->setTextures(textures);
  }
  
  UI::Paragraph::Ptr caption = std::make_shared<UI::Paragraph>();
  {
    UI::AABB bounds;
    bounds.setOrigin(UI::Origin::TOP_LEFT);
    bounds.setSizeSpace(UI::Space::REL);
    bounds.setSize({1.0f, 1.0f});
    caption->setBounds(bounds);
  }
  caption->setFont("arial_32");
  caption->setText("This text is just long enough");
  caption->setAlign(UI::Paragraph::Align::LEFT);
  caption->setColor({1.0f, 0.0f, 0.0f, 1.0f});
  caption->setHeight(2);
  
  button->addChild(caption);
  
  UI::Button::Ptr otherButton = std::make_shared<UI::Button>();
  {
    UI::AABB bounds;
    bounds.setOrigin(UI::Origin::BOTTOM_RIGHT);
    bounds.setSize(0.5f);
    bounds.setSizeAxis(UI::Axis::BOTH);
    otherButton->setBounds(bounds);
  }
  otherButton->setTextures(button->getTextures());
  button->addChild(otherButton);
  button->onDown([](UI::Button &) {
    std::cout << "Down\n";
  });
  button->onUp([](UI::Button &) {
    std::cout << "Up\n";
  });
  button->onEnter([](UI::Button &) {
    std::cout << "Enter\n";
  });
  button->onLeave([](UI::Button &) {
    std::cout << "Leave\n";
  });
  
  UI::Checkbox::Ptr checkbox = std::make_shared<UI::Checkbox>();
  {
    UI::AABB bounds;
    bounds.setOrigin(UI::Origin::BOTTOM_RIGHT, UI::Origin::BOTTOM_LEFT);
    bounds.setSize(1.0f);
    bounds.setSizeAxis(UI::Axis::BOTH);
    checkbox->setBounds(bounds);
  }
  {
    UI::Checkbox::Textures textures;
    textures.unCheckOut = "Unchecked";
    textures.unCheckHover = textures.unCheckOut;
    textures.unCheckDown = textures.unCheckOut;
    
    textures.checkOut = "Checked";
    textures.checkHover = textures.checkOut;
    textures.checkDown = textures.checkOut;
    checkbox->setTextures(textures);
  }
  checkbox->onCheck([](UI::Checkbox &) {
    std::cout << "Checked\n";
  });
  checkbox->onUncheck([](UI::Checkbox &) {
    std::cout << "Unchecked\n";
  });
  
  button->addChild(checkbox);
  
  UI::Radio::Ptr radio0 = std::make_shared<UI::Radio>(true);
  {
    UI::AABB bounds;
    bounds.setOrigin(UI::Origin::TOP_LEFT, UI::Origin::TOP_RIGHT);
    bounds.setSize(1.0f / 3.0f);
    bounds.setSizeAxis(UI::Axis::BOTH);
    radio0->setBounds(bounds);
  }
  {
    UI::Radio::Textures textures;
    textures.unCheckOut = "Unchecked";
    textures.unCheckHover = textures.unCheckOut;
    textures.unCheckDown = textures.unCheckOut;
    
    textures.checkOut = "Checked";
    textures.checkHover = textures.checkOut;
    textures.checkDown = textures.checkOut;
    radio0->setTextures(textures);
  }
  UI::Radio::Ptr radio1 = std::make_shared<UI::Radio>();
  {
    UI::AABB bounds;
    bounds.setOrigin(UI::Origin::TOP_LEFT, UI::Origin::TOP_RIGHT);
    bounds.setSize(1.0f / 3.0f);
    bounds.setSizeAxis(UI::Axis::BOTH);
    bounds.setPos({0.0f, 1.0f / 3.0f});
    radio1->setBounds(bounds);
  }
  radio1->setTextures(radio0->getTextures());
  UI::Radio::Ptr radio2 = std::make_shared<UI::Radio>();
  {
    UI::AABB bounds;
    bounds.setOrigin(UI::Origin::TOP_LEFT, UI::Origin::TOP_RIGHT);
    bounds.setSize(1.0f / 3.0f);
    bounds.setSizeAxis(UI::Axis::BOTH);
    bounds.setPos({0.0f, 2.0f / 3.0f});
    radio2->setBounds(bounds);
  }
  radio2->setTextures(radio0->getTextures());
  
  button->addChild(radio0);
  button->addChild(radio1);
  button->addChild(radio2);
  
  UI::Image::Ptr image = std::make_shared<UI::Image>();
  {
    UI::AABB bounds;
    bounds.setSizeAxis(UI::Axis::BOTH);
    bounds.setSpace(UI::Space::ABS);
    image->setBounds(bounds);
  }
  image->setTexture("Overlay");
  image->setHeight(10);
  image->setPassthrough(true);
  button->addChild(image);
  
  UI::Button::Ptr triangle = std::make_shared<UI::Button>();
  {
    UI::AABB bounds;
    bounds.setOrigin(UI::Origin::TOP_LEFT, UI::Origin::TOP_RIGHT);
    bounds.setPos({1.0f / 3.0f, 0.0f});
    bounds.setSizeAxis(UI::Axis::VERT);
    triangle->setBounds(bounds);
  }
  {
    UI::Button::Textures textures;
    textures.out = "Green triangle";
    textures.hover = "Blue triangle";
    textures.down = textures.hover;
    triangle->setTextures(textures);
  }
  triangle->setHitRegion({{0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}});
  button->addChild(triangle);
  
  //the above mess is the reason why I planning on creating an XML based data
  //format for describing the UI. The callbacks will probably be written in
  //ChaiScript so that the whole thing is defined in data
  
  uiRoot->setChild(button);
}

void Game::HumanViewImpl::update(uint64_t delta) {
  PROFILE(HumanViewImpl update);
  ogrehead->yaw(Ogre::Radian(0.01f));

  HumanView::update(delta);
}

void Game::HumanViewImpl::quit() {
  PROFILE(HumanViewImpl quit);

  HumanView::quit();
}
