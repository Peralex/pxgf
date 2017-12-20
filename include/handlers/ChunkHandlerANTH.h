#ifndef CHUNKHANDLERANTH_H
#define CHUNKHANDLERANTH_H
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

#include <string>

#include "../ChunkHandler.h"
#include "../PXGStreamWriter.h"
#include "../exceptions/PxgfIllegalArgumentException.h"

namespace pxgf
{
	/// Handler which deals with the Antenna chunk
	//
	/// This chunk is used to add the GAD Antenna name to the PXGF stream
	class cChunkHandlerANTH : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackANTH
		{
		public:
			/// Called on the callback handler whenever a ANTH
			/// chunk is processed.
			/// @param strAntennaName The antenna name.
			virtual void callbackANTH(std::string &strAntennaName) = 0;
		};

		/// Default Constructor
		cChunkHandlerANTH();

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler Pointer to the callback handler that is to be used.
		void registerCallbackHandler(cCallbackANTH *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData The vector of data belonging to the chunk
		/// @param bBigEndian True if the data is to be stored in big-endian format, false otherwise.
		/// @return true if the chunk was processed successfully, false otherwise.
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a ANTH chunk to the given stream writer
		/// @param stream The output stream to write the chunk to.
		/// @param strAntennaName Antenna name written to the stream.
		void WriteChunkANTH(cPXGWriterBase &stream, const std::string &strAntennaName) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			// "ANTH"
			return 0x414E5448;
		}
	private:
		cCallbackANTH *m_pCallbackANTHHandler; ///< Object which implements the callback handler interface.
	};
}

#endif
