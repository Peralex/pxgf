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

#include "handlers/ChunkHandlerSSIQ.h"
#include "handlers/PxgfHandlerException.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerSSIQ::cChunkHandlerSSIQ()
	  : m_pCallbackSSIQHandler(nullptr)
	{
	}

	void cChunkHandlerSSIQ::registerCallbackHandler(cCallbackSSIQ *pHandler)
	{
		m_pCallbackSSIQHandler = pHandler;
	}

	bool cChunkHandlerSSIQ::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		// check that there is enough data for an even number of shorts
		if(((vchData.size()) & 0x3) != 0)
			return false;
		if(m_pCallbackSSIQHandler == nullptr) {
			return true;
		}

		int64_t lTimestamp_us = *(int64_t *)vchData.data();
		int iLength = (vchData.size() - 8) / 2;
		m_vsIqData.resize(iLength);
		memcpy(m_vsIqData.data(), &vchData[8], iLength * sizeof(short));
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
			SWAP_INT64(lTimestamp_us);
			for(auto &&sData : m_vsIqData)
				SWAP_SHORT(sData);
		}

		m_pCallbackSSIQHandler->callbackSSIQ(lTimestamp_us, m_vsIqData);
		return true;
	}

	void cChunkHandlerSSIQ::writeChunkSSIQ(cPXGWriterBase &stream, int64_t lTimestamp_us, const vector<short> &vsIqData) const
	{
		stream.writeChunkHeader(*this, 8 + vsIqData.size() * 2);
		stream.writeInt64(lTimestamp_us);
		stream.writeShortArray(vsIqData);
	}

	void cChunkHandlerSSIQ::repack(const cPackingSIQP &oInputPacking, const cPackingSIQP &oOutputPacking, const vector<short> &vsInputIQData, vector<short> &vsOutputIQData)
	{
		if(oInputPacking.equals(oOutputPacking)) {
			vsOutputIQData = vsInputIQData;
			return;
		}
		// check that there are an even number of elements
		if((vsInputIQData.size() & 0x1) != 0)
			throw cPxgfHandlerException("The length of vsInputIQData must be even.");

		// swap IQ data
		vsOutputIQData.resize(vsInputIQData.size());
		for(unsigned uSample = 0; uSample < vsInputIQData.size(); uSample += 2) {
			vsOutputIQData[uSample] = vsInputIQData[uSample + 1];
			vsOutputIQData[uSample + 1] = vsInputIQData[uSample];
		}
	}
}
