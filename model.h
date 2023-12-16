#include <iostream>

using namespace std;

class Model {

    public:
        virtual void notifyView() = 0;
        virtual int getControllerInput() = 0;
};
