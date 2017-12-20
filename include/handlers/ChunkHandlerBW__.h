#ifndef CCHUNKHANDLERBW___H
#define CCHUNKHANDLERBW___H
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
	/// Handler which deals with the Bandwidth chunk
	class cChunkHandlerBW__ : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackBW__
		{
		public:
			/// Called on the callback handler whenever a BW__
			/// chunk is processed.
			/// @param lBandwidth_uHz Bandwidth in uHz
			virtual void callbackBW__(int64_t lBandwidth_uHz) = 0;
		};

		/// Constructor
		cChunkHandlerBW__();

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler Pointer to the callback handler that is to be used
		void registerCallbackHandler(cCallbackBW__ *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian Is the stream big-endian?
		/// @return true if the chunk was processed successfully, false otherwise.
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a BW__ chunk to the given stream writer
		/// @param stream Stream to write to
		/// @param lBandwidth_uHz Bandwidth in uHz
		void writeChunkBW__(cPXGWriterBase &stream, int64_t lBandwidth_uHz) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"BW__"
			return 0x42575f5f;
		}

	private:
		cCallbackBW__ *m_pCallbackBW__Handler; ///< Object which implements the callback handler interface.
	};
}

#endif
