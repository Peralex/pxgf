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

#include <cstring>

#include "handlers/ChunkHandlerGFNC.h"
#include "handlers/PxgfHandlerException.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerGFNC::cChunkHandlerGFNC()
	  : m_pCallbackGFNCHandler(nullptr)
	{
	}

	void cChunkHandlerGFNC::registerCallbackHandler(cCallbackGFNC *pHandler)
	{
		m_pCallbackGFNCHandler = pHandler;
	}

	bool cChunkHandlerGFNC::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		// check that there is enough data for an even number of shorts
		if(((vchData.size()) & 0x4) != 0)
			return false;
		if(m_pCallbackGFNCHandler == nullptr)
			return true;

		int64_t lTimestamp_ns = *(int64_t *)vchData.data();
		int iLength = (vchData.size() - 8) / 4;
		vector<float> vfIqData(iLength);
		memcpy(&vfIqData[0], &vchData[8], iLength * sizeof(float));
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
			SWAP_INT64(lTimestamp_ns);
			for(auto it = vfIqData.begin(); it != vfIqData.end(); it++)
				SWAP_FLOAT(*it);
		}

		m_pCallbackGFNCHandler->callbackGFNC(lTimestamp_ns, vfIqData);
		return true;
	}

	void cChunkHandlerGFNC::writeChunkGFNC(cPXGWriterBase &stream, int64_t lTimestamp_ns, const vector<float> &vfIqData) const
	{
		stream.writeChunkHeader(*this, 8 + vfIqData.size() * 4);
		stream.writeInt64(lTimestamp_ns);
		stream.writeFloatArray(vfIqData);
	}

	void cChunkHandlerGFNC::repack(const cPackingGIQP &oInputPacking, const cPackingGIQP &oOutputPacking, const vector<float> &vfInputIQData, vector<float> &vfOutputIQData)
	{
		if(oInputPacking.getNumChannels() != oOutputPacking.getNumChannels())
			throw cPxgfHandlerException("Packing definitions are not compatible!");
		if(oInputPacking.equals(oOutputPacking)) {
			vfOutputIQData = vfInputIQData;
			return;
		}
		// check that there are an even number of elements
		if((vfInputIQData.size() & 0x1) != 0)
			throw cPxgfHandlerException("The length of vsInputIQData must be even.");
		vfOutputIQData.resize(vfInputIQData.size());
		int iNumChannels = oInputPacking.getNumChannels();
		int iNumSamples = vfInputIQData.size() / (iNumChannels * 2);
		int iInputIncr = oInputPacking.getIncrement() * 2;
		int iOutputIncr = oOutputPacking.getIncrement() * 2;
		int iInputIndex, iOutputIndex;
		for(int iChannel = 0; iChannel < iNumChannels; iChannel++) {
			iInputIndex = oInputPacking.getChannelOffsets()[iChannel] * 2;
			iOutputIndex = oOutputPacking.getChannelOffsets()[iChannel] * 2;
			for(int iSample = 0; iSample < iNumSamples; iSample++) {
				vfOutputIQData[iOutputIndex] = vfInputIQData[iInputIndex];
				vfOutputIQData[iOutputIndex + 1] = vfInputIQData[iInputIndex + 1];
				iInputIndex += iInputIncr;
				iOutputIndex += iOutputIncr;
			}
		}
		// swap IQ data
		if(oInputPacking.getIsIQPacked() != oOutputPacking.getIsIQPacked()) {
			float fSwap;
			for(unsigned uSample = 0; uSample < vfOutputIQData.size(); uSample += 2) {
				fSwap = vfOutputIQData[uSample];
				vfOutputIQData[uSample] = vfOutputIQData[uSample + 1];
				vfOutputIQData[uSample + 1] = fSwap;
			}
		}
	}
}
