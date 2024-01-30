#include <iostream>
#include <cstring>

using namespace std;
class Frabjous
{
private:
    char fab[20];
public:
    Frabjous(const char* s = "C++") { 
        strncpy(fab, s, 19); 
        fab[19] = '\0';
    }
    virtual void tell() { cout << fab; }
};

class Gloam
{
private:
    int glip;
    Frabjous fb;
public:
    Gloam(int g = 0, const char* s = "C++") : glip(g), fb(s) { }
    Gloam(int g, const Frabjous& f) : glip(g), fb(f) { }
    // note: the above uses the default Frabjous copy constructor
    void tell()
    {
        fb.tell();
        cout << glip << endl;
    }
};

int main()
{
    Frabjous fr("Hello World!");
    Gloam gl(21, fr);
    gl.tell();
    return 0;
}