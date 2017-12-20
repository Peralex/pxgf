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

#include "handlers/ChunkHandlerCF__.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;

	cChunkHandlerCF__::cChunkHandlerCF__()
	  : m_pCallbackCF__Handler(nullptr)
	{
	}

	void cChunkHandlerCF__::registerCallbackHandler(cCallbackCF__ *pHandler)
	{
		m_pCallbackCF__Handler = pHandler;
	}

	bool cChunkHandlerCF__::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		if(vchData.size() != 8)
			return false;

		if(m_pCallbackCF__Handler == nullptr)
			return true;

		int64_t lCentreFrequency_uHz = *(int64_t *)vchData.data();
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT64(lCentreFrequency_uHz);

		m_pCallbackCF__Handler->callbackCF__(lCentreFrequency_uHz);
		return true;
	}

	void cChunkHandlerCF__::writeChunkCF__(cPXGWriterBase &stream, int64_t lCentreFrequency_uHz) const
	{
		stream.writeChunkHeader(*this, 8);
		stream.writeInt64(lCentreFrequency_uHz);
	}
}
