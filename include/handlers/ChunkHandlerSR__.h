#ifndef CCHUNKHANDLERSR___H
#define CCHUNKHANDLERSR___H
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
#include "../PXGWriterBase.h"

namespace pxgf
{
	/// Handler which deals with the Sample Rate chunk
	class cChunkHandlerSR__ : public cChunkHandler
	{
	public:
		/// Callback interface for the SR__ chunk handler.
		class cCallbackSR__
		{
		public:
			/// Called on the callback handler whenever a SR__
			/// chunk is processed.
			/// @param lSampleRate_uHz Sample rate of sampled data in uHz
			virtual void callbackSR__(int64_t lSampleRate_uHz) = 0;
		};

		/// Constructor
		cChunkHandlerSR__();

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler The callback handler to be invoked to handle the processed data.
		void registerCallbackHandler(cCallbackSR__ *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian Is data big-endian?
		/// @return true if the chunk was processed successfully
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a SR__ chunk to the given stream writer
		/// @param stream
		/// @param lSampleRate_uHz Sample rate of sampled data in uHz
		void writeChunkSR__(cPXGWriterBase &stream, int64_t lSampleRate_uHz) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"SR__"
			return 0x53525f5f;
		}

	private:
		cCallbackSR__ *m_pCallbackSR__Handler; ///< Object which implements the callback handler interface.
	};
}

#endif
