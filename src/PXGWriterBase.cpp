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

#include <sstream>
#include <cstring>

#include "PXGWriterBase.h"
#include "SwapEndian.h"
#include "exceptions/PxgfIllegalArgumentException.h"

namespace pxgf
{
	using namespace std;

	cPXGWriterBase::cPXGWriterBase(const bool bBigEndian) :
		m_bBigEndian(bBigEndian),
		m_bSwapEndian(getIsLocalBigEndian() != m_bBigEndian)
	{
	}

	bool cPXGWriterBase::getIsLocalBigEndian()
	{
		long L = 1;
		void *Ptr = &L;
		char B = *(char *)Ptr;
		return (B == 0);
	}

	bool cPXGWriterBase::getIsBigEndian() const
	{
		return m_bBigEndian;
	}

	void cPXGWriterBase::setEndianByteOrdering(bool bBigEndian)
	{
		if(m_bBigEndian != bBigEndian) {
			m_bBigEndian = bBigEndian;
			m_bSwapEndian = getIsLocalBigEndian() != m_bBigEndian;
		}
	}

	void cPXGWriterBase::writeChunkHeader(const cChunkHandler &handler, int iLength)
	{
		if(iLength > getMaxChunkLength()) {
			stringstream strstrMessage;
			strstrMessage << "length of chunk must be less than " << getMaxChunkLength();
			throw cPxgfIllegalArgumentException(strstrMessage.str());
		}
		// make sure that length is multiple of 4 bytes
		if((iLength & 0x3) != 0)
			throw cPxgfIllegalArgumentException("length of chunk must fill an integer number of 32-bit words");

		m_piChunkHeader[0] = getSynchronisationWord();
		m_piChunkHeader[1] = handler.getChunkType();
		m_piChunkHeader[2] = iLength;
		if(m_bSwapEndian) {
			SWAP_INT(m_piChunkHeader[0]);
			SWAP_INT(m_piChunkHeader[1]);
			SWAP_INT(m_piChunkHeader[2]);
		}
		write((const char *)m_piChunkHeader, 12); // 12 = sizeof(piChunkHeader)
	}

	void cPXGWriterBase::writeInteger(int32_t iValue)
	{
		if(m_bSwapEndian)
			SWAP_INT(iValue);
		write((const char *)&iValue, sizeof(iValue));
	}

	void cPXGWriterBase::writeInt64(int64_t lValue)
	{
		if(m_bSwapEndian)
			SWAP_INT64(lValue);
		write((const char *)&lValue, sizeof(lValue));
	}

	void cPXGWriterBase::writeFloat(float fValue)
	{
		if(m_bSwapEndian)
			SWAP_FLOAT(fValue);
		write((const char *)&fValue, sizeof(fValue));
	}

	void cPXGWriterBase::writeShortArray(const vector<short> &vsData)
	{
		if(m_bSwapEndian) {
			vector<short> vsSwappedData(vsData);
			for(unsigned uI = 0; uI < vsSwappedData.size(); uI++)
				SWAP_SHORT(vsSwappedData[uI]);
			write((const char *)&vsSwappedData[0], vsSwappedData.size() * sizeof(short));
		}
		else
			write((const char *)&vsData[0], vsData.size() * sizeof(short));
	}

	void cPXGWriterBase::writeIntegerArray(const vector<int> &viData)
	{
		if(m_bSwapEndian) {
			vector<int> viSwappedData(viData);
			for(unsigned uI = 0; uI < viSwappedData.size(); uI++)
				SWAP_INT(viSwappedData[uI]);
			write((const char *)&viSwappedData[0], viSwappedData.size() * sizeof(int));
		}
		else
			write((const char *)&viData[0], viData.size() * sizeof(int));
	}

	void cPXGWriterBase::writeFloatArray(const vector<float> &vfData)
	{
		if(m_bSwapEndian) {
			vector<float> vfSwappedData(vfData);
			for(unsigned uI = 0; uI < vfSwappedData.size(); uI++)
				SWAP_FLOAT(vfSwappedData[uI]);
			write((const char *)&vfSwappedData[0], vfSwappedData.size() * sizeof(float));
		}
		else
			write((const char *)&vfData[0], vfData.size() * sizeof(float));
	}

	void cPXGWriterBase::writeCharArray(const std::vector<char> &vchData)
	{
		write((const char *)vchData.data(), vchData.size());
	}

	void cPXGWriterBase::writeString(const string &strMessage)
	{
		write(strMessage.c_str(), strMessage.length());
	}

	void cPXGWriterBase::writeRawChunk(int iType, const vector<char> &vchData)
	{
		writeInteger(getSynchronisationWord());
		writeInteger(iType);
		writeInteger(vchData.size());
		if(!vchData.empty()) {
			write(vchData.data(), vchData.size());
		}
	}
}
