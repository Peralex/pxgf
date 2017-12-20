#ifndef CCHUNKHANDLERSSR__H
#define CCHUNKHANDLERSSR__H
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

#include <vector>

#include "../ChunkHandler.h"
#include "../PXGStreamWriter.h"

namespace pxgf
{
	/// Handler which deals with the Single channel Short Real chunk
	//
	/// This chunk handles a data stream containing real samples only.
	/// useful for ADC data or audio data.
	class cChunkHandlerSSR_ : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallbackSSR_
		{
		public:
			/// Called on the callback handler whenever a SSR_
			/// chunk is processed.
			/// @param lTimestamp_us Timestamp of first sample in frame.
			/// @param vsRealData Vector of 16 bit samples.
			virtual void callbackSSR_(int64_t lTimestamp_us, const std::vector<short> &vsRealData) = 0;
		};

		/// Constructor
		cChunkHandlerSSR_();

		/// Destructor
		virtual ~cChunkHandlerSSR_(){};

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler The callback handler object whose function will be called to handle processed SSR_ chunks.
		void registerCallbackHandler(cCallbackSSR_ *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian Is data big-endian?
		/// @return true if the chunk was processed successfully.
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a SSR_ chunk to the given stream writer
		/// @param stream Stream to write to.
		/// @param lTimestamp_us Timestamp of first sample in frame.
		/// @param vsRealData Vector of 16 bit samples.
		void writeChunkSSR_(cPXGWriterBase &stream, int64_t lTimestamp_us, const std::vector<short> &vsRealData) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"SSR_"
			return 0x5353525F;
		}

	private:
		cCallbackSSR_ *m_pCallbackSSR_Handler; ///< Pointer to object that implements the callback handler interface.

		std::vector<short> m_vsIqData; ///< Vector used to change endianness if required, and change from char to short.
	};
}

#endif
