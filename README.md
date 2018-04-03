# BucketAlerts

Library for Token-Buckets based alerts.

## What Is It

`BucketAlerts` uses Token Buckets to generate events after a specific resource or function was used too much.

If you don't know what it is, a [Token Bucket](https://en.wikipedia.org/wiki/Token_bucket) is an algorithm typically used in networking to create limits on bandwidth while supporting burstiness.

This lib simply implement a basic Token Bucket object + Managers with callbacks, to provide a more logger-like API and allow you to easily define different buckets and register to their events.

### Why

I was working on a scene tree with 3d transformations that had a mechanism to cache transformations matrix internally, so objects won't constantly update when its not necessary. In small 'lab condition' tests it seemed to work perfectly, but as the scenes grew complicated and bigger I began to suspect that the mechanism may be broken.

However, debugging it was tricky. Putting a breakpoint or a log on the update event was useless because in a large scenes it naturally happens a lot and from many different objects. Its also OK for a specific object to update often and many times, just not *too* often. I needed to break on a very special case, when the same object was updating too much over a longer period of time.

That's when I realized I need a token-bucket mechanism that could detect these events. Later I found it to be useful for other cases and projects (like detecting when two objects are "stuck" on each other and constantly colliding) and decided to port it to CPP and make it slightly more generic.

This lib can be used for debugging, logging, or even to respond to real events of resources abuse.

## Install

There's no dll / lib built, so just grab the source and include / build it yourself.

## Usage

Here's a quick usage example:

```cpp
#include "Source/AlertsManager.h"

// test bucket category and name
#define TEST_CATEGORY 5
#define TEST_BUCKET 15

int main()
{
	// create a bucket with category 'TEST_CATEGORY' and id 'TEST_BUCKET'
	BucketAlerts::get_main().CreateBucket(TEST_CATEGORY, TEST_BUCKET, 5, 10, 1, 
		[](const BucketAlerts::TokenBucket& bucket) {
			std::cout << "Bucket Exhausted!" << std::endl;
	});
	
	// consume 1 token
	BucketAlerts::get_main().Consume(TEST_CATEGORY, TEST_BUCKET, 1.0);
}
```

Now lets explain the code above:

To manage the token buckets we use a manager called `Alerts Manager`. Calling `BucketAlerts::get_main()` will return a default static instance of it, but you can also create your own instances if you need to manage different pools of buckets.

In the first line of code we create a new bucket.

`Category Id` is not mandatory (there's a version of CreateBucket without this argument) but useful to group together buckets under a common subject. For example, things like "Memory Allocation", "Update Calls", etc.

`Bucket Id` is what identifies the bucket itself. It should represent the actual cause of the resource consumption or the related object id.

The next 3 numbers define the bucket starting size, max tokens, and how many new tokens it will replenish per second (tokens replenish automatically when you try to consume). 

And the final argument is the callback to trigger when someone tries to consume tokens but don't have enough left in the bucket.

Next and final line in the example shows how to consume 1 token from the bucket. If we ran out of tokens this function will return false and trigger the alert (note however that the tokens will still be consumed).

### Without Category Id

If you don't want to use categories for your buckets, the code above would look like this:

```cpp
// create a bucket with id 'TEST_BUCKET'
BucketAlerts::get_main().CreateBucket(TEST_BUCKET, 5, 10, 1, 
	[](const BucketAlerts::TokenBucket& bucket) {
			std::cout << "Bucket Exhausted!" << std::endl;
});

// consume 1 token
BucketAlerts::get_main().Consume(TEST_BUCKET, 1.0);
```

### Using Custom Managers

As mentioned before, you don't have to use the default `Alerts Manager`. To create your own manager simply instantiate a `AlertsManager` class:

```cpp
BucketAlerts::AlertsManager manager();
```

And use it just like you would use `BucketAlerts::get_main()`.

### AlertsManager API

The following are some other useful functions you should know about `Alerts Manager`:

#### GetBucket()

Return a bucket reference by id and optional category.

#### Restore()

Restore tokens to bucket.

#### ResetAll()

Reset all bucket to their starting value.

#### Clear()

Remove all buckets.

#### ManualUpdate()

Force all buckets to recalculate their remaining tokens based on last time they were accessed. Normally you don't need to call this.

#### Enabled

Set to false to temporarily disable all consuming and alerts (will just return true and do nothing instead of consuming). This is useful if you have a special user-invoked action or a heavy initialization step that you don't want to trigger alerts.

### TokenBucket

You can create and use token buckets yourself, without any `Alert Managers` bossing them around. To create a bucket simply instantiate `BucketAlerts::TokenBucket`:

```cpp
BucketAlerts::TokenBucket myBucket(starting, max, replenish_rate);
```

And to register the callback to invoke when exhausted, set the public `OnBucketExhausted` pointer:

```cpp
myBucket.OnBucketExhausted = some_func; 
```

### Manual Update

By default, token buckets update (eg replenish tokens) every time you try to consume from them. However, if you're planning to consume a lot of times per second and only want updates at a constant rate (and not on every time you consume), you can disable the auto update by setting:

```cpp
BucketAlerts::Defs::AutoUpdate = false;
```

And then update buckets yourself, by calling:

```cpp
BucketAlerts::get_main().ManualUpdate();
```

(or call ManualUpdate on your own custom managers, if you don't use the default one).

### Defs

There are some global defs you can set to change the buckets behavior before you create them (note: don't change these flags while running - it will cause undefined behavior). To access these defs use the `BucketAlerts::Defs` object.

Defs you can change are:

#### BucketAlerts::Defs::ThreadSafe (default: true)

If true, will use mutex internally to make sure the buckets are thread safe. If you don't use threads, disable this option for slightly better performance.

#### BucketAlerts::Defs::DefaultCategoryId (default: -1)

Category id to use as the default category (when no category id provided).

#### BucketAlerts::Defs::ResetWhenConsumed (default: false)

If true, will reset buckets back to their starting value whenever they are exhausted (but after invoking the callback).

#### BucketAlerts::Defs::AutoUpdate (default: true)

If true, buckets will update automatically whenever you try to consume from them. If false, you'll need to call ManualUpdate() every few intervals (depending on your normal consumption rate) to make sure tokens replenish.

## License

BucketAlerts is distributed under the MIT license and is free to use for any commercial or non commercial purpose.


