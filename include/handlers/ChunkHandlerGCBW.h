#ifndef CHUNKHANDLERGCBW_H
#define CHUNKHANDLERGCBW_H
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
	/// Handler which deals with the Group channel Bandwidth chunk
	//
	/// This chunk is used to describe the bandwidth of each channel in a GSIQ stream
	class cChunkHandlerGCBW : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackGCBW
		{
		public:
			/// Called on the callback handler whenever a GCBW
			/// chunk is processed.
			/// @param lChannelBandwidth_uHz Bandwidth per channel in microhertz.
			virtual void callbackGCBW(int64_t lChannelBandwidth_uHz) = 0;
		};

		/// Constructor
		cChunkHandlerGCBW();

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler The callback handler to be invoked to handle the processed data.
		void registerCallbackHandler(cCallbackGCBW *pHandler);

		/// Perform the appropriate processing required for this chunk.
		//
		/// If any problems are detected during parsing of the data then the
		/// return value should be false.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian Is data big-endian?
		/// @return true if the chunk was processed successfully
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a GCBW chunk to the given stream writer
		/// @param stream Stream to write to
		/// @param lChannelBandwidth_uHz Bandwidth of each channel in uHz
		void writeChunkGCBW(cPXGWriterBase &stream, int64_t lChannelBandwidth_uHz) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"GCBW"
			return 0x47434257;
		}

	private:
		cCallbackGCBW *m_pCallbackGCBWHandler; ///< Object which implements the callback handler interface.
	};
}

#endif
