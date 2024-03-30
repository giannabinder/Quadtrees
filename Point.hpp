// encircle in define conditionals to ensure it is only run once
#ifndef _POINT_HPP_
#define _POINT_HPP_

// include libraries
#include <iostream>

class Point {
  public:
    Point();
    ~Point();

    void set_coords(const double x, const double y);
    const double distance(const double x, const double y);

    // getter functions   
    const double get_x();
    const double get_y();

  private:

    // declare member variables
    double _x;
    double _y;
};

#endif // end definition