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

#include "handlers/ChunkHandlerSR__.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerSR__::cChunkHandlerSR__()
	  : m_pCallbackSR__Handler(nullptr)
	{
	}

	void cChunkHandlerSR__::registerCallbackHandler(cCallbackSR__ *pHandler)
	{
		m_pCallbackSR__Handler = pHandler;
	}

	bool cChunkHandlerSR__::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		if(vchData.size() != 8) {
			return false;
		}
		if(m_pCallbackSR__Handler == nullptr) {
			return true;
		}

		int64_t lSampleRate_uHz = *(int64_t *)vchData.data();
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT64(lSampleRate_uHz);

		m_pCallbackSR__Handler->callbackSR__(lSampleRate_uHz);
		return true;
	}

	void cChunkHandlerSR__::writeChunkSR__(cPXGWriterBase &stream, int64_t lSampleRate_uHz) const
	{
		stream.writeChunkHeader(*this, 8);
		stream.writeInt64(lSampleRate_uHz);
	}
}
