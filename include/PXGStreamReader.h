#ifndef PXGSTREAMREADER_H
#define PXGSTREAMREADER_H
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

#include "PXGReaderBase.h"
#include "PXGStreamWriter.h"
#include "ResetHandler.h"

namespace pxgf
{
	/// PXGReader class that reads a PXGF stream from an std::istream.
	//
	/// This class is used for parsing PXGF streams. The user must register a
	/// reset handler and multiple chunk handlers. The reset handler is used to
	/// clear the state of the application when synchronisation is lost. The chunk
	/// handlers are used to process recognised chunks when they are recognised
	/// in the stream.
	///
	/// Once you have opened an input stream and determined the endian of the
	/// stream you will call the processStream() function. This function will not
	/// return until either you specify somewhere else that you wish to stop
	/// processing by calling removeFromStream(), or it reaches the end of the file.
	class cPXGStreamReader : public cPXGReaderBase
	{
	public:
		/// Constructor
		cPXGStreamReader();

		/// Constructor
		/// @param stream The stream to read data from.
		cPXGStreamReader(std::istream &stream);

		/// Process an input stream
		//
		/// Calling this stream instructs the engine to start processing the given
		/// stream from the specfied input stream and assuming the endian
		/// ordering specified. It will continuously process chunks until it
		/// either reaches the end-of-file, or you specify that you wish it to
		/// stop by calling removeFromStream(). (This processStream is deprecated
		/// and the other one which specfies endian by the eEndianType enum should
		/// be used instead).
		/// @param stream A valid istream to read the data stream from
		/// @param bBigEndian Is the stream big-endian byte order?
		void processStream(std::istream &stream, bool bBigEndian);

		/// Process an input stream
		//
		/// Calling this stream instructs the engine to start processing the given
		/// stream from the specfied stream and assuming the endian
		/// ordering specified. It will continuously process chunks until it
		/// either reaches the end-of-file, or you specify that you wish it to
		/// stop by calling removeFromStream().
		/// This differs from the other processStream function in that allows one to
		/// specify that the endian should be automatically detected.
		/// @param stream A valid istream to read the data stream from
		/// @param EndianType Specify the endian type of the input stream. Use Endian_Auto
		/// for it to be automatically detected.
		void processStream(std::istream &stream, eEndianType EndianType = Endian_Auto);

	protected:
		/// Perform a blocking read of the desired length and throw eof exception
		/// @param pData Destination for read data
		/// @param iLength Number of bytes to read
		virtual void blockingRead(char *pData, int iLength) override;

	private:
		std::istream *m_pStream; ///< This is the input stream used for reading data.
	};
}

#endif
