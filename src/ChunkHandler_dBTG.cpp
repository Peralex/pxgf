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

#include "handlers/ChunkHandler_dBTG.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandler_dBTG::cChunkHandler_dBTG()
	  : m_pCallback_dBTGHandler(nullptr)
	{
	}

	void cChunkHandler_dBTG::registerCallbackHandler(cCallback_dBTG *pHandler)
	{
		m_pCallback_dBTGHandler = pHandler;
	}

	bool cChunkHandler_dBTG::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		if(vchData.size() != 4) {
			return false;
		}
		if(m_pCallback_dBTGHandler == nullptr) {
			return true;
		}

		float fTotalGain_dB = *(float *)vchData.data();
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_FLOAT(fTotalGain_dB);

		m_pCallback_dBTGHandler->callback_dBTG(fTotalGain_dB);
		return true;
	}

	void cChunkHandler_dBTG::writeChunk_dBTG(cPXGWriterBase &stream, float fTotalGain_dB) const
	{
		stream.writeChunkHeader(*this, 4);
		stream.writeFloat(fTotalGain_dB);
	}
}
