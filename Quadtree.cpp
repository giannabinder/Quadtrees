// include libraries and header files
#include "Quadtree.hpp"
#include "illegal_exception.hpp"

using namespace std;

// constructor, define constant member variables
Quadtree::Quadtree(const int m, const double x0, const double x1, const double y0, const double y1) : _m(m), _x0(x0), _x1(x1), _y0(y0), _y1(y1), _p_root(nullptr), _num_points(0) {

    // check if the bound is valid and print out success if it is
    if (_bound_validity(x0, x1, y0, y1)) { cout << "success"; }
    cout << endl;
}

// destructor, delete all dynamically allocated memory
Quadtree::~Quadtree() { 

    // if the tree is populated, call the function to recursively deallocate all the memory
    if (this->_p_root) {
        _recursive_dealloc(this-> _p_root); 
        this-> _p_root = nullptr;
    }
}

// recursively deallocate the tree from bottom up
void Quadtree::_recursive_dealloc(Quadnode* p_root) {

    // if the node is a leaf, call its destructor to delete all its pointer objects and array
    if ( p_root-> is_leaf() ) { p_root-> ~Quadnode(); }

    // if the node is not a leaf, continue recursively traversing through the tree
    else {
        _recursive_dealloc(p_root->get_p_NW());
        _recursive_dealloc(p_root->get_p_NE());
        _recursive_dealloc(p_root->get_p_SW());
        _recursive_dealloc(p_root->get_p_SE());
    }

    // delete the root of the subtree being tackled
    delete p_root;
}

// checks whether the given bound is valid, if it is not, an exception is thrown and returns false. Otherwise, true.
const bool Quadtree::_bound_validity(const double x0, const double x1, const double y0, const double y1) {

    try {
        if ((x1 <= x0) || (y1 <= y0)) { throw illegal_exception(); }
        else { return true; }
    } 
    
    catch (illegal_exception& exception) {
        cout << exception.invalid_bound();
        return false;
    }
}

// return the number of points in the quad tree
const int Quadtree::get_num() { return this-> _num_points; }

// inserts a point into the tree
void Quadtree::t_insert(const double x, const double y) {

    // if the point lies outside of or on the quadtree's boundaries, do not continue
    if ((x >= this-> _x1) || (x <= this-> _x0) || (y >= this-> _y1) || (y <= this-> _y0)) { cout << "failure\n"; }

    else {

        // check if the tree is empty, if it is, create the root and insert the point into the node directly
        if (!_p_root) { 
            this-> _p_root = new Quadnode(this-> _x0, this-> _x1, this-> _y0, this-> _y1, this-> _m);
            this-> _p_root-> n_insert(x, y);
            this-> _num_points++; 
            cout << "success\n"; 
        }
        
        else {

            // declare a temporary pointer to traverse through the tree
            Quadnode* p_current = this-> _p_root;

            
            while ( !(p_current-> is_leaf()) ) {

                // calculate the mid-axes of the current node we are visiting
                double mid_y = ((p_current-> y1) + (p_current-> y0))/2;
                double mid_x = ((p_current-> x1) + (p_current-> x0))/2;

                // determine which node to visit
                if ( x <= mid_x )  {

                    // visit the NW quadrant as the point lies in it
                    if ( y >= mid_y ) { p_current = p_current-> get_p_NW(); }

                    // visit the SW quadrant as the point lies in it
                    else { p_current = p_current-> get_p_SW(); }
                }

                else {

                    // visit the NE quadrant as the point lies in it
                    if ( y >= mid_y ) { p_current = p_current-> get_p_NE(); }

                    // visit the SE quadrant as the point lies in it
                    else { p_current = p_current-> get_p_SE(); }
                }
            }

            // insert the point into the quadrant we determined it belongs in and increase the counter if it gets inserted successfully
            if (p_current-> n_insert(x, y)) {
                this-> _num_points++; 
                cout << "success\n"; 
            }

            else { cout << "failure\n"; }
        }
    }
}

// recursively searches through the tree for a point strictly less than d distance to a given point
const bool Quadtree::_search_recursive(const double x, const double y, const double d, Quadnode* p_cur) {
    
    // if the node is a leaf, search it for a point within the distance
    if (p_cur-> is_leaf()) { return p_cur-> n_search(x, y, d); }

    // otherwise, recursively search through the quadrants only if their shortest distance is strictly less than d
    // stop searching once a point is found
    else{
        if (p_cur-> get_p_NW()-> d_point(x, y, 2, d) < d) { 
            if (_search_recursive(x, y, d, p_cur-> get_p_NW())) { return true; } 
        }
        if (p_cur-> get_p_NE()-> d_point(x, y, 1, d) < d) { 
            if (_search_recursive(x, y, d, p_cur-> get_p_NE())) { return true; } 
        }
        if (p_cur-> get_p_SW()-> d_point(x, y, 3, d) < d) { 
            if (_search_recursive(x, y, d, p_cur-> get_p_SW())) { return true; } 
        }
        if (p_cur-> get_p_SE()-> d_point(x, y, 4, d) < d) { 
            if (_search_recursive(x, y, d, p_cur-> get_p_SE())) { return true; } 
        }
    }

    // return false if no point is found
    return false;
}

const bool Quadtree::_range_recursive(const double xr0, const double yr0, const double xr1, const double yr1, Quadnode* p_cur) {

    // declare boolean to keep track of whether point within range are found within range
    bool in_range = false;

    // if this is a leaf node, call the function to print all the points which fall within the range
    if ( p_cur-> is_leaf()) { in_range = p_cur-> n_range(xr0, yr0, xr1, yr1); }

    // otherwise, continue pruning through the nodes and search the ones who's boundaries intersect the range recursively
    // perform a logical OR operation with the return value of each subproblem and in_range to ensure that once a point is found, the boolean is never set to false again
    else {
        if ( p_cur-> get_p_NW()-> spans(xr0, yr0, xr1, yr1) ) { in_range |= _range_recursive(xr0, yr0, xr1, yr1, p_cur-> get_p_NW()); }
        if ( p_cur-> get_p_NE()-> spans(xr0, yr0, xr1, yr1) ) { in_range |= _range_recursive(xr0, yr0, xr1, yr1, p_cur-> get_p_NE()); }
        if ( p_cur-> get_p_SW()-> spans(xr0, yr0, xr1, yr1) ) { in_range |= _range_recursive(xr0, yr0, xr1, yr1, p_cur-> get_p_SW()); }
        if ( p_cur-> get_p_SE()-> spans(xr0, yr0, xr1, yr1) ) { in_range |= _range_recursive(xr0, yr0, xr1, yr1, p_cur-> get_p_SE()); }
    } 

    return in_range;
}

const void Quadtree::_nearest_recursive(double& x_near, double& y_near, double& d_near, const double x, const double y, Quadnode* p_cur) {

    // go into node if it is a leaf node
    if ( p_cur-> is_leaf() ) { p_cur-> n_nearest(x_near, y_near, d_near, x, y); }

    // prune through the subquadrants to see which has boundarues closer to the point than the closest point already found, or if we havn't located a point yet, search through it
    else { 
        if ( p_cur-> get_p_NW()-> d_point(x, y, 2, d_near) < d_near || d_near == -1 ) { _nearest_recursive(x_near, y_near, d_near, x, y, p_cur-> get_p_NW()); }
        if ( p_cur-> get_p_NE()-> d_point(x, y, 1, d_near) < d_near || d_near == -1 ) { _nearest_recursive(x_near, y_near, d_near, x, y, p_cur-> get_p_NE()); }
        if ( p_cur-> get_p_SW()-> d_point(x, y, 3, d_near) < d_near || d_near == -1 ) { _nearest_recursive(x_near, y_near, d_near, x, y, p_cur-> get_p_SW()); }
        if ( p_cur-> get_p_SE()-> d_point(x, y, 4, d_near) < d_near || d_near == -1 ) { _nearest_recursive(x_near, y_near, d_near, x, y, p_cur-> get_p_SE()); }
    }
}

// determine which recursive function to use for each command given
const void Quadtree::dispatcher(const double x, const double y, const double xr1, const double yr1, const double d, const int f){
    
    switch (f) {

        // range
        case 1:
            // verify validity of boundary with function call here
            // check if the range is completely out of the tree's boundaries, verify that the tree contains points, and call the function to search through the tree recursively
            if ( (_bound_validity(x, xr1, y, yr1)) && (x >= this-> _x1 || xr1 <= this-> _x0 || y >= this-> _y1 || yr1 <= this-> _y0 || !(this-> _num_points) || !(_range_recursive(x, y, xr1, yr1, _p_root)))) { cout << "no points within range"; }
            cout << endl;
            return;

        // search
        case 2:
            // validate that the distance is a valid magnitude and that the tree contains points
            // call the function to recursively search for a point fitting the criteria
            if ( d < 0 || !(this-> _num_points) || !(_search_recursive(x, y, d, this-> _p_root))) { cout << "no point exists\n"; }
            else { cout << "point exists\n"; }
            return;

        // nearest
        case 3:
            // if the tree has no points, do not search through the tree
            if ( !(this-> _num_points) ) { cout << "no point exists\n"; }
            else { 

                // declare variables which will hold the closest point found and its distance
                double x_nearest = 0, y_nearest = 0, d_nearest = -1;

                // call the function which recursively searches through the tree for the nearest point
                _nearest_recursive(x_nearest, y_nearest, d_nearest, x, y, this-> _p_root); 

                cout << x_nearest << " " << y_nearest << endl;
            }
    }
}