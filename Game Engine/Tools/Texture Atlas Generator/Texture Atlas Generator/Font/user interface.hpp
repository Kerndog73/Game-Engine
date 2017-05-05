//
//  user interface.hpp
//  Texture Atlas Generator
//
//  Created by Indi Kernick on 3/5/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef font_user_interface_hpp
#define font_user_interface_hpp

#include <string>

class OptionsFileError final : public std::runtime_error {
public:
  OptionsFileError(int, int, const char *);
};

void createAtlasFromOptionsFile(const std::string &, const std::string &, const std::string &);

#endif
