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

#include <vector>

#include "handlers/ChunkHandlerSIQP.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerSIQP::cChunkHandlerSIQP()
	  : m_pCallbackSIQPHandler(nullptr),
		m_oIQPacking(true),
		m_oQIPacking(false)
	{
	}

	void cChunkHandlerSIQP::registerCallbackHandler(cCallbackSIQP *pHandler)
	{
		m_pCallbackSIQPHandler = pHandler;
	}

	bool cChunkHandlerSIQP::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		if(vchData.size() != 4) {
			return false;
		}
		if(m_pCallbackSIQPHandler == nullptr) {
			return true;
		}

		int iValue = *(int *)vchData.data();
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT(iValue);

		switch(iValue) {
		case 0: m_pCallbackSIQPHandler->callbackSIQP(m_oQIPacking); break;
		case 1: m_pCallbackSIQPHandler->callbackSIQP(m_oIQPacking); break;
		default: return false;
		}

		return true;
	}

	void cChunkHandlerSIQP::writeChunkSIQP(cPXGWriterBase &stream, const cPackingSIQP &oPacking) const
	{
		stream.writeChunkHeader(*this, 4);
		if(oPacking.getIsIQPacked())
			stream.writeInteger(1);
		else
			stream.writeInteger(0);
	}
}
