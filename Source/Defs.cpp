#include "Defs.h"

namespace BucketAlerts
{
	bool			Defs::ThreadSafe			= true;
	CategoryId		Defs::DefaultCategoryId		= (CategoryId)-1;
	bool			Defs::ResetWhenConsumed		= false;
	bool			Defs::AutoUpdate			= true;
}