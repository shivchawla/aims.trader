/*
 *  errors.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 9/23/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#ifndef ERRORS_H
#define ERRORS_H

//#include <boost/assert.hpp>
///#include <boost/current_function.hpp>
#include <boost/shared_ptr.hpp>
#include <exception>
#include <string>

namespace IBTrader 
{	
//! Base error class
class Error : public std::exception 
{
	public:
        /*! The explicit use of this constructor is not advised.
		 Use the QL_FAIL macro instead.
		 */
        Error(const std::string& file,
              long line,
              const std::string& functionName,
              const std::string& message = "");
        /*! the automatically generated destructor would
		 not have the throw specifier.
		 */
        ~Error() throw() {}
        //! returns the error message.
        const char* what() const throw ();
	
    private:
        boost::shared_ptr<std::string> message_;
};
	
}

inline void FAIL(const std::string& message)
{
	do{ 
	    throw IBTrader::Error(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, message);
	}while (false);
}


//throw an error if the given condition is not verified
inline void ASSERT(bool condition, const std::string& message)
{
	if(!condition)
	{
		throw IBTrader::Error(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, message);
	}
}

//throw an error if the given pre-condition is not verified
inline void REQUIRE(bool condition, const std::string& message)
{
	if(!condition)
	{
		throw IBTrader::Error(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, message);
	}
	
}

//throw an error if the given post-condition is not verified
inline void ENSURE(bool condition, const std::string& message)
{
	if(!condition)
	{
		throw IBTrader::Error(__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, message);
	}
}



#endif

