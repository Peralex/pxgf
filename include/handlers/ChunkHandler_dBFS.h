#ifndef CCHUNKHANDLER_DBFS_H
#define CCHUNKHANDLER_DBFS_H
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
	/// Handler which deals with the Full Scale dBm chunk
	class cChunkHandler_dBFS : public cChunkHandler
	{
	public:
		/// Callback handler interface
		class cCallback_dBFS
		{
		public:
			/// Called on the callback handler whenever a _dBFS
			/// chunk is processed.
			/// @param f_dBFullScale_dBm Level in dBm that a fullscale (i.e. +-SHRT_MAX in the case of SSIQ chunks)
			///								signal represents
			virtual void callback_dBFS(float f_dBFullScale_dBm) = 0;
		};

		/// Constructor
		cChunkHandler_dBFS();

		/// Register the object which will provide an implementation of this handler's
		/// callback function.
		/// @param pHandler Pointer to the callback handler that is to be used.
		void registerCallbackHandler(cCallback_dBFS *pHandler);

		/// To be used by a PXG Reader class to process the relevant chunk.
		/// @param vchData Vector of bytes belonging to the chunk.
		/// @param bBigEndian True if the data is big-endian.
		/// @return true if the chunk was processed successfully or ignored, false otherwise.
		bool processChunk(const std::vector<char> &vchData, bool bBigEndian) override;

		/// Write a _dBFS chunk to the given stream writer
		/// @param stream Stream to write to
		/// @param f_dBFullScale_dBm Full scale value of data in dBm
		void writeChunk_dBFS(cPXGWriterBase &stream, float f_dBFullScale_dBm) const;

		/// Get the 32 bit unique number for this chunk type
		/// @return Unique 32 bit chunk ID for this chunk type.
		int getChunkType() const override
		{
			//"_dBFS"
			return 0x64424653;
		}

	private:
		cCallback_dBFS *m_pCallback_dBFSHandler; ///< Object which implements the callback handler interface.
	};
}

#endif
