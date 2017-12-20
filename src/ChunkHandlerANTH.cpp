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

#include "handlers/ChunkHandlerANTH.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;

	cChunkHandlerANTH::cChunkHandlerANTH()
	  : m_pCallbackANTHHandler(nullptr)
	{
	}

	void cChunkHandlerANTH::registerCallbackHandler(cCallbackANTH *pHandler)
	{
		m_pCallbackANTHHandler = pHandler;
	}

	bool cChunkHandlerANTH::processChunk(const std::vector<char> &vchData, bool bBigEndian)
	{
		if(m_pCallbackANTHHandler == nullptr)
			return true;

		int iNameLength = *(int *)vchData.data();

		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT(iNameLength);

		string strAntennaName(vchData.data() + 4, vchData.data() + 4 + iNameLength);
		m_pCallbackANTHHandler->callbackANTH(strAntennaName);
		return true;
	}

	void cChunkHandlerANTH::WriteChunkANTH(cPXGWriterBase &stream, const string &strAntennaName) const
	{
		int iNameLength = strAntennaName.length();
		int iPaddedLength = 4 * (iNameLength / 4);
		if(iPaddedLength != iNameLength)
			iPaddedLength += 4;

		stream.writeChunkHeader(*this, iPaddedLength + 4);
		stream.writeInteger(iNameLength);

		string strPadded(strAntennaName);
		strPadded.append((iPaddedLength - iNameLength), '\0');
		stream.writeString(strPadded);
	}
}
