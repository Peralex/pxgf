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

#include "handlers/ChunkHandlerGIQP.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerGIQP::cChunkHandlerGIQP()
	  : m_pCallbackGIQPHandler(nullptr)
	{
	}

	void cChunkHandlerGIQP::registerCallbackHandler(cCallbackGIQP *pHandler)
	{
		m_pCallbackGIQPHandler = pHandler;
	}

	bool cChunkHandlerGIQP::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		if(m_pCallbackGIQPHandler == nullptr) {
			return true;
		}

		int iNumChannels = *(int *)vchData.data();
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT(iNumChannels);
		// check that the block has the correct amount of data
		if(vchData.size() != unsigned(iNumChannels * 4 + 12)) {
			return false;
		}

		bool bIsInputIQPacked;
		int iIQPacking = *(int *)&vchData[4];
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT(iIQPacking);
		switch(iIQPacking) {
		case 0: bIsInputIQPacked = false; break;
		case 1: bIsInputIQPacked = true; break;
		default: return false;
		}
		int iIncrement = *(int *)&vchData[8];
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT(iIncrement);
		vector<int> aiChannelOffsets(iNumChannels);
		memcpy(&aiChannelOffsets[0], &vchData[12], iNumChannels * sizeof(int));
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
			for(vector<int>::iterator it = aiChannelOffsets.begin(); it != aiChannelOffsets.end(); it++)
				SWAP_INT(*it);
		}

		m_pCallbackGIQPHandler->callbackGIQP(cPackingGIQP(bIsInputIQPacked, iIncrement, aiChannelOffsets));
		return true;
	}

	void cChunkHandlerGIQP::writeChunkGIQP(cPXGWriterBase &stream, const cPackingGIQP &oPacking) const
	{
		stream.writeChunkHeader(*this, oPacking.getNumChannels() * 4 + 12);
		stream.writeInteger(oPacking.getNumChannels());
		if(oPacking.getIsIQPacked())
			stream.writeInteger(1);
		else
			stream.writeInteger(0);
		stream.writeInteger(oPacking.getIncrement());
		stream.writeIntegerArray(oPacking.getChannelOffsets());
	}
}
