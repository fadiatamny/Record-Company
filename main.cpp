#include "includes.hpp"

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		std::cout << "Please insert connection details" << std::endl;
		exit(-1);
	}

	strcpy(username, argv[1]);
	strcpy(password, argv[2]);

	if(initDB() == -1)
	{
		std::cout<< "There was an error with DB initlization" << std::endl;
		exit(-1);
	}

	std::cout << "DB initlized successfully" << std::endl;

	if(initData() == -1)
	{
		std::cout<< "There was an error with Data initlization" << std::endl;
		exit(-1);
	}
	
	std::cout << "Data inserted successfully" << std::endl;
	getchar();
	return 0;
}