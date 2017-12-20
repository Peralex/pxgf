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

#include "handlers/ChunkHandlerBWOF.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerBWOF::cChunkHandlerBWOF()
	  : m_pCallbackBWOFHandler(nullptr)
	{
	}

	void cChunkHandlerBWOF::registerCallbackHandler(cCallbackBWOF *pHandler)
	{
		m_pCallbackBWOFHandler = pHandler;
	}

	bool cChunkHandlerBWOF::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		if(vchData.size() != 16) {
			return false;
		}
		if(m_pCallbackBWOFHandler == nullptr) {
			return true;
		}

		int64_t lBandwidth_uHz = *(int64_t *)vchData.data();
		int64_t lOffsetFrequency_uHz = *(int64_t *)&vchData[8];
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
			SWAP_INT64(lBandwidth_uHz);
			SWAP_INT64(lOffsetFrequency_uHz);
		}

		m_pCallbackBWOFHandler->callbackBWOF(lBandwidth_uHz, lOffsetFrequency_uHz);
		return true;
	}

	void cChunkHandlerBWOF::writeChunkBWOF(cPXGWriterBase &stream, int64_t lBandwidth_uHz, int64_t lOffsetFrequency_uHz) const
	{
		stream.writeChunkHeader(*this, 16);
		stream.writeInt64(lBandwidth_uHz);
		stream.writeInt64(lOffsetFrequency_uHz);
	}
}
