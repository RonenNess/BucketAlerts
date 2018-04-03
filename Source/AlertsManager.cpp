#include "AlertsManager.h"
#include "TokenBucket.h"
#include "Clock.h"
#include "Defs.h"


namespace BucketAlerts
{

	AlertsManager::AlertsManager()
	{
	}

	AlertsManager::~AlertsManager()
	{
	}

	void AlertsManager::CreateBucket(CategoryId cat_id, BucketId bucket_id, const TokenBucket& bucket)
	{
		// lock mutex
		if (Defs::ThreadSafe) _mtx.lock();

		// create bucket in category
		_buckets[cat_id][bucket_id] = bucket;

		// unlock mutex
		if (Defs::ThreadSafe) _mtx.unlock();
	}

	void AlertsManager::CreateBucket(CategoryId cat_id, BucketId bucket_id, double starting_tokens, double max_tokens, double replenish_rate, BucketCallback callback)
	{
		TokenBucket bucket(starting_tokens, max_tokens, replenish_rate);
		bucket.OnBucketExhausted = callback;
		CreateBucket(cat_id, bucket_id, bucket);
	}

	void AlertsManager::CreateBucket(BucketId bucket_id, const TokenBucket& bucket)
	{
		CreateBucket(Defs::DefaultCategoryId, bucket_id, bucket);
	}

	void AlertsManager::CreateBucket(BucketId bucket_id, double starting_tokens, double max_tokens, double replenish_rate, BucketCallback callback)
	{
		CreateBucket(Defs::DefaultCategoryId, bucket_id, starting_tokens, max_tokens, replenish_rate, callback);
	}

	void AlertsManager::Clear()
	{
		// lock mutex
		if (Defs::ThreadSafe) _mtx.lock();

		// clear buckets
		_buckets.clear();

		// unlock mutex
		if (Defs::ThreadSafe) _mtx.unlock();
	}

	TokenBucket& AlertsManager::GetBucket(CategoryId cat_id, BucketId bucket_id)
	{
		return _buckets[cat_id][bucket_id];
	}

	TokenBucket& AlertsManager::GetBucket(BucketId bucket_id)
	{
		return GetBucket(Defs::DefaultCategoryId, bucket_id);
	}

	bool AlertsManager::Consume(CategoryId cat_id, BucketId bucket_id, double amount)
	{
		// skip if disabled
		if (!Enabled) 
			return true;

		// get bucket
		TokenBucket& bucket = GetBucket(cat_id, bucket_id);

		// consume amount and get if exhausted
		bool ret = bucket.Consume(amount);

		// if exhausted and need to reset, reset bucket
		if (!ret && Defs::ResetWhenConsumed)
			GetBucket(cat_id, bucket_id).Reset();

		// return result
		return ret;
	}

	bool AlertsManager::Consume(BucketId bucket_id, double amount)
	{
		return Consume(Defs::DefaultCategoryId, bucket_id, amount);
	}

	void AlertsManager::Restore(CategoryId cat_id, BucketId bucket_id, double amount)
	{
		GetBucket(cat_id, bucket_id).Restore(amount);
	}

	void AlertsManager::Restore(BucketId bucket_id, double amount)
	{
		Restore(Defs::DefaultCategoryId, bucket_id, amount);
	}

	void AlertsManager::ManualUpdate()
	{
		_mtx.lock();
		for (auto cat_it = _buckets.begin(); cat_it != _buckets.end(); ++cat_it)
		{
			for (auto bucket = cat_it->second.begin(); bucket != cat_it->second.end(); ++bucket)
			{
				bucket->second.Update();
			}
		}
		_mtx.unlock();
	}

	void AlertsManager::ResetAll()
	{
		_mtx.lock();
		for (auto cat_it = _buckets.begin(); cat_it != _buckets.end(); ++cat_it)
		{
			for (auto bucket = cat_it->second.begin(); bucket != cat_it->second.end(); ++bucket)
			{
				bucket->second.Reset();
			}
		}
		_mtx.unlock();
	}

	AlertsManager& get_main()
	{
		static AlertsManager ret;
		return ret;
	}

}