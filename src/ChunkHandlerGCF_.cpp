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

#include <cstring>

#include "handlers/ChunkHandlerGCF_.h"
#include "SwapEndian.h"

namespace pxgf
{
	using namespace std;
	cChunkHandlerGCF_::cChunkHandlerGCF_()
	  : m_pCallbackGCF_Handler(nullptr)
	{
	}

	void cChunkHandlerGCF_::registerCallbackHandler(cCallbackGCF_ *pHandler)
	{
		m_pCallbackGCF_Handler = pHandler;
	}

	bool cChunkHandlerGCF_::processChunk(const vector<char> &vchData, bool bBigEndian)
	{
		if(m_pCallbackGCF_Handler == nullptr) {
			return true;
		}

		int iNumChannels = *(int *)vchData.data();
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian)
			SWAP_INT(iNumChannels);

		// check to see if there is the correct amount of data in the block
		if(iNumChannels != int(vchData.size() - 4) / 8) {
			return false;
		}

		vector<int64_t> vlCentreFrequencies_uHz(iNumChannels);
		memcpy(&vlCentreFrequencies_uHz[0], &vchData[4], iNumChannels * sizeof(int64_t));
		if(cPXGWriterBase::getIsLocalBigEndian() != bBigEndian) {
			for(vector<int64_t>::iterator it = vlCentreFrequencies_uHz.begin(); it != vlCentreFrequencies_uHz.end(); it++)
				SWAP_INT64(*it);
		}

		m_pCallbackGCF_Handler->callbackGCF_(vlCentreFrequencies_uHz);
		return true;
	}

	void cChunkHandlerGCF_::writeChunkGCF_(cPXGWriterBase &stream, const vector<int64_t> &vlCentreFrequencies_uHz) const
	{
		stream.writeChunkHeader(*this, 4 + 8 * vlCentreFrequencies_uHz.size());
		stream.writeInteger(vlCentreFrequencies_uHz.size());
		for(unsigned uChannel = 0; uChannel < vlCentreFrequencies_uHz.size(); uChannel++)
			stream.writeInt64(vlCentreFrequencies_uHz[uChannel]);
	}
}
