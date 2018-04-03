/*!
 * \file	Source\Clock.h.
 *
 * \brief	Provide an API to get accurate time.
 */

#pragma once
#include <chrono>

namespace BucketAlerts
{
	/*!
	 * \class	Clock
	 *
	 * \brief	A clock.
	 *
	 * \author	Ronen Ness
	 * \date	3/31/2018
	 */
	class AccurateClock
	{
	public:
		typedef std::chrono::high_resolution_clock Clock;
		typedef std::chrono::time_point<Clock> TimePoint;

		/*!
		 * \fn	static TimePoint AccurateClock::Now() noexcept
		 *
		 * \brief	Gets accurate time now.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 *
		 * \return	A TimePoint.
		 */
		static TimePoint Now() noexcept
		{
			return Clock::now();
		}

		/*!
		 * \fn	static double AccurateClock::DiffSeconds(TimePoint prev_t, TimePoint now_t) noexcept
		 *
		 * \brief	Calc difference in seconds between two time points.
		 *
		 * \author	Ronen Ness
		 * \date	4/1/2018
		 *
		 * \param	prev_t	The previous time point.
		 * \param	now_t 	The current time point.
		 *
		 * \return	Seconds between the time points.
		 */
		static double DiffSeconds(const TimePoint& prev_t, const TimePoint& now_t) noexcept
		{
			return now_t == prev_t ? 0.0 : (double)((now_t - prev_t).count()) / 1000000000ULL;
		}
	};
}