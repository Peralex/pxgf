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

#include "handlers/ChunkHandlerBW__.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerBW__::cChunkHandlerBW__()
	  : m_pCallbackBW__Handler(nullptr)
	{
	}

	void cChunkHandlerBW__::registerCallbackHandler(cCallbackBW__ *pHandler)
	{
		m_pCallbackBW__Handler = pHandler;
	}

	bool cChunkHandlerBW__::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		if(vchData.size() != 8) {
			return false;
		}
		if(m_pCallbackBW__Handler == nullptr) {
			return true;
		}

		int64_t lBandwidth_uHz = *(int64_t *)vchData.data();
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT64(lBandwidth_uHz);

		m_pCallbackBW__Handler->callbackBW__(lBandwidth_uHz);
		return true;
	}

	void cChunkHandlerBW__::writeChunkBW__(cPXGWriterBase &stream, int64_t lBandwidth_uHz) const
	{
		stream.writeChunkHeader(*this, 8);
		stream.writeInt64(lBandwidth_uHz);
	}
}
