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

#include "handlers/ChunkHandlerTEXT.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerTEXT::cChunkHandlerTEXT()
	  : m_pCallbackTEXTHandler(nullptr)
	{
	}

	cChunkHandlerTEXT::cChunkHandlerTEXT(const string &strChunkName)
	  : m_pCallbackTEXTHandler(nullptr)
	{
	}

	void cChunkHandlerTEXT::registerCallbackHandler(cCallbackTEXT *pHandler)
	{
		m_pCallbackTEXTHandler = pHandler;
	}

	bool cChunkHandlerTEXT::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		if(m_pCallbackTEXTHandler == nullptr) {
			return true;
		}

		int iTextBytes = *(int *)vchData.data();
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT(iTextBytes);

		string strMessage(vchData.data() + 4, vchData.data() + 4 + iTextBytes);
		m_pCallbackTEXTHandler->callbackTEXT(strMessage);
		return true;
	}

	void cChunkHandlerTEXT::writeChunkTEXT(cPXGWriterBase &stream, const string &strMessage) const
	{
		int iTextBytes = strMessage.length();
		int iPaddedBytes = 4 * (iTextBytes / 4);
		if(iTextBytes != iPaddedBytes) {
			iPaddedBytes += 4;
		}
		stream.writeChunkHeader(*this, iPaddedBytes + 4);
		stream.writeInteger(iTextBytes);

		string strPadded(strMessage);
		strPadded.append(iPaddedBytes - iTextBytes, '\0');
		stream.writeString(strPadded);
	}
}
