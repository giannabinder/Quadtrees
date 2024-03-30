// encircle in define conditionals to ensure it is only run once
#ifndef _QUADTREE_HPP_
#define _QUADTREE_HPP_

// include libraries
#include <iostream>
#include "Quadnode.hpp"

class Quadtree {
  public:
    Quadtree(const int m, const double x0, const double x1, const double y0, const double y1); // declare custom constructor
    ~Quadtree();

    void t_insert(const double x, const double y);
    const void dispatcher(const double xr0, const double yr0, const double xr1, const double yr1, const double d, const int f);
    const int get_num();

  private:
    void _recursive_dealloc(Quadnode* p_root);

    const bool _search_recursive(const double x, const double y, const double d, Quadnode* p_cur);
    const bool _range_recursive(const double xr0, const double yr0, const double xr1, const double yr1, Quadnode* p_cur);
    const void _nearest_recursive(double& x_near, double& y_near, double& d_near, const double x, const double y, Quadnode* p_cur);
    
    const bool _bound_validity(const double x0, const double x1, const double y0, const double y1);

    // declare member variables
    // boundary
    const int _x0;
    const int _x1;
    const int  _y0;
    const int _y1;

    const int _m; // stores the maximum number of points each leaf can hold

    int _num_points; // stores the number of points within the tree

    Quadnode* _p_root; // pointer to root of tree

};

#endif // end definition