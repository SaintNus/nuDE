/*!
 * \file main.mm
 * \brief Main function.
 * \author Nus
 * \date 2011/09/11 17:53
 */
 
#include "Core/nuDebug.h"

int main(int argc, const char** argv)
{
  NU_ASSERT(false, "Test");
  NU_ASSERT_C(false);
  return NSApplicationMain(argc, argv);
}
