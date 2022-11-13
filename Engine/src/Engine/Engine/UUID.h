#pragma once
#include <cstdint>

namespace Polyboid {

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }

		bool operator==(const UUID& lhs) const
		{
			return m_UUID == lhs.m_UUID;
		}

		bool operator!=(const UUID& lhs) const
		{
			return !(m_UUID == lhs.m_UUID);
		}

		bool operator==(const UUID& lhs)
		{
			return m_UUID == lhs.m_UUID;
		}

		bool operator!=(const UUID& lhs)
		{
			return !(m_UUID == lhs.m_UUID);
		}


	private:
		uint64_t m_UUID;
	};

}

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<Polyboid::UUID>
	{
		std::size_t operator()(const Polyboid::UUID& uuid) const noexcept
		{
			return uuid;
		}
	};

}