/*!
 * \file	Source\AlertsManager.h.
 *
 * \brief	Declares the alerts manager class.
 */
#pragma once
#include "TokenBucket.h"
#include "Defs.h"
#include <unordered_map>


namespace BucketAlerts
{
	/*!
	 * \class	AlertsManager
	 *
	 * \brief	The main class that manage counters and alerts.
	 * 			You can create your own instance or just use the global static object
	 * 			by calling 'get_main()'.
	 *
	 * \author	Ronen Ness
	 * \date	3/31/2018
	 */
	class AlertsManager
	{
	private:

		// all the buckets
		std::unordered_map<CategoryId, std::unordered_map<BucketId, TokenBucket> > _buckets;

	public:

		/*! \brief	Enable / disable the alerts manager and consumption counting. */
		bool Enabled = true;

		/*!
		 * \fn	AlertsManager::AlertsManager();
		 *
		 * \brief	Default constructor.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 */
		AlertsManager();

		/*!
		 * \fn	virtual AlertsManager::~AlertsManager();
		 *
		 * \brief	Destructor.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 */
		virtual ~AlertsManager();

		/*!
		 * \fn	void AlertsManager::CreateBucket(CategoryId cat_id, BucketId bucket_id);
		 *
		 * \brief	Creates a new bucket.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 *
		 * \param	cat_id   	Identifier for the category.
		 * \param	bucket_id	Identifier for the bucket.
		 * \param	bucket		Bucket to create from.
		 */
		void CreateBucket(CategoryId cat_id, BucketId bucket_id, const TokenBucket& bucket);

		/*!
		 * \fn	void AlertsManager::CreateBucket(CategoryId cat_id, BucketId bucket_id, double starting_tokens, double max_tokens, double replenish_rate);
		 *
		 * \brief	Creates a new bucket.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 *
		 * \param	cat_id		   	Identifier for the category.
		 * \param	bucket_id	   	Identifier for the bucket.
		 * \param	starting_tokens	Bucket starting tokens count.
		 * \param	max_tokens	   	Bucket max tokens.
		 * \param	replenish_rate 	Bucket replenish rate.
		 * \param	callback		Callback to trigger when bucket exhausted.
		 */
		void CreateBucket(CategoryId cat_id, BucketId bucket_id, double starting_tokens, double max_tokens, double replenish_rate, BucketCallback callback);

		/*!
		* \fn	void AlertsManager::CreateBucket(CategoryId cat_id, BucketId bucket_id);
		*
		* \brief	Creates a new bucket.
		*
		* \author	Ronen Ness
		* \date	3/31/2018
		*
		* \param	bucket_id	Identifier for the bucket.
		* \param	bucket		Bucket to create from.
		*/
		inline void CreateBucket(BucketId bucket_id, const TokenBucket& bucket);

		/*!
		* \fn	void AlertsManager::CreateBucket(CategoryId cat_id, BucketId bucket_id, double starting_tokens, double max_tokens, double replenish_rate);
		*
		* \brief	Creates a new bucket.
		*
		* \author	Ronen Ness
		* \date	3/31/2018
		*
		* \param	bucket_id	   	Identifier for the bucket.
		* \param	starting_tokens	Bucket starting tokens count.
		* \param	max_tokens	   	Bucket max tokens.
		* \param	replenish_rate 	Bucket replenish rate.
		* \param	callback		Callback to trigger when bucket exhausted.
		*/
		void CreateBucket(BucketId bucket_id, double starting_tokens, double max_tokens, double replenish_rate, BucketCallback callback);

		/*!
		 * \fn	TokenBucket& AlertsManager::GetBucket(CategoryId cat_id, BucketId bucket_id);
		 *
		 * \brief	Gets a bucket reference.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 *
		 * \param	cat_id   	Identifier for the category.
		 * \param	bucket_id	Identifier for the bucket.
		 *
		 * \return	The bucket.
		 */
		TokenBucket& GetBucket(CategoryId cat_id, BucketId bucket_id);

		/*!
		 * \fn	TokenBucket& AlertsManager::GetBucket(BucketId bucket_id);
		 *
		 * \brief	Gets a bucket from the default category.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 *
		 * \param	bucket_id	Identifier for the bucket.
		 *
		 * \return	The bucket.
		 */
		TokenBucket& GetBucket(BucketId bucket_id);

		/*!
		 * \fn	bool AlertsManager::Consume(CategoryId cat_id, BucketId bucket_id, double amount = 1.0);
		 *
		 * \brief	Consumes from bucket, and return false if was exhausted.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 *
		 * \param	cat_id   	Identifier for the category.
		 * \param	bucket_id	Identifier for the bucket.
		 * \param	amount   	(Optional) The amount to consume.
		 *
		 * \return	True if bucket is not empty, false if consumed.
		 */
		bool Consume(CategoryId cat_id, BucketId bucket_id, double amount = 1.0);

		/*!
		 * \fn	bool AlertsManager::Consume(BucketId bucket_id, double amount = 1.0);
		 *
		 * \brief	Consumes from bucket, and return false if was exhausted.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 *
		 * \param	bucket_id	Identifier for the bucket in default category.
		 * \param	amount   	(Optional) The amount to consume.
		 *
		 * \return	True if bucket is not empty, false if consumed.
		 */
		bool Consume(BucketId bucket_id, double amount = 1.0);

		/*!
		* \fn	void AlertsManager::Restore(CategoryId cat_id, BucketId bucket_id, double amount = 1.0);
		*
		* \brief	Restore tokens to bucket.
		*
		* \author	Ronen Ness
		* \date	3/31/2018
		*
		* \param	cat_id   	Identifier for the category.
		* \param	bucket_id	Identifier for the bucket.
		* \param	amount   	(Optional) The amount to restore.
		*/
		void Restore(CategoryId cat_id, BucketId bucket_id, double amount = 1.0);

		/*!
		* \fn	void AlertsManager::Restore(BucketId bucket_id, double amount = 1.0);
		*
		* \brief	Restore tokens to bucket.
		*
		* \author	Ronen Ness
		* \date	3/31/2018
		*
		* \param	bucket_id	Identifier for the bucket in default category.
		* \param	amount   	(Optional) The amount to restore.
		*/
		void Restore(BucketId bucket_id, double amount = 1.0);

		/*!
		 * \fn	void AlertsManager::ResetAll();
		 *
		 * \brief	Resets all buckets.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 */
		void ResetAll();

		/*!
		 * \fn	void AlertsManager::ManualUpdate();
		 *
		 * \brief	Normally, buckets will update only when you try to consume them. This function will
		 * 			force-update all buckets. Use this if you disable auto-update mode.
		 *
		 * \author	Ronen Ness
		 * \date	4/3/2018
		 */
		void ManualUpdate();

		/*!
		 * \fn	void AlertsManager::Clear();
		 *
		 * \brief	Clears this object to its blank/initial state.
		 *
		 * \author	Ronen Ness
		 * \date	3/31/2018
		 */
		inline void Clear();
	};

	/*!
	 * \fn	AlertsManager& main();
	 *
	 * \brief	Gets the default static alerts manager.
	 *
	 * \author	Ronen Ness
	 * \date	3/31/2018
	 *
	 * \return	A reference to the default static AlertsManager.
	 */
	AlertsManager& get_main();
}