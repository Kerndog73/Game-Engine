//
//  checkbox.hpp
//  Game Engine
//
//  Created by Indi Kernick on 20/3/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_game_view_human_checkbox_hpp
#define engine_game_view_human_checkbox_hpp

#include "static state element.hpp"

namespace UI {
  ///A checkbox that is either checked or not checked
  class Checkbox final : public StaticStateElement<2> {
  public:
    using Ptr = std::shared_ptr<Checkbox>;
    
    static constexpr StateElement::SubState UNCHECKED_STATE = 0;
    static constexpr StateElement::SubState CHECKED_STATE = 1;
    
    explicit Checkbox(const std::string &, bool = false);
    
    bool isChecked() const;
    bool isUnchecked() const;
    void check();
    void uncheck();
  };
};

#endif
