#ifndef PACKINGGIQP_H
#define PACKINGGIQP_H
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
	/// Class for storing packing information about data sent in
	/// Group Short IQ time data - GSIQ chunks
	class cPackingGIQP
	{
	public:
		/// Constructor
		/// @param bIsIQPacked Are the samples IQ packed? Otherwise they are QI packed
		cPackingGIQP(bool bIsIQPacked = true);

		/// Constructor
		/// @param bIsIQPacked Are the samples IQ packed? Otherwise they are QI packed
		/// @param iIncrement The number of samples to skip in the data block to advance to the
		///						next sample for a particular channel
		/// @param viChannelOffsets The offset (in samples) to the first sample for each channel
		cPackingGIQP(bool bIsIQPacked, int iIncrement, const std::vector<int> &viChannelOffsets);

		/// Sets the cPackingGIQP members
		/// @param bIsIQPacked Are the samples IQ packed? Otherwise they are QI packed
		/// @param iIncrement The number of samples to skip in the data block to advance to the
		///						next sample for a particular channel
		/// @param viChannelOffsets The offset (in samples) to the first sample for each channel
		void set(bool bIsIQPacked, int iIncrement, const std::vector<int> &viChannelOffsets);

		/// Test for equality with another object
		/// @param oOther Object to test against
		/// @returns true if equal
		bool equals(const cPackingGIQP &oOther) const;

		/// Get the number of channels in the group
		int getNumChannels() const;

		/// Test if the packing is IQ?
		bool getIsIQPacked() const;

		/// Get the number of samples to advance in the GSIQ data block in order to retrieve the
		/// next sample for a particluar channel
		/// @return The number of samples.
		int getIncrement() const;

		/// Get the offsets to the first sample of each channel
		/// @return A const reference to a vector of channel offsets (measured in samples).
		const std::vector<int> &getChannelOffsets() const;

	private:
		bool m_bIsIQPacked;
		int m_iIncrement;
		std::vector<int> m_viChannelOffsets; ///< Channel offsets (measured in samples) for each channel.
	};
}

#endif
