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

#include "PXGSocketReader.h"
#include "exceptions/PxgfException.h"

namespace pxgf
{
	cPXGSocketReader::cPXGSocketReader() :
		m_hSocket(INVALID_SOCKET)
	{
	}
#ifndef __unix__
	cPXGSocketReader::cPXGSocketReader(SOCKET hSocket) :
		m_hSocket(hSocket)
	{
	}
#else
	cPXGSocketReader::cPXGSocketReader(int hSocket) :
		m_hSocket(hSocket)
	{
	}
#endif

#ifndef __unix__
	void cPXGSocketReader::processStream(SOCKET hSocket, bool bBigEndian)
#else
	void cPXGSocketReader::processStream(int hSocket, bool bBigEndian)
#endif
	{
		m_hSocket = hSocket;
		processPXGStream(bBigEndian);
		m_hSocket = INVALID_SOCKET;
	}

#ifndef __unix__
	void cPXGSocketReader::processStream(SOCKET hSocket, eEndianType EndianType)
#else
	void cPXGSocketReader::processStream(int hSocket, eEndianType EndianType)
#endif
	{
		m_hSocket = hSocket;
		processPXGStream(EndianType);
	}

	void cPXGSocketReader::blockingRead(char *pData, int iLength)
	{
		int iRead;
		if(m_hSocket == INVALID_SOCKET)
			throw cPxgfException("Invalid Socket");

		while(iLength) {
			iRead = recv(m_hSocket, pData, iLength, 0);
			if(iRead == 0 || iRead == SOCKET_ERROR) {
				m_hSocket = INVALID_SOCKET;
				throw cPxgfException("Socket closed");
			}
			iLength -= iRead;
			pData += iRead;
		}
	}
}
