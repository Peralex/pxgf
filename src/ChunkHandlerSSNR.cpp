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

// LIBRARY INCLUDE FILES
#include <cstring>

#include "handlers/ChunkHandlerSSNR.h"
#include "handlers/PxgfHandlerException.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerSSNR::cChunkHandlerSSNR()
	  : m_pCallbackSSNRHandler(nullptr)
	{
	}

	void cChunkHandlerSSNR::registerCallbackHandler(cCallbackSSNR *pHandler)
	{
		m_pCallbackSSNRHandler = pHandler;
	}

	bool cChunkHandlerSSNR::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		// check that there is enough data for an even number of shorts
		if(((vchData.size()) & 0x3) != 0)
			return false;
		if(m_pCallbackSSNRHandler == nullptr) {
			return true;
		}

		int64_t lTimestamp_ns = *(int64_t *)vchData.data();
		int iLength = (vchData.size() - 8) / 2;
		//vector<short> vsIqData (iLength);
		m_vsIqData.resize(iLength);
		memcpy(&m_vsIqData[0], &vchData[8], iLength * sizeof(short));
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
			SWAP_INT64(lTimestamp_ns);
			for(vector<short>::iterator it = m_vsIqData.begin(); it != m_vsIqData.end(); it++)
				SWAP_SHORT(*it);
		}

		m_pCallbackSSNRHandler->callbackSSNR(lTimestamp_ns, m_vsIqData);
		return true;
	}

	void cChunkHandlerSSNR::writeChunkSSNR(cPXGWriterBase &stream, int64_t lTimestamp_ns, const vector<short> &vsRealData) const
	{
		stream.writeChunkHeader(*this, 8 + vsRealData.size() * 2);
		stream.writeInt64(lTimestamp_ns);

		// check that there are an even number of elements
		if((vsRealData.size() & 0x1) != 0)
			throw cPxgfHandlerException("The length of vsRealData must be even.");
		stream.writeShortArray(vsRealData);
	}
}
