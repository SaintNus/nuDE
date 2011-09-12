/*!
 * \file main.mm
 * \brief Main function.
 * \author Nus
 * \date 2011/09/11 17:53
 */
 
class Nus
{
public:
  Nus() {
    NU_TRACE("Nus::Nus()\n");
  }
  ~Nus() {
    NU_TRACE("Nus::~Nus()\n");
  }
};

int main(int argc, const char** argv)
{
  Nus* p_nus = new Nus;
  delete p_nus;
  return NSApplicationMain(argc, argv);
}
