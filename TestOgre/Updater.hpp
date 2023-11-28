#include <Ogre.h>

using namespace std;
using namespace Ogre;

class Updater : public FrameListener
{
public:
    Updater() {

    }

    bool frameStarted(const FrameEvent&) override {

        return true;
    }

};