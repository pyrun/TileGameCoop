#include "types.h"

#include <math.h>

/*double round(double d)
{
  return floor(d + 0.5);
}*/

bool file_exist( std::string file) {
    if( access( file.c_str(), F_OK ) != -1 )
        return true;
    return false;
}
