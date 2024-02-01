#ifndef __EMP_H__
#define __EMP_H__

// emp.h -- header file for abstr_emp class and children
#include <iostream>
#include <string>

class abstr_emp
{
private:
    std::string fname;      // abstr_emp's first name
    std::string lname;      // abstr_emp's last name
    std::string job;
public:
    abstr_emp();
    abstr_emp(const std::string& fn, const std::string& ln, const std::string& j);
    virtual void ShowAll() const;   // labels and shows all data
    virtual void SetAll();          // prompts user for values
    friend std::ostream& operator<<(std::ostream &os, const abstr_emp& e);
    // just displays first and last name
    virtual ~abstr_emp() = 0;       // virtual base class
};

class employee : public abstr_emp
{
public:
    employee();
    employee(const std::string& fn, const std::string& ln, const std::string& j);
    virtual void ShowAll() const;
    virtual void SetAll();
}

class manager : virtual public abstr_emp
{
    
}

#endif
