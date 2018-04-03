/*!
 * \file	Source\Defs.h.
 *
 * \brief	Declares the defs class.
 */
#pragma once

namespace BucketAlerts
{
	/*!
	* \typedef	unsigned int CategoryId
	*
	* \brief	Defines an alias representing a category id.
	*/
	typedef unsigned int CategoryId;

	/*!
	* \typedef	unsigned int BucketId
	*
	* \brief	Defines an alias representing a bucket id inside a category.
	*/
	typedef unsigned int BucketId;

	/*!
	 * \class	Defs
	 *
	 * \brief	Misc defs and behavior flags.
	 *
	 * \author	Ronen Ness
	 * \date	3/31/2018
	 */
	class Defs
	{
	public:

		/*! \brief	True to be thread safe (at the cost of having mutex locks). */
		static bool ThreadSafe;

		/*! \brief	The default category identifier for when not specifying category. */
		static CategoryId DefaultCategoryId;

		/*! \brief	If true, whenever we consume a bucket and call the event the bucket will be reset automatically. */
		static bool ResetWhenConsumed;

		/*! \brief	If true will automatically update tokens when you try to consume from bucket. If false, you need to call Update() yourself. */
		static bool AutoUpdate;
	};
}