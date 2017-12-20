#ifndef CHUNKHANDLER_GRG__H
#define CHUNKHANDLER_GRG__H
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
	/// Handler which deals with the Group Relative Gains chunk
	class cChunkHandlerGRG_ : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackGRG_
		{
		public:
			/// Called on the callback handler whenever a GRG_
			/// chunk is processed.
			/// @param vfGains_dB Total system gain in dB.
			virtual void callbackGRG_(const std::vector<float> &vfGains_dB) = 0;
		};

		/// Constructor
		cChunkHandlerGRG_();

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler Pointer to the callback handler that is to be used
		void registerCallbackHandler(cCallbackGRG_ *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Array of data belonging to the chunk.
		/// @param bBigEndian Is the stream big-endian?
		/// @return true if the chunk was processed successfully.
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a GRG_ chunk to the given stream writer
		/// @param stream Stream to write to.
		/// @param vfGains_dB Vector of total system gains per channel in dB.
		void writeChunkGRG_(cPXGWriterBase &stream, std::vector<float> vfGains_dB) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"GRG_"
			return 0x4752475f;
		}

	private:
		cCallbackGRG_ *m_pCallbackGRG_Handler; ///< Object which implements the callback handler interface.
	};
}

#endif
