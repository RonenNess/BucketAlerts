#include "TokenBucket.h"
#include "Defs.h"

namespace BucketAlerts
{
	TokenBucket::TokenBucket(double starting, double max, double replenish_rate) : 
		_starting_count(starting), _tokens(starting), _max_tokens(max), _replenish_rate(replenish_rate), _total_consumption(0)
	{
		_last_update_time = AccurateClock::Now();
	}

	TokenBucket::TokenBucket(const TokenBucket& other) :
		_starting_count(other._starting_count), _tokens(other._tokens), _max_tokens(other._max_tokens), _replenish_rate(other._replenish_rate), _total_consumption(other._total_consumption)
	{
		_last_update_time = AccurateClock::Now();
	}

	const TokenBucket& TokenBucket::operator=(const TokenBucket& other)
	{
		_starting_count = other._starting_count;
		_tokens = other._tokens;
		_max_tokens = other._max_tokens;
		_replenish_rate = other._replenish_rate;
		_last_update_time = other._last_update_time;
		_total_consumption = other._total_consumption;
		OnBucketExhausted = other.OnBucketExhausted;
		return *this;
	}

	void TokenBucket::Update()
	{
		// calculate time diff in seconds
		auto curr_update_time = AccurateClock::Now();
		double dt = AccurateClock::DiffSeconds(_last_update_time, curr_update_time);

		// no time passed? nothing to do
		if (dt == 0)
			return;

		// update last update time
		_last_update_time = curr_update_time;

		// lock mutex
		if (Defs::ThreadSafe) _mtx.lock();

		// add tokens
		_tokens += dt * _replenish_rate;

		// limit to max
		if (_tokens > _max_tokens)
		{
			_tokens = _max_tokens;
		}

		// unlock mutex
		if (Defs::ThreadSafe) _mtx.unlock();
	}

	void TokenBucket::Restore(double amount)
	{
		// lock if needed
		if (Defs::ThreadSafe) _mtx.lock();

		// add tokens and make sure didn't pass max
		_tokens += amount;
		if (_tokens > _max_tokens)
			_tokens = _max_tokens;

		// unlock
		if (Defs::ThreadSafe) _mtx.unlock();
	}

	bool TokenBucket::Consume(double amount)
	{
		// update tokens before consuming
		if (BucketAlerts::Defs::AutoUpdate) 
			Update();

		// lock mutex
		if (Defs::ThreadSafe) _mtx.lock();

		// if got enough to consume reduce tokens and return true
		if (_tokens >= amount)
		{
			// decrease tokens
			_tokens -= amount;
			_total_consumption += amount;

			// unlock and return
			if (Defs::ThreadSafe) _mtx.unlock();
			return true;
		}
		// if don't have enough zero tokens and return false
		else
		{
			// zero tokens and return false
			_total_consumption += _tokens;
			_tokens = 0;

			// release the lock before calling the callback
			if (Defs::ThreadSafe) _mtx.unlock();

			// invoke callback
			if (OnBucketExhausted)
			{
				OnBucketExhausted(*this);
			}

			// return false
			return false;
		}
	}

	double TokenBucket::Count() 
	{ 
		// update tokens
		if (BucketAlerts::Defs::AutoUpdate) 
			Update(); 

		// return current balance
		return _tokens; 
	}

	void TokenBucket::Reset() 
	{ 
		if (Defs::ThreadSafe) _mtx.lock();
		_tokens = _starting_count;
		if (Defs::ThreadSafe) _mtx.unlock();
	}


	bool TokenBucket::Test(double amount) const
	{
		return _tokens >= amount;
	}
}