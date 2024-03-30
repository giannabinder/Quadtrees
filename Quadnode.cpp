// include libraries and header files
#include "Quadnode.hpp"
#include <cmath>

using namespace std;

// constructor, define constant member variables
Quadnode::Quadnode(const double x0, const double x1, const double y0, const double y1, const int m) : x0(x0), x1(x1), y0(y0), y1(y1), _p_array(nullptr), _p_NW(nullptr), _p_NE(nullptr), _p_SW(nullptr), _p_SE(nullptr), _stored_points(0), _m(m) {}

// destructor, delete all dynamically allocated memory
Quadnode::~Quadnode() {
    
    for ( int counter = 0; counter < this-> _m ; counter++ ) {
        // delete the point object
        _p_array[counter].~Point();
    }

    // delete the array and set the pointer to null
    if (this-> _p_array) {
        delete[] this-> _p_array;
        _p_array = nullptr;
    }
}

// returns true if the node is a leaf or false if it's internal node
const bool Quadnode::is_leaf() {
    if ( this-> _stored_points >= 0 ) { return true; }
    else { return false; }
}

// getter functions which returns pointers to the node's children
Quadnode* Quadnode::get_p_NW() { return this-> _p_NW; }
Quadnode* Quadnode::get_p_NE() { return this-> _p_NE; }
Quadnode* Quadnode::get_p_SW() { return this-> _p_SW; }
Quadnode* Quadnode::get_p_SE() { return this-> _p_SE; }

// determine which sub-quadrant the points belongs in and inserts them 
void Quadnode::_sort_point(const double mid_x, const double mid_y, const double x, const double y) {
    
    // determine which quadrants the points stored in the original node belong to 
    if ( x <= mid_x )  {

        // insert in NW quadrant
        if ( y >= mid_y ) { _p_NW-> n_insert(x, y); }

        // insert in SW quadrant
        else { _p_SW-> n_insert(x, y); }
    }

    else {

        // insert in NE quadrant
        if ( y >= mid_y ) { _p_NE-> n_insert(x, y); }

        // insert in SE quadrant
        else { _p_SE-> n_insert(x, y); }
    }
}

// creates sub-quadrants and assigns them points
void Quadnode::_split(const double x, const double y) {

    // determine the boundaries of the sub-quadrants
    double mid_y = ((this-> y1) + (this-> y0))/2;
    double mid_x = ((this-> x1) + (this-> x0))/2;

    // create new sub nodes
    this-> _p_NW = new Quadnode(this-> x0, mid_x, mid_y, this-> y1, this-> _m);
    this-> _p_NE = new Quadnode(mid_x, this-> x1, mid_y, this-> y1, this-> _m);
    this-> _p_SW = new Quadnode(this-> x0, mid_x, this-> y0, mid_y, this-> _m);
    this-> _p_SE = new Quadnode(mid_x, this-> x1, this-> y0, mid_y, this-> _m);

    // sort the original node's points
    for (int counter = 0; counter < this-> _m; counter++ ){
        
        // get the point's coordinates
        double x_prev = _p_array[counter].get_x();
        double y_prev = _p_array[counter].get_y();

        // call the function to determine which sub-quadrant the point belongs in
        _sort_point(mid_x, mid_y, x_prev, y_prev);

        // delete the point
        this-> _p_array[counter]. ~Point();
    }

    // delete the array and set the pointer to null
    delete[] this-> _p_array;
    this-> _p_array = nullptr;

    // sort the new point
    _sort_point(mid_x, mid_y, x, y);
}

// insert a point into a leaf node
bool Quadnode::n_insert(const double x, const double y) {

    // if the leaf node does not hold any points
    if (!(this-> _p_array)) { 
        this-> _p_array = new Point[this-> _m]; // dynamically create the array
        this-> _p_array[_stored_points].set_coords(x, y); // store the point
        this-> _stored_points++; // increase stored points counter
    }

    else {

        // go through the node and look to see if the point already exists
        for ( int counter = 0 ; counter < this-> _stored_points ; counter++)  {
            if ( (this-> _p_array[counter].get_x() == x) && (this-> _p_array[counter].get_y() == y)) { return false; }
        }

        // if the node is full, split it and add the point
        if ( this-> _stored_points == this-> _m ) { 
            this-> _stored_points = -1;
            this-> _split(x, y); 
        }

        // otherwise, add the point and increase the counter to indicate how many points the node stores
        else { 
            this-> _p_array[_stored_points].set_coords(x, y); 
            this-> _stored_points++;
        }
    }

    return true; 
}

const bool Quadnode::n_search(const double x, const double y, const double d) {

    // check if there is a point within the distance
    for ( int counter = 0 ; counter < this-> _stored_points ; counter++)  {  
        if (d > (_p_array[counter].distance(x, y))) { return true; }
    }

    return false;
}

// determine if the node intersects with the range
const bool Quadnode::spans(const double xr0, const double yr0, const double xr1, const double yr1) {
    return ((this-> x0 <= xr0 && this-> x1 >= xr0) || (this-> x0 <= xr1 && this-> x1 >= xr1) || (this-> y0 <= yr1 && this-> y1 >= yr1) || (this-> y0 <= yr0 && this-> y1 >= yr0));
}

// prints all the points within the node which are strictly within the range specified
// returns true if points fall within the range
const bool Quadnode::n_range(const double xr0, const double yr0, const double xr1, const double yr1) {

    // declare boolean to keep track of a node within range
    bool in_range = false;

    // if this is a leaf node, print all the points which fall within the range
    for ( int counter = 0 ; counter < this-> _stored_points ; counter++)  {
    
        // get the point's coordinates
        double x = this-> _p_array[counter].get_x();
        double y = this-> _p_array[counter].get_y();
        
        // if the point is in the range, print its coordinates and update the boolean
        if ( (x < xr1) && (x > xr0)  && (y < yr1) && (y > yr0)) { 
            cout << x << " " << y << " ";
            in_range = true;
        }
    }

    return in_range;
}

// calculate the distance between two points
const double Quadnode::_pythagorean(const double x1, const double x0, const double y1, const double y0) { return sqrt(pow((x1 - x0), 2) + pow((y1 - y0), 2)); }

// determine the shortest distance from a point to a quadrant
const double Quadnode::d_point(const double x, const double y, const int quad, const double d_near) {

    // if the point lies inside the quadrant, return 0
    if ( (this-> x0 <= x) && (this-> x1 >= x) && (this-> y0 < y) && (this-> y1 > y) ) { 
    
        // check the quadrant's boundary conditions, if the point lies on the edge it doesn't contain, return a number larger than the distance we are checking against
        if (( (quad == 1) && (x == this-> x0) ) || ( (quad == 3) && (y == this-> y1) ) || ( (quad == 4) && ( (y == this-> y1) || (x == this-> x0)) )) { return d_near + 1; }

        return 0;
    }

    // determine the shortest distance from the point to the quadrant's boundary

    else if ( (this-> x0 <= x) && (this-> x1 >= x) ) {
        if (this-> y1 < y) { return ( y - (this-> y1) ); } // above the quadrant
        else{ return ( (this-> y0) - y ); } // below the quadrant
    }

    else if ( (this-> y0 <= y) && (this-> y1 >= y) ) {
        if (this-> x0 >= x)  { return ( (this-> x0) - x ); } // to the left of quadrant
        else { return ( x - (this-> x1)); } // to the right of quadrant
    }

    else if (this-> y1 <= y) {
        if (this-> x0 >= x) { return _pythagorean(this-> x0, x, y, this-> y1); } // above and to the left
        else { return _pythagorean(x, this-> x1, y, this-> y1); } // above and to the right
    }

    else {
        if (this-> x0 >= x) { return _pythagorean(this-> x0, x, this-> y0, y); } // below and to the left
        else { return _pythagorean(x, this-> x1, this-> y0, y); } // below and to the right
    }
}

// check the node for a point closer to the given point than the closest one already found
const void Quadnode::n_nearest(double& x_near, double& y_near, double& d, const double x, const double y) {

    // if this is a leaf node, we check all points against our current closest point found
    for ( int counter = 0 ; counter < this-> _stored_points ; counter++)  {

        // calculate the distance from the point
        double distance_compare = _p_array[counter].distance(x, y);

        // get the point's coordinates
        double x_compare = this-> _p_array[counter].get_x();
        double y_compare = this-> _p_array[counter].get_y();

        // if the distance is less than our current distance or we haven't found a point yet, set this point as our current nearest point
        if (( d < 0 ) || (distance_compare < d)) {
            x_near = x_compare;
            y_near = y_compare;
            d = distance_compare;
        }

        // if there is a tie between distances, choose the one with the larger x value (or y value if the xs are equal)
        if ( ( d == distance_compare ) && ( ( x_compare > x_near ) || ( x_compare == x_near && y_compare > y_near ) )) { y_near = y_compare; } 
    }
}