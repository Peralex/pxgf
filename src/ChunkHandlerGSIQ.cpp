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

#include "handlers/ChunkHandlerGSIQ.h"
#include "handlers/PxgfHandlerException.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerGSIQ::cChunkHandlerGSIQ()
	  : m_pCallbackGSIQHandler(nullptr)
	{
	}

	void cChunkHandlerGSIQ::registerCallbackHandler(cCallbackGSIQ *pHandler)
	{
		m_pCallbackGSIQHandler = pHandler;
	}

	bool cChunkHandlerGSIQ::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		// check that there is enough data for an even number of shorts
		if(((vchData.size()) & 0x3) != 0)
			return false;
		if(m_pCallbackGSIQHandler == nullptr)
			return true;

		int64_t lTimestamp_us = *(int64_t *)vchData.data();
		int iLength = (vchData.size() - 8) / 2;
		vector<short> vsIqData(iLength);
		memcpy(&vsIqData[0], &vchData[8], iLength * sizeof(short));
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
			SWAP_INT64(lTimestamp_us);
			for(auto it = vsIqData.begin(); it != vsIqData.end(); it++)
				SWAP_SHORT(*it);
		}

		m_pCallbackGSIQHandler->callbackGSIQ(lTimestamp_us, vsIqData);
		return true;
	}

	void cChunkHandlerGSIQ::writeChunkGSIQ(cPXGWriterBase &stream, int64_t lTimestamp_us, const vector<short> &vsIqData) const
	{
		stream.writeChunkHeader(*this, 8 + vsIqData.size() * 2);
		stream.writeInt64(lTimestamp_us);
		stream.writeShortArray(vsIqData);
	}

	void cChunkHandlerGSIQ::repack(const cPackingGIQP &oInputPacking, const cPackingGIQP &oOutputPacking, const vector<short> &vsInputIQData, vector<short> &vsOutputIQData)
	{
		if(oInputPacking.getNumChannels() != oOutputPacking.getNumChannels())
			throw cPxgfHandlerException("Packing definitions are not compatible!");
		if(oInputPacking.equals(oOutputPacking)) {
			vsOutputIQData = vsInputIQData;
			return;
		}
		// check that there are an even number of elements
		if((vsInputIQData.size() & 0x1) != 0)
			throw cPxgfHandlerException("The length of vsInputIQData must be even.");
		vsOutputIQData.resize(vsInputIQData.size());
		int iNumChannels = oInputPacking.getNumChannels();
		int iNumSamples = vsInputIQData.size() / (iNumChannels * 2);
		int iInputIncr = oInputPacking.getIncrement() * 2;
		int iOutputIncr = oOutputPacking.getIncrement() * 2;
		int iInputIndex, iOutputIndex;
		for(int iChannel = 0; iChannel < iNumChannels; iChannel++) {
			iInputIndex = oInputPacking.getChannelOffsets()[iChannel] * 2;
			iOutputIndex = oOutputPacking.getChannelOffsets()[iChannel] * 2;
			for(int iSample = 0; iSample < iNumSamples; iSample++) {
				vsOutputIQData[iOutputIndex] = vsInputIQData[iInputIndex];
				vsOutputIQData[iOutputIndex + 1] = vsInputIQData[iInputIndex + 1];
				iInputIndex += iInputIncr;
				iOutputIndex += iOutputIncr;
			}
		}
		// swap IQ data
		if(oInputPacking.getIsIQPacked() != oOutputPacking.getIsIQPacked()) {
			short sSwap;
			for(unsigned uSample = 0; uSample < vsOutputIQData.size(); uSample += 2) {
				sSwap = vsOutputIQData[uSample];
				vsOutputIQData[uSample] = vsOutputIQData[uSample + 1];
				vsOutputIQData[uSample + 1] = sSwap;
			}
		}
	}
}
