#ifndef PACKINGSIQP_H
#define PACKINGSIQP_H
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

namespace pxgf
{
	/// Class used to decribe the packing of IQ data. This information is needed
	/// to parse SSIQ data chunks.
	class cPackingSIQP
	{
	public:
		/// Constructor
		/// @param bIsIQPacked Is SSIQ data IQ packed? (Otherwise it is QI packed)
		cPackingSIQP(bool bIsIQPacked = true);

		/// Test for equality with another object
		bool equals(const cPackingSIQP &oOther) const;

		/// Test if the packing is IQ?
		bool getIsIQPacked() const;

	private:
		bool m_bIsIQPacked; ///< true if the data is IQIQIQ..., false if it's QIQIQI...
	};
}

#endif
