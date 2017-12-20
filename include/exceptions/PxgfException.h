#ifndef PXGFEXCEPTION_H
#define PXGFEXCEPTION_H
// Copyright 2006 Peralex Electronics (Pty) Ltd
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string>

namespace pxgf
{
	/// Base exception class for the PXGF library
	class cPxgfException
	{
	public:
		/// Default Constructor
		cPxgfException()
		{
		}

		/// Constructor with a defined message
		cPxgfException(const std::string &strMessage)
		  : m_strMessage(strMessage)
		{
		}

		/// Destructor
		virtual ~cPxgfException()
		{
		};

		/// Get a string representing the exception message
		/// @return Error message string.
		virtual std::string toString()
		{
			return std::string("cPxgfException: ") + getMessage();
		}

	protected:
		/// Get a string representing the exception message
		/// @return Error message string.
		std::string getMessage()
		{
			return m_strMessage;
		}

	private:
		std::string m_strMessage; ///< The exception message
	};
}

#endif
