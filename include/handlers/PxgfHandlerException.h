#ifndef PXGFHANDLEREXCEPTION_H
#define PXGFHANDLEREXCEPTION_H

#include "../exceptions/PxgfException.h"

namespace pxgf
{
	/// Chunk handlers will throw objects of this class
	/// when throwing exceptions
	class cPxgfHandlerException : public cPxgfException
	{
	public:
		/// Default constructor
		cPxgfHandlerException()
		{
		}

		/// Constructor with a defined message
		cPxgfHandlerException(const std::string &strMessage) : cPxgfException(strMessage)
		{
		}

		/// Returns a description of the exception
		virtual std::string toString() override
		{
			return std::string("cPxgfHandlerException: " + getMessage());
		}
	};
}

#endif
