/*
 *  errors.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 9/23/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "errors.h"
#include <sstream>
#include <stdexcept>


std::string trim(const std::string& file) 
{
	// find last path separator
	std::string::size_type n = file.find_last_of("/\\");
	if (n == std::string::npos)
	{
	   // return the whole thing--it's a naked file name anyway
		return file;
	}
	else
	{
		// keep the file name only
		return file.substr(n+1);
	}
}
    
	
std::string format(const std::string& file, long line,
                       const std::string& function,
                       const std::string& message) 
{
	std::ostringstream msg;
	msg << "\n" << file << ":" << line << ": ";
	if (function != "(unknown)")
	{
		msg << "In function `" << function << "': \n";
	}

	msg << message;
	return msg.str();
}
	


namespace boost {
	
    // must be defined by the user
    void assertion_failed(char const * expr, char const * function,
                          char const * file, long line) {
        throw std::runtime_error(format(file, line, function,
                                        "Boost assertion failed: " +
                                        std::string(expr)));
    }
	
}

namespace IBTrader
{
		
Error::Error(const std::string& file, long line,
                 const std::string& function,
                 const std::string& message) 
{
	message_ = boost::shared_ptr<std::string>(new std::string(format(file, line, function, message)));
}
	
const char* Error::what() const throw () 
{
	return message_->c_str();
}
	
}
	


