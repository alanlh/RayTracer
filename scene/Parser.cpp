#include "Parser.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../objects/Drawable.h"
#include "../objects/Triangle.h"
#include "Vector3.h"

std::vector<Drawable *> *Parser::ParseObjects(std::string path) {

  std::ifstream file;
  file.open(path);

  if (!file) {
    std::cerr << "File didn't load with path " << path << std::endl;
    exit(1);
  }
  std::string line;

  std::vector<Vector3> vertexCoordinates;
  std::vector<Drawable *> *objects = new std::vector<Drawable *>;
  
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string init;
    if (line.length() == 0) {
      
    } else if (line.at(0) == 'v') {
      double a, b, c;
      if (!(iss >> init >> a >> b >> c)) {
	std::cerr << "A bad vertex has appeared?" << std::endl;
	std::cerr << line << std::endl;
	exit(1);
      }

      Vector3 vec(a, b, c);
      vertexCoordinates.push_back(vec);
      
    } else if (line.at(0) == 'f'){
      unsigned int a, b, c;
      if (!(iss >> init >> a >> b >> c)) {
	std::cerr << "A bad face has appeared?" << std::endl;
	std::cerr << line << std::endl;
	exit(1);
      }

      if (a > vertexCoordinates.size() ||
	  b > vertexCoordinates.size() ||
	  c > vertexCoordinates.size()) {
	std::cerr << "Requesting a non-existent vertex?" << std::endl;
	std::cerr << "Vertex numbers: " << a << " " <<  b << " "
		  << c << std::endl;
	std::cerr << "Current vertex count: " << vertexCoordinates.size();
	exit(1);	  
      }

      Triangle *triangle = new Triangle();
      triangle->a_ = vertexCoordinates[a - 1];
      triangle->b_ = vertexCoordinates[b - 1];
      triangle->c_ = vertexCoordinates[c - 1];

      objects->push_back(triangle);
    }
  }

  file.close();
  return objects;
}
