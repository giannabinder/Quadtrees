// include libraries and header files
#include "Point.hpp"
#include <cmath>

using namespace std;

// constructor, define constant member variables
Point::Point() : _x(0), _y(0) {}

Point::~Point() {}

// set the coordinates of the point
void Point::set_coords(const double x, const double y) {
    this-> _x = x;
    this-> _y = y;
}

// return the distance between the point and another
const double Point::distance(const double x, const double y) { return (sqrt((this->_x -x)*(this->_x -x)+ (this->_y - y)*(this->_y - y))); }

// getter functions which return the coordinates of the point
const double Point::get_x() {return this-> _x; }
const double Point::get_y() {return this-> _y; }