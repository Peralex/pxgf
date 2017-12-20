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

#include "handlers/packings/PackingGIQP.h"

namespace pxgf
{
	using namespace std;

	cPackingGIQP::cPackingGIQP(bool bIsIQPacked)
	  : m_iIncrement(1),
		m_viChannelOffsets(vector<int>(1))
	{
		m_bIsIQPacked = bIsIQPacked;
	}

	cPackingGIQP::cPackingGIQP(bool bIsIQPacked, int iIncrement, const vector<int> &viChannelOffsets) :
		m_bIsIQPacked(bIsIQPacked),
		m_iIncrement(iIncrement),
		m_viChannelOffsets(viChannelOffsets)
	{
	}

	void cPackingGIQP::set(bool bIsIQPacked, int iIncrement, const std::vector<int> &viChannelOffsets)
	{
		m_bIsIQPacked = bIsIQPacked;
		m_iIncrement = iIncrement;
		m_viChannelOffsets = viChannelOffsets;
	}

	bool cPackingGIQP::equals(const cPackingGIQP &oOther) const
	{
		return m_bIsIQPacked == oOther.m_bIsIQPacked &&
		 m_iIncrement == oOther.m_iIncrement &&
		 m_viChannelOffsets == oOther.m_viChannelOffsets;
	}

	int cPackingGIQP::getNumChannels() const
	{
		return m_viChannelOffsets.size();
	}

	bool cPackingGIQP::getIsIQPacked() const
	{
		return m_bIsIQPacked;
	}

	int cPackingGIQP::getIncrement() const
	{
		return m_iIncrement;
	}

	const vector<int> &cPackingGIQP::getChannelOffsets() const
	{
		return m_viChannelOffsets;
	}
}
