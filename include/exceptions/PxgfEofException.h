#ifndef PXGFEOFEXCEPTION_H
#define PXGFEOFEXCEPTION_H
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

#include "PxgfException.h"

namespace pxgf
{
	/// End-of-file exception class for the PXGF library.
	class cPxgfEofException : public cPxgfException
	{
	public:
		/// Default Constructor
		cPxgfEofException()
		{
		}

		/// Constructor
		/// @param strMessage Error message string.
		cPxgfEofException(const std::string &strMessage)
		  : cPxgfException(strMessage)
		{
		}

		std::string toString() override
		{
			return std::string("PxgfEofException: ") + getMessage();
		}
	};
}

#endif
