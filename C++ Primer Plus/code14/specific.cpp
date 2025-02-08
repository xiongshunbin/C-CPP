#include <iostream>
#include <string>

template <class T1, class T2>
class Temp
{
    private:
        T1 a;
        T2 b;
    public:
        Temp() { };
        Temp(const T1 & aval, const T2 & bval) : a(aval), b(bval) { };
        void show() const;
};

template <class T1, class T2>
void Temp<T1, T2>::show() const
{
    std:: cout << "a = " << a <<" b = " << b << std::endl;
    std::cout << "use general definition." << std::endl;
}

// explicit generate Temp<string, double> class
template class Temp<std::string, double>;

template<> 
class Temp<double, int>
{
    private:
        double a;
        int b;
    public:
        Temp() { };
        Temp(double aval, int bval) : a(aval), b(bval) { }
        void show() const;
};

void Temp<double, int>::show() const
{
    std::cout << "a = " << a << " b = " << b << std::endl;
    std::cout << "use specialized definition" << std:: endl;
}

template <class T1>
class Temp<T1, int>
{
    private:
        T1 a;
        int b;
    public:
        Temp() { };
        Temp(const T1 & aval, int bval) : a(aval), b(bval) { };
        void show() const;
};

template <class T1>
void Temp<T1, int>::show() const
{
    std::cout << "a = " << a << " b = " << b << std::endl;
    std::cout << "use partial specialized definition" << std::endl;
}

int main()
{
    Temp<char, char> tpii('a', 'A');
    tpii.show();

    Temp<double, int> tpdi(2.1, 5);
    tpdi.show();

    Temp<char, int> tpxi('a', 65);
    tpxi.show();

    return 0;
}