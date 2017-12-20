#ifndef CCHUNKHANDLERCF___H
#define CCHUNKHANDLERCF___H
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
	/// Handler which deals with the Centre Frequency chunk
	class cChunkHandlerCF__ : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackCF__
		{
		public:
			/// Called on the callback handler whenever a CF__
			/// chunk is processed.
			/// @param lCentreFrequency_uHz Centre frequency of receiver in uHz
			virtual void callbackCF__(int64_t lCentreFrequency_uHz) = 0;
		};

		/// Constructor
		cChunkHandlerCF__();

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler Pointer to the callback handler that is to be used
		void registerCallbackHandler(cCallbackCF__ *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian True if the data should be stored in big-endian format.
		/// @return true if the chunk was processed successfully.
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a CF__ chunk to the given stream writer
		/// @param stream Stream to write to
		/// @param lCentreFrequency_uHz Centre Frequency of receiver in uHz
		void writeChunkCF__(cPXGWriterBase &stream, int64_t lCentreFrequency_uHz) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"CF__"
			return 0x43465f5f;
		}

	private:
		cCallbackCF__ *m_pCallbackCF__Handler; ///< Object which implements the callback handler interface.
	};
}

#endif
