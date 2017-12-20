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

#include "PXGReaderBase.h"
#include "SwapEndian.h"
#include "exceptions/PxgfException.h"

namespace pxgf
{
	using namespace std;

	cPXGReaderBase::cPXGReaderBase()
	  : m_bRemoveFromStream(true),
		m_pStreamWriter(nullptr),
		m_pResetHandler(nullptr),
		m_iCopyMode(0),
		m_eEndian(Endian_Auto),
		m_bBigEndian(true)
	{
	}

	void cPXGReaderBase::registerStreamWriter(cPXGWriterBase *pStreamWriter, int iCopyMode)
	{
		m_pStreamWriter = pStreamWriter;
		m_iCopyMode = iCopyMode;
	}

	void cPXGReaderBase::registerResetHandler(cResetHandler *pHandler)
	{
		m_pResetHandler = pHandler;
	}

	void cPXGReaderBase::registerChunkHandler(cChunkHandler &handler)
	{
		m_chunkHandlers.insert(map<int, cChunkHandler *>::value_type(handler.getChunkType(), &handler));
	}

	void cPXGReaderBase::removeChunkHandler(const cChunkHandler &handler)
	{
		m_chunkHandlers.erase(handler.getChunkType());
	}

	void cPXGReaderBase::removeAllChunkHandlers()
	{
		m_chunkHandlers.clear();
	}

	void cPXGReaderBase::processPXGStream(bool bBigEndian)
	{
		m_bBigEndian = bBigEndian;
		m_eEndian = m_bBigEndian ? Endian_Big: Endian_Little;
		processPXGStream();
	}

	void cPXGReaderBase::processPXGStream(eEndianType EndianType)
	{
		m_eEndian = EndianType;
		m_bBigEndian = (m_eEndian == Endian_Big); //If Endian is Endian_Auto then it doesn't matter what we set m_bBigEndian to.
		processPXGStream();
	}

	void cPXGReaderBase::processPXGStream()
	{
		std::lock_guard<std::mutex> Guard(m_ProcessMutex);
		// clear the flag so that we don't give up immediately
		m_bRemoveFromStream = false;
		while(true) {
			resynchronise();
			// we have now achieved synchronisation so reset the application state
			if(m_pResetHandler != nullptr)
				m_pResetHandler->resetState();
			// now process chunks sequentially until something goes wrong
			while(true) {
				if(!processChunk())
					break;
				if(m_bRemoveFromStream)
					return;
				// make sure that the synchronisation word is next else we have lost sync
				if(!checkSynchronised())
					break;
			}
			if(m_bRemoveFromStream)
				return;
		}
	}

	void cPXGReaderBase::removeFromStream()
	{
		m_bRemoveFromStream = true;
	}

	void cPXGReaderBase::WaitReadComplete()
	{
		std::lock_guard<std::mutex> Guard(m_ProcessMutex);
	}

	bool cPXGReaderBase::processChunk()
	{
		// read the chunk type and length
		int pTypeLength[2];
		readTwoIntegers(pTypeLength);
		int &iType = pTypeLength[0];
		int &iLength = pTypeLength[1];
		// If the length is too great then we must somehow have lost synchronisation.  If length is negative, then
		//    this probably indicates loss of sync or corruption
		if(iLength > cPXGWriterBase::getMaxChunkLength() || iLength < 0) {
			return false;
		}
		if((iLength & 0x3) != 0) {
			return false;
		}
		// read the data for the
		m_vchData.resize(iLength);
		if(iLength)
			blockingRead(&m_vchData[0], iLength);

		// lookup handler and process if possible
		auto it = m_chunkHandlers.find(iType);
		if(it != m_chunkHandlers.end()) // chunk is registered
		{
			try {
				// make a copy of the chunk if required
				if((m_pStreamWriter != nullptr) && (m_iCopyMode == 0)) {
					m_pStreamWriter->setEndianByteOrdering(m_bBigEndian);
					m_pStreamWriter->writeRawChunk(iType, m_vchData);
				}
			}
			catch(cPxgfException &) {
				m_pStreamWriter = nullptr;
			}

			it->second->processChunk(m_vchData, m_bBigEndian);
		}
		else // chunk is NOT registered
		{
			try {
				// make a copy of the chunk if required
				if(m_pStreamWriter != nullptr) {
					m_pStreamWriter->setEndianByteOrdering(m_bBigEndian);
					m_pStreamWriter->writeRawChunk(iType, m_vchData);
				}
			}
			catch(cPxgfException &) {
				m_pStreamWriter = nullptr;
			}
		}
		// otherwise just skip over the chunk
		return true;
	}

	void cPXGReaderBase::resynchronise()
	{
		unsigned uBigEndianSync = 0;
		unsigned uLittleEndianSync = 0;

		bool bSync = false;
		int iNext = 0;
		while(!bSync && !m_bRemoveFromStream) {
			unsigned char ucByte;
			blockingRead((char *)&ucByte, 1);
			iNext = ucByte;

			uBigEndianSync <<= 8;
			uBigEndianSync |= iNext;
			uLittleEndianSync >>= 8;
			uLittleEndianSync |= ((iNext) << 24);

			if(m_eEndian == Endian_Big) {
				bSync = (uBigEndianSync == unsigned(cPXGWriterBase::getSynchronisationWord()));
			}
			else if(m_eEndian == Endian_Little) {
				bSync = (uLittleEndianSync == unsigned(cPXGWriterBase::getSynchronisationWord()));
			}
			else if(m_eEndian == Endian_Auto) {
				if(uBigEndianSync == unsigned(cPXGWriterBase::getSynchronisationWord())) {
					m_bBigEndian = true;
					bSync = true;
				}
				else if(uLittleEndianSync == unsigned(cPXGWriterBase::getSynchronisationWord())) {
					m_bBigEndian = false;
					bSync = true;
				}
			}
			else
				throw cPxgfException("Invalid endian specification.");
		}
	}

	bool cPXGReaderBase::checkSynchronised()
	{
		int iNextWord = readInteger();
		bool bSync = iNextWord == cPXGWriterBase::getSynchronisationWord();
		if(!bSync && m_eEndian == Endian_Auto) {
			SWAP_INT(iNextWord);
			bSync = iNextWord == cPXGWriterBase::getSynchronisationWord();
			if(bSync) {
				// Endian swap detected
				m_bBigEndian = !m_bBigEndian;
			}
		}
		return bSync;
	}

	int cPXGReaderBase::readInteger()
	{
		int iValue;
		blockingRead((char *)&iValue, sizeof(int));
		if(cPXGWriterBase::getIsLocalBigEndian() != m_bBigEndian)
			SWAP_INT(iValue);
		return iValue;
	}

	void cPXGReaderBase::readTwoIntegers(int pTwoIntegers[])
	{
		blockingRead((char *)pTwoIntegers, 2 * sizeof(int));
		if(cPXGWriterBase::getIsLocalBigEndian() != m_bBigEndian) {
			SWAP_INT(pTwoIntegers[0]);
			SWAP_INT(pTwoIntegers[1]);
		}
	}
}
