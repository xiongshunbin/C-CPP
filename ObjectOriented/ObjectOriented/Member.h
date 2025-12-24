#pragma once

#include <string>
#include "Visitor.h"

/**
 * When a member object is not instantiated, its gender is unknown, 
 * just as a person's gender is unknown before they are born.
 */
enum class Sex : char
{
	Unknown,
	Male,
	Female
};

// 成员类的基类
class AbstractMember
{
public:
	explicit AbstractMember(std::string name) : m_name(name), m_sex(Sex::Unknown) { }
	std::string getName()
	{
		return m_name;
	}
	virtual void accept(AbstractAction* action) = 0;
	virtual ~AbstractMember() { }
	Sex getSex()
	{
		return m_sex;
	}

protected:
	std::string m_name;
	Sex m_sex;
};

// 男性子类
class MaleMember : public AbstractMember
{
public:
	explicit MaleMember(std::string name) : AbstractMember(name)
	{
		m_sex = Sex::Male;
	}

	void accept(AbstractAction* action) override
	{
		action->maleDoing(this);
	}
};

// 女性子类
class FemaleMember : public AbstractMember
{
public:
	explicit FemaleMember(std::string name) : AbstractMember(name)
	{
		m_sex = Sex::Female;
	}

	void accept(AbstractAction* action) override
	{
		action->femaleDoing(this);
	}
};
