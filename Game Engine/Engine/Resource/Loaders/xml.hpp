//
//  xml.hpp
//  Game Engine
//
//  Created by Indi Kernick on 3/1/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_resource_loaders_xml_hpp
#define engine_resource_loaders_xml_hpp

#include "../loader.hpp"
#include "../../../Libraries/tinyxml2.hpp"
#include "../Handles/xml.hpp"

namespace Res {
  class XMLLoader final : public Loader {
  public:
    const std::string &getName() const override;
    bool canLoad(std::experimental::string_view ext) const override;
    Handle::Ptr load(const ID &id) const override;
  };
}

#endif
