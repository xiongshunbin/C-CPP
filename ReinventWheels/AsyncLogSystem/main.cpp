#include "Logger.h"

using utility::Logger;
using utility::LogLevel;

int main()
{
	try
	{
		Logger::getInstance()->configure("log.txt");
		int user_id = 42;
		std::string action = "login";
		double duration = 3.5;
		std::string world = "World";
		DEBUG("Hello {}!", world);
		INFO("User {} performed {} in {} seconds.", user_id, action, duration);
		WARN("This is a message without any placeholders");
		ERROR("Multiple placeholders: {}, {}, ", 1, 2, 3);
		FATAL("I'd rather be {1} than {0}.", "right", "happy");
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}
