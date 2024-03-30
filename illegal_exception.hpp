// encircle in define conditionals to ensure it is only run once
#ifndef _ILLEGAL_EXCEPTION_HPP_
#define _ILLEGAL_EXCEPTION_HPP_

// include libraries
#include <iostream>
#include <exception>

class illegal_exception : public std::exception { 
  public:
    const char* invalid_bound();
};

#endif // end definition