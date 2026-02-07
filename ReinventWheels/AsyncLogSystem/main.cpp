#include "Logger.h"

int main()
{
	try
	{
		Logger logger("log.txt");
		logger.log(LogLevel::INFO, "Starting application...");
		int user_id = 42;
		std::string action = "login";
		double duration = 3.5;
		std::string world = "World";
		logger.log(LogLevel::INFO, "User {} performed {} in {} seconds.", user_id, action, duration);
		logger.log(LogLevel::DEBUG, "Hello {}", world);
		logger.log(LogLevel::WARN, "This is a message without any placeholders");
		logger.log(LogLevel::ERROR, "Multiple placeholders: {}, {}, ", 1, 2, 3);
		logger.log(LogLevel::INFO, "I'd rather be {1} than {0}.", "right", "happy");
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}
