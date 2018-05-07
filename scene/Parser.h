#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <fstream>

#include "../objects/Drawable.h"

class Parser {
 public: 
  /**
   * Parses through an obj file and returns a vector of Drawable pointers
   */
  static std::vector<Drawable *> *ParseObjects(std::string path);
};

#endif
