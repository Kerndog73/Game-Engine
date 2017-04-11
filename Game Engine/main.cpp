//
//  main.cpp
//  Game Engine
//
//  Created by Indi Kernick on 11/11/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "Engine/Application/impl.hpp"

int main(int, char const**) {
  app = std::make_unique<Game::AppImpl>();
  app->mainloop();
  PROFILER_INFO(std::cout);
  
  return EXIT_SUCCESS;
}
