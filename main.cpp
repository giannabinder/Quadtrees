// include header files
#include "Quadtree.hpp"

using namespace std;

int main() {

  // declare pointer to point to the tree object which will be dynamically created with INIT
  Quadtree* tree = nullptr;

  string command;

  // Continuously take in user input and handle it with appropriate fucntion calls
  while (cin >> command){
    
    if (command == "INIT") { 
        
      int m;
      double x0, x1, y0, y1;

      cin >> m >> x0 >> y0 >> x1 >> y1;

      tree = new Quadtree(m, x0, x1, y0, y1);
    }

    else if (command == "INSERT") { 
        
      double x, y;
      cin >> x >> y;

      tree-> t_insert(x, y);
    }

    else if (command == "SEARCH") { 
        
      double x, y, d;
      cin >> x >> y >> d;

      // tree-> tree_search(x, y, d);
      tree-> dispatcher(x, y, 0, 0, d, 2);
    }

    else if (command == "NEAREST") { 
        
      double x, y;
      cin >> x >> y;

      // tree-> tree_nearest(x, y);
      tree-> dispatcher(x, y, 0, 0, 0, 3);
    }

    else if (command == "RANGE") { 
        
      double xr0, yr0, xr1, yr1;
      cin >> xr0 >> yr0 >> xr1 >> yr1;

      // tree-> tree_range(xr0, yr0, xr1, yr1);
      tree-> dispatcher(xr0, yr0, xr1, yr1, 0, 1);
    }

    else if (command == "NUM") { 

      cout << tree-> get_num() << endl;
    }

    // deallocate memory and exit the program  
    else if (command == "EXIT") {
      delete tree;
      tree = nullptr;
      return 0 ;
    }
  }
}