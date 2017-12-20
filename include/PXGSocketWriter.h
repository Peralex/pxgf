#ifndef __PXG_SOCKET_WRITER_H
#define __PXG_SOCKET_WRITER_H

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

#ifndef __unix__
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#else
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif //unix

#include "PXGWriterBase.h"
#include "ChunkHandler.h"

namespace pxgf
{
	/// PXG Writer class that uses a socket to write its PXGF stream to
	class cPXGSocketWriter : public cPXGWriterBase
	{
	public:
		/// Constructor
		/// @param hSocket Valid socket to write stream to
		/// @param bBigEndian Endian to write stream in
#ifndef __unix__
		cPXGSocketWriter(SOCKET hSocket, bool bBigEndian);
#else
		cPXGSocketWriter(int hSocket, bool bBigEndian);
#endif
		/// Sets the UDP socket to write to.  This is useful where a UDP socket is not connected
		/// @param hSocketDest The address to write to.
		void SetDatagramDest(sockaddr_in hSocketDest);

		/// Defines the socket MTU size, default is 1500
		/// @param uMTU Size in bytes of maximum transmission unit
		void SetMTUSize(unsigned uMTU)
		{
			m_uUDP_MTU = (uMTU - 40);
		}

	protected:
		/// Perform a blocking read of the desired length and throw eof exception
		/// @param pData Destination for read data
		/// @param iLength Number of bytes to read
		virtual void write(const char *pData, std::streamsize iLength) override;

	private:
#ifndef __unix__
		SOCKET m_hSocket; ///< Handle to the system socket
#else //unix
		int m_hSocket; ///< Handle to the system socket
#endif
		bool m_bIsSocketUDP;
		sockaddr_in m_DatagramDest;
		unsigned m_uUDP_MTU; ///< UDP parameters to set up, if socket is not TCP/IP
	};
}
#endif
