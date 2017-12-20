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

#include "exceptions/PxgfException.h"
#include "PXGSocketWriter.h"

using namespace pxgf;

#ifndef __unix__
cPXGSocketWriter::cPXGSocketWriter(SOCKET hSocket, bool bBigEndian) :
#else
cPXGSocketWriter::cPXGSocketWriter(int hSocket, bool bBigEndian) :
#endif
	cPXGWriterBase(bBigEndian),
	m_hSocket(hSocket),
	m_bIsSocketUDP(false),
	m_uUDP_MTU(1460)
{
}

void cPXGSocketWriter::SetDatagramDest(sockaddr_in hSocketDest)
{
	m_DatagramDest = hSocketDest;
	m_bIsSocketUDP = true;
}

void cPXGSocketWriter::write(const char *pcData, std::streamsize iLength)
{
	if(iLength == 0)
		return;
	if(m_hSocket == INVALID_SOCKET)
		throw cPxgfException("Invalid socket");
	int iSent;
	while(iLength) {
		if(m_bIsSocketUDP) {
			int iLen = iLength > m_uUDP_MTU ? m_uUDP_MTU : iLength;
			iSent = sendto(m_hSocket, pcData, iLen, 0, (struct sockaddr *)&m_DatagramDest, sizeof(m_DatagramDest));
		}
		else
			iSent = send(m_hSocket, pcData, iLength, 0);

		//If it receives SOCKET_ERROR, the socket is almost certainly closed.  It should
		//   never receive 0 - when the socket is closed on the other end, it should receive
		//   an error.  Leave the check for 0 in here though in case send starts behaving
		//   like recv at some stage.
		if(iSent == SOCKET_ERROR || iSent == 0) {
			//User app is responsible for shutting down and calling close on the socket
			throw cPxgfException("Socket closed or non-blocking");
		}
		iLength -= iSent;
		pcData += iSent;
	}
}
