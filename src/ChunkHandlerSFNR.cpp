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

#include "handlers/ChunkHandlerSFNR.h"
#include "handlers/PxgfHandlerException.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerSFNR::cChunkHandlerSFNR()
	  : m_pCallbackSFNRHandler(nullptr)
	{
	}

	void cChunkHandlerSFNR::registerCallbackHandler(cCallbackSFNR *pHandler)
	{
		m_pCallbackSFNRHandler = pHandler;
	}

	bool cChunkHandlerSFNR::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		// check that there is enough data for an even number of floats
		if(((vchData.size()) & 0x3) != 0)
			return false;
		if(m_pCallbackSFNRHandler == nullptr) {
			return true;
		}

		int64_t lTimestamp_ns = *(int64_t *)vchData.data();
		int iLength = (vchData.size() - 8) / 4; // 4 bytes in a float
		//vector<float> vfRealData (iLength);
		m_vfRealData.resize(iLength);
		memcpy(&m_vfRealData[0], &vchData[8], iLength * sizeof(float));
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
			SWAP_INT64(lTimestamp_ns);
			for(vector<float>::iterator it = m_vfRealData.begin(); it != m_vfRealData.end(); it++)
				SWAP_FLOAT(*it);
		}

		m_pCallbackSFNRHandler->callbackSFNR(lTimestamp_ns, m_vfRealData);
		return true;
	}

	void cChunkHandlerSFNR::writeChunkSFNR(cPXGWriterBase &stream, int64_t lTimestamp_ns, const vector<float> &vfRealData) const
	{
		stream.writeChunkHeader(*this, 8 + vfRealData.size() * 4); // 4 bytes in a float
		stream.writeInt64(lTimestamp_ns);

		// check that there are an even number of elements
		if((vfRealData.size() & 0x1) != 0)
			throw cPxgfHandlerException("The length of vsIqData must be even.");
		stream.writeFloatArray(vfRealData);
	}
}
