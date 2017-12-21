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

#include "handlers/ChunkHandlerFFS_.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerFFS_::cChunkHandlerFFS_()
	  : m_pCallbackFFS_Handler(nullptr)
	{
	}

	void cChunkHandlerFFS_::registerCallbackHandler(cCallbackFFS_ *pHandler)
	{
		m_pCallbackFFS_Handler = pHandler;
	}

	bool cChunkHandlerFFS_::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		if(vchData.size() != 4) {
			return false;
		}
		if(m_pCallbackFFS_Handler == nullptr) {
			return true;
		}

		float fFullScale = *(float *)vchData.data();
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_FLOAT(fFullScale);

		m_pCallbackFFS_Handler->callbackFFS_(fFullScale);
		return true;
	}

	void cChunkHandlerFFS_::writeChunkFFS_(cPXGWriterBase &stream, float fFullscale) const
	{
		stream.writeChunkHeader(*this, 4);
		stream.writeFloat(fFullscale);
	}
}
