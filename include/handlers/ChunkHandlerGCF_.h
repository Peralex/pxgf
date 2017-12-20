#ifndef CCHUNKHANDLERGCF__H
#define CCHUNKHANDLERGCF__H
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
	/// Handler which deals with the group channel centre frequency chunk
	//
	/// This chunk is used to describe the centre frequency of each channel
	class cChunkHandlerGCF_ : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackGCF_
		{
		public:
			/// Called on the callback handler whenever a GCF_
			/// chunk is processed.
			/// @param alCentreFrequencies_uHz Vector containing the centre frequencies of each channel
			virtual void callbackGCF_(const std::vector<int64_t> &alCentreFrequencies_uHz) = 0;
		};

		/// Constructor
		cChunkHandlerGCF_();

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler The callback handler to be invoked to handle the processed data.
		void registerCallbackHandler(cCallbackGCF_ *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian Is data big-endian?
		/// @return true if the chunk was processed successfully
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a GCF_ chunk to the given stream writer
		/// @param stream Stream to write to
		/// @param vlCentreFrequencies_uHz Vector containing the centre frequencies of each channel
		void writeChunkGCF_(cPXGWriterBase &stream, const std::vector<int64_t> &vlCentreFrequencies_uHz) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"GCF_"
			return 0x4743465F;
		}

	private:
		cCallbackGCF_ *m_pCallbackGCF_Handler; ///< Object which implements the callback handler interface.
	};
}

#endif
