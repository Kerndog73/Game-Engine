//
//  input.hpp
//  Game Engine
//
//  Created by Indi Kernick on 19/3/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_game_view_human_input_hpp
#define engine_game_view_human_input_hpp

#include "element.hpp"
#include "../../Event/manager.hpp"
#include "../../Utils/member function.hpp"
#include "../../Input/event.hpp"
#include "aabb stack.hpp"
#include "height stack.hpp"
#include "../../Application/base.hpp"
#include "../../Utils/point in polygon.hpp"
#include "../../Math/vectors.hpp"
#include "../../Utils/safe down cast.hpp"
#include "conversions.hpp"

namespace UI {
  class Input {
  public:
    explicit Input(std::weak_ptr<Platform::Window>);
    ~Input();
    
    void setRoot(Element::Ptr);
    void unSetRoot();
    
  private:
    std::weak_ptr<Platform::Window> window;
    Game::EventManager::ListenerID mouseDownID, mouseUpID, mouseMoveID;
    Element::Ptr root;
    Element::Ptr lastFocused;
    Element::Ptr downElement;
    bool mouseDown = false;
    
    void onMouseDown(::Input::MouseDown::Ptr);
    void onMouseUp(::Input::MouseUp::Ptr);
    void onMouseMove(::Input::MouseMove::Ptr);
    
    void handleMouseDown(Element::Ptr);
    void handleMouseUp(Element::Ptr);
    void handleMouseMove(Element::Ptr, Point, Point);
    
    void triggerMouseMove(Element::Ptr, Point, Point);
    
    static bool withinHitRegion(Element::Ptr, Bounds, Point);
    
    struct AbsBounds {
      Bounds thisBounds;
      Bounds parentBounds;
    };
    
    AbsBounds getAbsBounds(Element::Ptr);
    static AbsBounds getAbsBoundsHelper(Element &, AABBStack &);
    
    template <typename T>
    Element::Ptr getFocused(std::shared_ptr<T>);
    void getTopElement(
      Element::Ptr,
      Element::Ptr &,
      Height &,
      const Point,
      AABBStack &,
      HeightStack &
    );
  };
}

#endif
