#include "emp.h"

using std::cout;
using std::cin;
using std::endl;

abstr_emp::abstr_emp() : fname("no"), lname("one"), job("none")
{
}

abstr_emp::abstr_emp(const std::string &fn, const std::string &ln, const std::string &j) : fname(fn), lname(ln), job(j)
{
}

void abstr_emp::ShowAll() const
{
    cout << "firstname: " << fname << endl;
    cout << "lastname: " << lname << endl;
    cout << "job: " << job << endl;
}

void abstr_emp::SetAll()
{
    while(cin.get() != '\n');
    cout << "Enter the firstname: ";
    getline(cin, fname);
    cout << "Enter the lastname: ";
    getline(cin, lname);
    cout << "Enter the job: ";
    getline(cin, job);
}

abstr_emp::~abstr_emp()
{
}

std::ostream &operator<<(std::ostream &os, const abstr_emp &e)
{
    // TODO: insert return statement here
    os << e.fname << " " << e.lname << "'s job: " << e.job;
    return os;
}

employee::employee() : abstr_emp()
{
}

employee::employee(const std::string &fn, const std::string &ln, const std::string &j) : abstr_emp(fn, ln, j)
{
}

void employee::ShowAll() const
{
    abstr_emp::ShowAll();
    cout << endl;
}

void employee::SetAll()
{
    abstr_emp::SetAll();
}

manager::manager() : abstr_emp(), inchargeof(0)
{
}

manager::manager(const std::string &fn, const std::string &ln, const std::string &j, int ico) : abstr_emp(fn, ln, j), inchargeof(ico)
{
}

manager::manager(const abstr_emp &e, int ico) : abstr_emp(e), inchargeof(ico)
{
}

manager::manager(const manager &m) : abstr_emp(m), inchargeof(m.inchargeof)
{
}

void manager::ShowAll() const
{
    abstr_emp::ShowAll();
    cout << "inchargeof: " << inchargeof << endl;
    cout << endl;
}

void manager::SetAll()
{
    abstr_emp::SetAll();
    cout << "Enter the inchargeof: ";
    cin >> inchargeof;
}

fink::fink() : abstr_emp(), reportsto("none")
{
}

fink::fink(const std::string &fn, const std::string &ln, const std::string &j, const std::string &rpo) : abstr_emp(fn, ln, j), reportsto(rpo)
{
}

fink::fink(const abstr_emp &e, const std::string &rpo) : abstr_emp(e), reportsto(rpo)
{
}

fink::fink(const fink &e) : abstr_emp(e), reportsto(e.reportsto)
{
}

void fink::ShowAll() const
{
    abstr_emp::ShowAll();
    cout << "reportsto: " << reportsto << endl;
    cout << endl;
}

void fink::SetAll()
{
    abstr_emp::SetAll();
    cout << "Enter the reportsto: ";
    cin >> reportsto;
}

highfink::highfink() : abstr_emp(), fink(), manager()
{
}

highfink::highfink(const std::string &fn, const std::string &ln, const std::string &j, const std::string &rpo, int ico)
    : abstr_emp(fn, ln ,j), fink(fn, ln, j, rpo), manager(fn, ln, j, ico)
{
}

highfink::highfink(const abstr_emp &e, const std::string &rpo, int ico) : abstr_emp(e), fink(e, rpo), manager(e, ico)
{
}

highfink::highfink(const fink &f, int ico) : abstr_emp(f), fink(f), manager(f, ico)
{
}

highfink::highfink(const manager &m, const std::string rpo) : abstr_emp(m), fink(m, rpo), manager(m)
{
}

highfink::highfink(const highfink &h) : abstr_emp(h), fink(h), manager(h)
{
}

void highfink::ShowAll() const
{
    abstr_emp::ShowAll();
    cout << "reportsto: " << fink::ReportsTo() << endl;
    cout << "inchargeof: " << manager::InChargeOf() << endl;
    cout << endl;
}

void highfink::SetAll()
{
    abstr_emp::SetAll();
    cout << "Enter the reportsto: ";
    cin >> fink::ReportsTo();
    cout << "Enter the inchargeof: ";
    cin >> manager::InChargeOf();
}
