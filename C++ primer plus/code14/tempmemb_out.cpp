#include <iostream>
// tempmemb_out.cpp -- template members
template <class T>
class beta
{
    private:
        template <class V>  // declaration
        class hold;
        hold<T> q;
        hold<int> n;
    public:
        beta(T t, int i) : q(t), n(i) { }
        template <typename U>   // declaration
        U blab(U u, T t);
        void Show() const { q.show(); n.show(); }
};

// member definition
template <class T>
    template<class V>
        class beta<T>::hold
        {
            private:
                V val;
            public:
                hold(V v = 0) : val(v) { }
                void show() const { std:: cout << val << std::endl; }
                V Value() const { return val; }
        };

// member definition
template <class T>
    template <typename U>
        U beta<T>::blab(U u, T t)
        {
            return (n.Value() + q.Value()) * u / t;
        }
int main()
{
    beta<double> guy(3.5, 3);
    std::cout << "T was set to double\n";
    guy.Show();
    std::cout << "V was set to T, which is double, then V was set to int\n";
    std::cout << guy.blab(10, 2.3) << std::endl;
    std::cout << "U was set to int\n";
    std::cout << guy.blab(10.0, 2.3) << std::endl;
    std::cout << "U was set to double\n";
    std::cout << "Done\n";
    return 0;
}