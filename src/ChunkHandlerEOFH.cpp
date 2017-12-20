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

#include "handlers/ChunkHandlerEOFH.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerEOFH::cChunkHandlerEOFH()
	  : m_pCallbackEOFHHandler(nullptr)
	{
	}

	void cChunkHandlerEOFH::registerCallbackHandler(cCallbackEOFH *pHandler)
	{
		m_pCallbackEOFHHandler = pHandler;
	}

	bool cChunkHandlerEOFH::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		if(vchData.size() != 0) {
			return false;
		}
		if(m_pCallbackEOFHHandler == nullptr) {
			return true;
		}

		m_pCallbackEOFHHandler->callbackEOFH();
		return true;
	}

	void cChunkHandlerEOFH::writeChunkEOFH(cPXGWriterBase &stream) const
	{
		stream.writeChunkHeader(*this, 0);
	}
}
