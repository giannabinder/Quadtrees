// encircle in define conditionals to ensure it is only run once
#ifndef _QUADNODE_HPP_
#define _QUADNODE_HPP_

// include libraries
#include <iostream>
#include "Point.hpp"

class Quadnode {
    public:
        Quadnode(const double x0, const double x1, const double y0, const double y1, const int m);
        ~Quadnode();
        
        bool n_insert(const double x, const double y);
        const bool n_search(const double x, const double y, const double d);
        const bool n_range(const double xr0, const double yr0, const double xr1, const double yr1);
        const void n_nearest(double& x_near, double& y_near, double& d, const double x, const double y);

        const bool is_leaf();
        const double d_point(const double x, const double y, const int quad, const double d_near);
        const bool spans(const double xr0, const double yr0, const double xr1, const double yr1);

        Quadnode* get_p_NW();
        Quadnode* get_p_NE();
        Quadnode* get_p_SW();
        Quadnode* get_p_SE();


        const double x0, x1, y0, y1;

    private:
        void _sort_point(const double mid_x, const double mid_y, const double x, const double y);
        void _split(const double x, const double y);
        const double _pythagorean(const double x1, const double x0, const double y1, const double y0);

        // declare private member variables
        // pointers to its children and array which holds points
        Point* _p_array;
        Quadnode* _p_NW;
        Quadnode* _p_NE;
        Quadnode* _p_SE; 
        Quadnode* _p_SW;

        const int _m; // stores the maximum number of points the node can have

        int _stored_points; // stores the number of points within the node
};

#endif // end definition