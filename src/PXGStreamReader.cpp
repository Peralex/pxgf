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

#include "PXGStreamReader.h"
#include "SwapEndian.h"
#include "exceptions/PxgfException.h"

namespace pxgf
{
	using namespace std;
	cPXGStreamReader::cPXGStreamReader()
	  : m_pStream(nullptr)
	{
	}

	cPXGStreamReader::cPXGStreamReader(istream &stream)
	  : m_pStream(&stream)
	{
	}

	void cPXGStreamReader::processStream(istream &stream, bool bBigEndian)
	{
		m_pStream = &stream;
		processPXGStream(bBigEndian);
	}

	void cPXGStreamReader::processStream(std::istream &stream, eEndianType EndianType)
	{
		m_pStream = &stream;
		processPXGStream(EndianType);
	}

	void cPXGStreamReader::blockingRead(char *pchData, int iLength)
	{
		int iLengthRead = 0;
		do {
			if(m_pStream->eof())
				throw cPxgfException("End of file");
			m_pStream->read(pchData + iLengthRead, iLength - iLengthRead);
			iLengthRead += m_pStream->gcount();
		} while(iLengthRead < iLength);
	}
}
