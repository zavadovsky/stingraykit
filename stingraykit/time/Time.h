#ifndef STINGRAYKIT_TIME_TIME_H
#define STINGRAYKIT_TIME_TIME_H

// Copyright (c) 2011 - 2025, GS Group, https://github.com/GSGroup
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <stingraykit/serialization/ISerializable.h>
#include <stingraykit/time/BrokenDownTime.h>
#include <stingraykit/optional.h>

#include <limits>

namespace stingray
{

	/**
	 * @addtogroup toolkit_time
	 * @{
	 */

	class TimeDuration
	{
	private:
		s64		_microseconds;

	private:
		constexpr TimeDuration(s64 microseconds, Dummy dummy)
			:	_microseconds(microseconds)
		{ }

	public:
		constexpr explicit TimeDuration(s64 milliseconds = 0)
			:	_microseconds(1000 * milliseconds)
		{ }

		constexpr s64 GetMicroseconds() const								{ return _microseconds; }
		constexpr s64 GetMilliseconds() const								{ return GetMicroseconds() / 1000; }
		constexpr s64 GetSeconds() const									{ return GetMilliseconds() / 1000; }
		constexpr s64 GetMinutes() const									{ return GetSeconds() / 60; }
		constexpr s64 GetHours() const										{ return GetMinutes() / 60; }
		constexpr s64 GetDays() const										{ return GetHours() / 24; }

		TimeDuration Absolute() const;
		TimeDuration RoundToMilliseconds() const;

		void Serialize(ObjectOStream& ar) const;
		void Deserialize(ObjectIStream& ar);

		constexpr TimeDuration& operator += (TimeDuration other)			{ _microseconds += other._microseconds; return *this; }
		constexpr TimeDuration operator + (TimeDuration other) const		{ TimeDuration result(*this); return result += other; }

		constexpr TimeDuration& operator -= (TimeDuration other)			{ _microseconds -= other._microseconds; return *this; }
		constexpr TimeDuration operator - (TimeDuration other) const		{ TimeDuration result(*this); return result -= other; }
		constexpr TimeDuration operator - () const							{ return TimeDuration(-_microseconds, Dummy()); }

		constexpr TimeDuration& operator *= (int multiplier)				{ _microseconds *= multiplier; return *this; }
		constexpr TimeDuration operator * (int multiplier) const			{ TimeDuration result(*this); return result *= multiplier; }

		constexpr TimeDuration& operator /= (int divisor)					{ _microseconds /= divisor; return *this; }
		constexpr TimeDuration operator / (int divisor) const				{ TimeDuration result(*this); return result /= divisor; }

		constexpr s64 operator / (TimeDuration divisor) const				{ return _microseconds / divisor._microseconds; }

		constexpr TimeDuration& operator %= (TimeDuration divisor)			{ _microseconds %= divisor._microseconds; return *this; }
		constexpr TimeDuration operator % (TimeDuration divisor) const		{ TimeDuration result(*this); return result %= divisor; }

		constexpr bool operator < (TimeDuration other) const				{ return _microseconds < other._microseconds; }
		STINGRAYKIT_GENERATE_CONSTEXPR_COMPARISON_OPERATORS_FROM_LESS(TimeDuration);

		std::string ToString(string_view format = string_view()) const;
		static TimeDuration FromString(string_view str);

		constexpr static TimeDuration FromMicroseconds(s64 microseconds)	{ return TimeDuration(microseconds, Dummy()); }
		constexpr static TimeDuration FromMilliseconds(s64 milliseconds)	{ return TimeDuration(milliseconds); }

		constexpr static TimeDuration Second()								{ return TimeDuration(1000); }
		constexpr static TimeDuration Minute()								{ return Second() * 60; }
		constexpr static TimeDuration Hour()								{ return Minute() * 60; }
		constexpr static TimeDuration Day()									{ return Hour() * 24; }

		constexpr static TimeDuration FromSeconds(int seconds)				{ return Second() * seconds; }
		constexpr static TimeDuration FromMinutes(int minutes)				{ return Minute() * minutes; }
		constexpr static TimeDuration FromHours(int hours)					{ return Hour() * hours; }
		constexpr static TimeDuration FromDays(int days)					{ return Day() * days; }

		static TimeDuration FromBcdDuration(u32 bcdDuration);

		constexpr static TimeDuration Min()									{ return TimeDuration::FromMicroseconds(std::numeric_limits<s64>::min()); }
		constexpr static TimeDuration Max()									{ return TimeDuration::FromMicroseconds(std::numeric_limits<s64>::max()); }
	};


	class TimeZone
	{
	private:
		s16		_minutesFromUtc;

	public:
		explicit TimeZone(s16 minutes = 0);

		s16 GetMinutesFromUtc() const { return _minutesFromUtc; }

		static TimeZone Current();

		bool operator < (const TimeZone& other) const { return _minutesFromUtc < other._minutesFromUtc; }
		STINGRAYKIT_GENERATE_COMPARISON_OPERATORS_FROM_LESS(TimeZone);

		std::string ToString() const;
		static TimeZone FromString(string_view str);

		void Serialize(ObjectOStream& ar) const;
		void Deserialize(ObjectIStream& ar);
	};


	class TimeKind
	{
		STINGRAYKIT_ENUM_VALUES(Utc, Local);
		STINGRAYKIT_DECLARE_ENUM_CLASS(TimeKind);
	};


	class Time
	{
	private:
		s64		_milliseconds;

	public:
		constexpr explicit Time(s64 milliseconds = 0)
			:	_milliseconds(milliseconds)
		{ }

		constexpr static Time FromTimeT(time_t t)
		{ return Time((s64)t * 1000); }

		constexpr u32 ToTimeT() const
		{ return _milliseconds / 1000; }

		constexpr TimeDuration operator - (Time other) const
		{ return TimeDuration(_milliseconds - other._milliseconds); }

		constexpr Time& operator -= (TimeDuration duration)
		{
			_milliseconds -= duration.GetMilliseconds();
			return *this;
		}

		constexpr Time& operator += (TimeDuration duration)
		{
			_milliseconds += duration.GetMilliseconds();
			return *this;
		}

		constexpr Time operator - (TimeDuration duration) const
		{
			Time result(*this);
			return result -= duration;
		}

		constexpr Time operator + (TimeDuration duration) const
		{
			Time result(*this);
			return result += duration;
		}

		constexpr bool operator < (Time other) const
		{ return _milliseconds < other._milliseconds; }
		STINGRAYKIT_GENERATE_CONSTEXPR_COMPARISON_OPERATORS_FROM_LESS(Time);

		static Time Now();

		constexpr s64 GetMilliseconds() const		{ return _milliseconds; }
		constexpr s64 GetSeconds() const			{ return GetMilliseconds() / 1000; }

		BrokenDownTime BreakDown(TimeKind kind = TimeKind::Local) const;
		static Time FromBrokenDownTime(const BrokenDownTime& bdt, TimeKind kind = TimeKind::Local);

		std::string ToString(string_view format = string_view(), TimeKind kind = TimeKind::Local) const;
		static Time FromString(string_view str, TimeKind kind = TimeKind::Local);

		u64 ToNtpTimestamp() const;
		static Time FromNtpTimestamp(u64 timestamp);

		static Time FromWindowsFileTime(u64 windowsTicks);

		static Time MjdToEpoch(int mjd, u32 bcdDuration = 0);

		int GetMjd() const;
		u32 GetBcdTime(TimeKind kind = TimeKind::Local) const;

		int DaysTo(Time endTime) const;
		int DaysTo(const BrokenDownTime& endTime) const;

		void Serialize(ObjectOStream& ar) const;
		void Deserialize(ObjectIStream& ar);
	};


	class TimeCmp : public comparers::CmpComparerInfo
	{
	private:
		TimeDuration			_allowedJitter;

	public:
		TimeCmp(TimeDuration allowedJitter = TimeDuration()) : _allowedJitter(allowedJitter) { }

		int operator () (Time lhs, Time rhs) const;
	};
	STINGRAYKIT_DECLARE_COMPARERS(Time);


	class TimeUtility
	{
		class FromIso8601Impl;

	public:
		static std::string ToIso8601(Time time);
		static Time FromIso8601(string_view str);
	};


	class TimeDurationUtility
	{
		class FromIso8601Impl;

	public:
		static std::string ToIso8601(TimeDuration timeDuration, const optional<Time>& base = null);
		static TimeDuration FromIso8601(string_view str, Time base = Time());
	};

	/** @} */

}


#endif
