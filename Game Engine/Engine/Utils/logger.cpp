//
//  logger.cpp
//  Game Engine
//
//  Created by Indi Kernick on 14/12/16.
//  Copyright © 2016 Indi Kernick. All rights reserved.
//

#include "logger.hpp"

std::FILE *Utils::Log::file = nullptr;
bool Utils::Log::initialized = false;
ptrdiff_t Utils::Log::filePathOffset = 0;
Utils::Log::Entry Utils::Log::preInitEntries[MAX_PRE_INIT_ENTRIES];
size_t Utils::Log::numPreInitEntries = 0;

const char *Utils::Log::DOMAIN_STRINGS[] {
  "Input",
  "UI",
  "Game Logic",
  "Game View",
  "Rendering",
  "Physics",
  "AI",
  "Audio",
  "Animation",
  "Game Events",
  "Application",
  "Scene Graph",
  "Resources",
  "Platform",
  "Memory"
};

const char *Utils::Log::SEVERITY_STRINGS[] {
  "Debug",
  "Info",
  "Warning",
  "Error"
};

bool Utils::Log::init(const char *filePath) {
  if (!initialized) {
    file = std::fopen(filePath, "w");
    if (file == nullptr) {
      std::cerr << "Failed to open file \"" << filePath << "\"\n";
      return false;
    }
    
    const char *outerProjFolder = std::strstr(__FILE__, "Game Engine/Game Engine/");
    assert(outerProjFolder);
    filePathOffset = outerProjFolder - __FILE__ + std::strlen("Game Engine/Game Engine/");
    
    initialized = true;
    
    flushPreInitEntries();
  } else {
    std::cerr << "Log was initialized more than once\n";
  }
  return true;
}

void Utils::Log::quit() {
  if (initialized) {
    std::fclose(file);
    file = nullptr;
    
    initialized = false;
  } else {
    std::cerr << "Log was destroyed before it was initialized (or it was destroyed twice)\n";
  }
}

void Utils::Log::write(Domain domain, Severity severity, const char *fileName,
                const char *function, int line, const char *format, ...) {
  va_list list;
  va_start(list, format);
  static char message[MAX_MESSAGE_LENGTH];
  int status = std::vsnprintf(message, MAX_MESSAGE_LENGTH, format, list);
  va_end(list);
  assert(status >= 0);
  assert(status < static_cast<int>(MAX_MESSAGE_LENGTH));
  
  if (initialized) {
    writeToFile(domain, severity, fileName, function, line, message);
  } else {
    preInitWrite({domain, severity, fileName, function, line, message});
  }
}

void Utils::Log::writeToFile(Domain domain, Severity severity, const char *fileName,
                      const char *function, int line, const char *message) {
  if (severity == ERROR) {
    std::fprintf(file, "%s %s\n  %s:%i\n  %s\n  %s\n", DOMAIN_STRINGS[domain],
                                                       SEVERITY_STRINGS[severity],
                                                       fileName + filePathOffset,
                                                       line,
                                                       function,
                                                       message);
  } else {
    std::fprintf(file, "%s %s\n  %s\n", DOMAIN_STRINGS[domain],
                                        SEVERITY_STRINGS[severity],
                                        message);
  }
  
  #ifdef DEBUG
  //making sure every entry is in the file before the program crashes
  std::fflush(file);
  #endif
}

void Utils::Log::preInitWrite(const Entry &entry) {
  if (numPreInitEntries >= MAX_PRE_INIT_ENTRIES) {
    std::cerr << "Too many log entries were written before the logger was initialized\n";
    return;
  }
  
  preInitEntries[numPreInitEntries++] = entry;
}

void Utils::Log::flushPreInitEntries() {
  for (size_t i = 0; i < numPreInitEntries; i++) {
    const Entry &entry = preInitEntries[i];
    writeToFile(entry.domain, entry.severity, entry.file,
                entry.function, entry.line, entry.message);
  }
  numPreInitEntries = 0;
}
