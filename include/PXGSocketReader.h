#ifndef __PXGSOCKET_READER_H
#define __PXGSOCKET_READER_H
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

#include "PXGReaderBase.h"
#include "PXGStreamWriter.h"
#include "ResetHandler.h"

namespace pxgf
{
	/// PXG Reader class that accepts an input PXGF stream from a socket
	class cPXGSocketReader : public cPXGReaderBase
	{
	public:
		/// Constructor
		cPXGSocketReader();
#ifndef __unix__
		/// Constructor
		/// @param hSocket Socket handle for the socket that will be read from.
		cPXGSocketReader(SOCKET hSocket);
#else
		/// Constructor
		/// @param hSocket Socket handle for the socket that will be read from.
		cPXGSocketReader(int hSocket);
#endif

		/// Process an input stream
		//
		/// Calling this stream instructs the engine to start processing the given
		/// stream from the specfied socket and assuming the endian
		/// ordering specified. It will continuously process chunks until the socket
		/// connection is broken, or you specify that you wish it to
		/// stop by calling removeFromStream().(This processStream is deprecated
		/// and the other one which specfies endian by the eEndianType enum should
		/// be used instead).
		/// @param hSocket A valid socket to read the data stream from
		/// @param bBigEndian Is the stream big-endian byte order?
#ifndef __unix__
		void processStream(SOCKET hSocket, bool bBigEndian);
#else
		void processStream(int hSocket, bool bBigEndian);
#endif

		/// Process an input stream
		//
		/// This is an alternative function to the previous processStream function
		/// that allows specifying that the endian should be automatically
		/// detected.
		/// @param hSocket A valid socket to read the data stream from
		/// @param EndianType Specify the endian type of the input stream. Use Endian_Auto
		/// for it to be automatically detected.
#ifndef __unix__
		void processStream(SOCKET hSocket, eEndianType EndianType);
#else
		void processStream(int hSocket, eEndianType EndianType);
#endif

	protected:
		/// Perform a blocking read of the desired length and throw eof exception
		/// @param pData Destination for read data
		/// @param iLength Number of bytes to read
		virtual void blockingRead(char *pData, int iLength) override;

	private:
#ifndef __unix__
		SOCKET m_hSocket; ///< Handle to the system socket
#else //unix
		int m_hSocket; ///< Handle to the system socket
#endif
	};
}

#endif
