#include "VolumeShapes.h"
#include <fstream>
using namespace std;


int main()
{
    
    LinkedList myList;
    std::ifstream fin("volShapes.txt");
    int size;
    fin >> size;
    for (int i = 0; i < size; ++i)
    {
        try
        {
            myList.addtoEnd(VolShape::MakeInstance(fin));
        }
        catch (VolShape::BadClassname& bcn)
        {
            std::cout << " !!! ERROR: Bad class name '" << bcn.what() << "' encountered at step #" << i + 1 << '\n';
            myList.addtoEnd(new Cylinder);
            while (fin.get() != '\n') continue;
        }
    }
    myList.insert(new Cylinder(4, 5.0), 2);
    myList.printAll();
    return 0;
}