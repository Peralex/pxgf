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

#include "handlers/ChunkHandlerSOFH.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerSOFH::cChunkHandlerSOFH()
	  : m_pCallbackSOFHHandler(nullptr)
	{
	}

	void cChunkHandlerSOFH::registerCallbackHandler(cCallbackSOFH *pHandler)
	{
		m_pCallbackSOFHHandler = pHandler;
	}

	bool cChunkHandlerSOFH::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		if(vchData.size() != 4) {
			return false;
		}
		if(m_pCallbackSOFHHandler == nullptr) {
			return true;
		}

		int iFormat = *(int *)vchData.data();
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT(iFormat);

		m_pCallbackSOFHHandler->callbackSOFH(iFormat);
		return true;
	}

	void cChunkHandlerSOFH::writeChunkSOFH(cPXGWriterBase &Writer, int iFormat) const
	{
		Writer.writeChunkHeader(*this, 4);
		Writer.writeInteger(iFormat);
	}
}
