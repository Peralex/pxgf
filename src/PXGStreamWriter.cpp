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

#include <sstream>

#include "PXGStreamWriter.h"
#include "SwapEndian.h"
#include "exceptions/PxgfIllegalArgumentException.h"

namespace pxgf
{
	using namespace std;
	cPXGStreamWriter::cPXGStreamWriter(ostream &Stream, const bool bBigEndian) :
		cPXGWriterBase(bBigEndian),
		m_stream(Stream)
	{
	}

	ostream &cPXGStreamWriter::getOutputStream()
	{
		return m_stream;
	}

	void cPXGStreamWriter::write(const char *pchData, std::streamsize nLength)
	{
		m_stream.write(pchData, nLength);
	}
}
