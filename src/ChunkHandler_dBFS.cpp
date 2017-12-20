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

#include "handlers/ChunkHandler_dBFS.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandler_dBFS::cChunkHandler_dBFS()
	  : m_pCallback_dBFSHandler(nullptr)
	{
	}

	void cChunkHandler_dBFS::registerCallbackHandler(cCallback_dBFS *pHandler)
	{
		m_pCallback_dBFSHandler = pHandler;
	}

	bool cChunkHandler_dBFS::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		if(vchData.size() != 4) {
			return false;
		}
		if(m_pCallback_dBFSHandler == nullptr) {
			return true;
		}

		float f_dBFullScale_dBm = *(float *)vchData.data();
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_FLOAT(f_dBFullScale_dBm);

		m_pCallback_dBFSHandler->callback_dBFS(f_dBFullScale_dBm);
		return true;
	}

	void cChunkHandler_dBFS::writeChunk_dBFS(cPXGWriterBase &stream, float f_dBFullScale_dBm) const
	{
		stream.writeChunkHeader(*this, 4);
		stream.writeFloat(f_dBFullScale_dBm);
	}
}
