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

#include "handlers/ChunkHandlerSFNC.h"
#include "handlers/PxgfHandlerException.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerSFNC::cChunkHandlerSFNC()
	  : m_pCallbackSFNCHandler(nullptr)
	{
	}

	void cChunkHandlerSFNC::registerCallbackHandler(cCallbackSFNC *pHandler)
	{
		m_pCallbackSFNCHandler = pHandler;
	}

	bool cChunkHandlerSFNC::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		// check that there is enough data for an even number of floats
		if(((vchData.size()) & 0x3) != 0)
			return false;
		if(m_pCallbackSFNCHandler == nullptr) {
			return true;
		}

		int64_t lTimestamp_ns = *(int64_t *)vchData.data();
		int iLength = (vchData.size() - 8) / sizeof(float);
		//vector<float> vfIqData (iLength);
		m_vfIqData.resize(iLength);
		memcpy(&m_vfIqData[0], &vchData[8], iLength * sizeof(float));
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
			SWAP_INT64(lTimestamp_ns);
			for(vector<float>::iterator it = m_vfIqData.begin(); it != m_vfIqData.end(); it++)
				SWAP_FLOAT(*it);
		}

		m_pCallbackSFNCHandler->callbackSFNC(lTimestamp_ns, m_vfIqData);
		return true;
	}

	void cChunkHandlerSFNC::writeChunkSFNC(cPXGWriterBase &stream, int64_t lTimestamp_ns, const vector<float> &vfIqData) const
	{
		stream.writeChunkHeader(*this, 8 + vfIqData.size() * sizeof(float));
		stream.writeInt64(lTimestamp_ns);
		stream.writeFloatArray(vfIqData);
	}

	void cChunkHandlerSFNC::repack(const cPackingSIQP &oInputPacking, const cPackingSIQP &oOutputPacking, const vector<float> &vfInputIQData, vector<float> &vfOutputData)
	{
		if(oInputPacking.equals(oOutputPacking)) {
			vfOutputData = vfInputIQData;
			return;
		}
		// check that there are an even number of elements
		if((vfInputIQData.size() & 0x1) != 0)
			throw cPxgfHandlerException("The length of vfInputIQData must be even.");

		// swap IQ data
		vfOutputData.resize(vfInputIQData.size());
		for(unsigned uSample = 0; uSample < vfInputIQData.size(); uSample += 2) {
			vfOutputData[uSample] = vfInputIQData[uSample + 1];
			vfOutputData[uSample + 1] = vfInputIQData[uSample];
		}
	}
}
