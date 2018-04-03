/*!
 * \file	Source\TokenBucket.h.
 *
 * \brief	Declares the token bucket class.
 */
#pragma once
#include <mutex>
#include "Clock.h"


namespace BucketAlerts
{
	// predef
	class TokenBucket;

	/*!
	 * \typedef	void(*onBucketExhausted)(const TokenBucket& bucket)
	 *
	 * \brief	A callback we can attach to a bucket to call when exhausted.
	 */
	typedef void(*BucketCallback)(const TokenBucket& bucket);

	/*!
	 * \class	TokenBucket
	 *
	 * \brief	A token bucket.
	 *
	 * \author	Ronen Ness
	 * \date	3/31/2018
	 */
	class TokenBucket
	{
	private:
		// current tokens count.
		double _tokens;

		// how many new tokens we get per second.
		double _replenish_rate;

		// max tokens allowed in bucket.
		double _max_tokens;

		// starting value.
		double _starting_count;

		// total tokens consumed since created.
		double _total_consumption;

		// last time we had a token update
		AccurateClock::TimePoint _last_update_time;

		// mutex
		std::mutex _mtx;

	public:

		/*! \brief	Optional function to call when bucket runs out of tokens */
		BucketCallback OnBucketExhausted = nullptr;

		/*!
		 * \fn	TokenBucket::TokenBucket(double starting, double max, double replenish_rate);
		 *
		 * \brief	Constructor
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 *
		 * \param	starting	  	Starting tokens count.
		 * \param	max			  	Max tokens allowed in bucket.
		 * \param	replenish_rate	Tokens replenish rate (tokens per second).
		 */
		TokenBucket(double starting=0, double max=10, double replenish_rate=1);

		/*!
		 * \fn	TokenBucket::TokenBucket(const TokenBucket& other);
		 *
		 * \brief	Copy constructor.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 *
		 * \param	other	The other object.
		 */
		TokenBucket(const TokenBucket& other);

		/*!
		 * \fn	const TokenBucket& TokenBucket::operator=(const TokenBucket& other);
		 *
		 * \brief	Assignment operator.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 *
		 * \param	other	The other object.
		 *
		 * \return	A shallow copy of this object.
		 */
		const TokenBucket& operator=(const TokenBucket& other);

		/*!
		 * \fn	bool TokenBucket::Consume(double amount = 1.0) };
		 *
		 * \brief	Consumes the given amount of tokens.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 *
		 * \param	amount	(Optional) The amount to consume.
		 *
		 * \return	True if it got enough tokens to consume, False if hit 0.
		 */
		bool Consume(double amount = 1.0);

		/*!
		* \fn	bool TokenBucket::Restore(double amount = 1.0) };
		*
		* \brief	Restore the given amount of tokens.
		*
		* \author	Ronen Ness
		* \date	3/31/2018
		*
		* \param	amount	(Optional) The amount to restore.
		*/
		void Restore(double amount = 1.0);

		/*!
		 * \fn	bool TokenBucket::Test(double amount = 1.0);
		 *
		 * \brief	Tests if have enough tokens to consume.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 *
		 * \param	amount	(Optional) The amount.
		 *
		 * \return	True if have enough tokens, false if not.
		 */
		inline bool Test(double amount = 1.0) const;

		/*!
		 * \fn	inline double TokenBucket::Count() const
		 *
		 * \brief	Get current tokens count.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 *
		 * \return	Current tokens count.
		 */
		double Count();

		/*!
		 * \fn	double inline TokenBucket::TotalConsumed() const
		 *
		 * \brief	Return how many tokens were consumed in total.
		 *
		 * \author	Ronen Ness
		 * \date	4/3/2018
		 *
		 * \return	The total number of consumed token since the creation of this bucket.
		 */
		double inline TotalConsumed() const { return _total_consumption; }

		/*!
		 * \fn	inline void TokenBucket::Reset()
		 *
		 * \brief	Resets this bucket to its starting value;
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 */
		void Reset();

		/*!
		* \fn	void TokenBucket::Update(double dt);
		*
		* \brief	Updates the tokens (replenish tokens based on time).
		* 			Note: you do not need to call this function manually, unless you disable auto-update.
		*
		* \author	Ronen Ness
		* \date	3/31/2018
		*/
		void Update();
	};

}