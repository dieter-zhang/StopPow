
/** test class for SRIM
 * @author Alex Zylstra
 * @date 2014/11/18
 */


#include <stdio.h>
#include <dirent.h>

#include <iostream>
#include <vector>
#include <stdexcept>
#include <limits>

#include "StopPow.h"
#include "StopPow_SRIM.h"

int main(int argc, char * argv [])
{
	// check for verbosity flag:
	bool verbose = false;
	if( argc >= 2 )
	{
		for(int i=1; i < argc; i++)
		{
			std::string flag(argv[i]);
			if(flag == "--verbose")
			{
				verbose = true;
			}
		}
	}

	std::cout << "========== Test Suite 8 ==========" << std::endl;
	std::cout << "   Testing SRIM loading various files" << std::endl;

	// Load all SRIM models from SRIM directory
	std::vector<std::string> files;
	std::string dir_name("SRIM");
	DIR *SRIM_dir = opendir(dir_name.c_str());
	if(SRIM_dir) // dir is open
	{
		// loop over all files:
		dirent* result = readdir(SRIM_dir);
		while( (result=readdir(SRIM_dir)) != NULL )
		{
			// try to load SRIM:
			try
			{
				std::string name(result->d_name);
				std::string ext = name.substr(name.size()-3, 3);
				if(ext == "txt" || ext == "csv")
				{
					// construct relative file path/name:
					std::string fname(dir_name);
					fname.append("/");
					fname.append(result->d_name);
					files.push_back(fname);
				}
			}
			// catch and ignore all exceptions
			catch(...){}
		}
	}
	if(verbose)
		std::cout << files.size() << " SRIM model(s) to load" << std::endl;

	// try to load each file
	bool pass = true;
	for(auto f : files)
	{
		try
		{
			StopPow::StopPow_SRIM model(f);
			if(verbose)
				std::cout << "Successfully loaded " << f << std::endl;
		}
		catch(...)
		{
			pass = false;
			std::cout << "Error loading: " << f << std::endl;
		}
	}
	std::cout << "RESULT: " << (pass ? "PASS" : "FAIL") << std::endl;

	return (pass? 0 : 1);
}