#ifndef CHUNKHANDLEREOFH_H
#define CHUNKHANDLEREOFH_H
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

#include "../ChunkHandler.h"
#include "../PXGStreamWriter.h"

namespace pxgf
{
	/// Chunk handler to handle Start Of File Header chunks.
	class cChunkHandlerEOFH : public cChunkHandler
	{
	public:
		/// Callback interface for EOFH chunk handler.
		class cCallbackEOFH
		{
		public:
			/// Called on the callback handler whenever an EOFH chunk is processed.
			virtual void callbackEOFH() = 0;
		};

		/// Creates a new instance of cChunkHandlerEOFH
		cChunkHandlerEOFH();

		/// Register the object which will provide an implementation of this handler's callback function.
		/// @param pHandler Pointer to the callback handler that is to be used.
		void registerCallbackHandler(cCallbackEOFH *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian True if the data is in big-endian order.
		/// @return true if the chunk was processed successfully or not processed because it has no registered handler.
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a EOFH chunk to the given stream writer
		/// @param stream Stream derived from cPXGWriterBase.
		void writeChunkEOFH(cPXGWriterBase &stream) const;

		/// Get the 32 bit unique number for this chunk type.
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"EOFH"
			return 0x454F4648;
		}

	private:
		cCallbackEOFH *m_pCallbackEOFHHandler; ///< Object which implements the callback handler interface.
	};
}

#endif
