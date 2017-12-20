#ifndef CHUNKHANDLER_H
#define CHUNKHANDLER_H
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

namespace pxgf
{
	/// Interface to be implemented by every class which is to be used as a chunk handler.
	class cChunkHandler
	{
	public:
		/// Perform the appropriate processing required for this chunk.
		/// @param vchData The vector of data belonging to the chunk.
		/// @param bBigEndian Is the data packed using big-endian?
		/// @return true if the chunk was processed successfully.
		virtual bool processChunk(const std::vector<char> &vchData, bool bBigEndian) = 0;

		/// Get the chunk type identifier that this handler corresponds to.
		/// @return Chunk type ID.
		virtual int getChunkType() const = 0;
	};
}

#endif
