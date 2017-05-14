//
//  radio.hpp
//  Game Engine
//
//  Created by Indi Kernick on 25/3/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_game_view_human_radio_hpp
#define engine_game_view_human_radio_hpp

#include "element.hpp"
#include <functional>
#include "../../Utils/callable.hpp"

namespace UI {
  ///Similar to checkbox but mutually exclusive with its sibling radios
  class Radio final : public Element {
  public:
    using Ptr = std::shared_ptr<Radio>;
  
    enum class State : uint8_t {
      UNCHECK_DOWN_OUT,
      UNCHECK_OUT,
      UNCHECK_HOVER,
      UNCHECK_DOWN,
      
      CHECK_DOWN_OUT,
      CHECK_OUT,
      CHECK_HOVER,
      CHECK_DOWN
    };
    
    using ChangeListener = std::function<void (Radio &, State, State)>;
    
    class CallListeners {
    public:
      using Listener = std::function<void (Radio &)>;
      
      CallListeners(const Listener &, const Listener &);
      
      void operator()(Radio &, State, State) const;
    
    private:
      Listener unCheck, check;
      
      static void defaultListener(Radio &) {}
    };
    
    class SetTextures {
    private:
      using StringRef = const std::string &;
    
    public:
      SetTextures(StringRef, StringRef, StringRef, StringRef, StringRef, StringRef);
      
      void operator()(Radio &, State, State) const;
    
    private:
      std::string unCheckOut, unCheckHover, unCheckDown,
                  checkOut, checkHover, checkDown;
    };
    
    Radio() = default;
    explicit Radio(bool);
    ~Radio() = default;
    
    void onStateChange(const ChangeListener &);
    static bool isChecked(State);
    
  private:
    ChangeListener stateChange = defaultListener;
    State state = State::UNCHECK_OUT;
    
    static void defaultListener(Radio &, State, State) {}
    static State makeCheckedIf(bool, State);
    static State makeUnchecked(State);
    void changeState(State);
    
    void onMouseDown() override;
    void onMouseUp(bool) override;
    void onMouseEnter(bool) override;
    void onMouseLeave(bool) override;
  };
};

template <typename LEFT, typename RIGHT>
std::enable_if_t<
  is_callable_v<LEFT,  UI::Radio &, UI::Radio::State, UI::Radio::State> &&
  is_callable_v<RIGHT, UI::Radio &, UI::Radio::State, UI::Radio::State>,
  UI::Radio::ChangeListener
>
operator&&(LEFT left, RIGHT right) {
  return [
    left = UI::Radio::ChangeListener(left),
    right = UI::Radio::ChangeListener(right)
  ] (
    UI::Radio &radio,
    UI::Radio::State fromState,
    UI::Radio::State toState
  ) {
    left(radio, fromState, toState);
    right(radio, fromState, toState);
  };
}

#endif
