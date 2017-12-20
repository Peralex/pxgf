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

#include "handlers/ChunkHandlerGRG_.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerGRG_::cChunkHandlerGRG_()
	  : m_pCallbackGRG_Handler(nullptr)
	{
	}

	void cChunkHandlerGRG_::registerCallbackHandler(cCallbackGRG_ *pHandler)
	{
		m_pCallbackGRG_Handler = pHandler;
	}

	bool cChunkHandlerGRG_::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		if(m_pCallbackGRG_Handler == nullptr) {
			return true;
		}

		int iNumChannels = *(int *)vchData.data();
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT(iNumChannels);

		// check to see if there is the correct amount of data in the block
		if(iNumChannels != int(vchData.size() - 4) / 4) {
			return false;
		}

		vector<float> vfGains_dB(iNumChannels);
		memcpy(&vfGains_dB[0], &vchData[4], iNumChannels * sizeof(float));
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
			for(vector<float>::iterator it = vfGains_dB.begin(); it != vfGains_dB.end(); it++)
				SWAP_FLOAT(*it);
		}

		m_pCallbackGRG_Handler->callbackGRG_(vfGains_dB);
		return true;
	}

	void cChunkHandlerGRG_::writeChunkGRG_(cPXGWriterBase &stream, vector<float> vfGains_dB) const
	{
		stream.writeChunkHeader(*this, 4 + 4 * vfGains_dB.size());
		stream.writeInteger(vfGains_dB.size());
		for(unsigned uChannel = 0; uChannel < vfGains_dB.size(); uChannel++)
			stream.writeFloat(vfGains_dB[uChannel]);
	}
}
