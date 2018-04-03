#include "Source/AlertsManager.h"
#include <iostream>
#include <thread>
#include <stdio.h>
#include <conio.h>

using namespace std::chrono_literals;

#define TEST_CATEGORY 5
#define TEST_BUCKET 15

// are we still running?
bool _is_running = true;

// for testing slowly consume over time
void slowly_consume()
{
	while (_is_running)
	{
		BucketAlerts::get_main().Consume(TEST_CATEGORY, TEST_BUCKET, 0.1);
		std::this_thread::sleep_for(1s);
		std::cout << "Tokens: " << BucketAlerts::get_main().GetBucket(TEST_CATEGORY, TEST_BUCKET).Count() << std::endl;
		std::cout << "Total Consumed: " << BucketAlerts::get_main().GetBucket(TEST_CATEGORY, TEST_BUCKET).TotalConsumed() << std::endl;
	}
}


int main()
{
	// reset bucket whenever we hit alert
	BucketAlerts::Defs::ResetWhenConsumed = true;

	// create test bucket
	BucketAlerts::get_main().CreateBucket(TEST_CATEGORY, TEST_BUCKET, 5, 10, 1, [](const BucketAlerts::TokenBucket& bucket) {
		std::cout << "ALERT CALLED!" << std::endl;
	});

	// init update thread
	std::thread update_thread(slowly_consume);

	// instructions
	std::cout << "-------------------------" << std::endl;
	std::cout << "Press z to consume a token." << std::endl;
	std::cout << "Press x to restore a token." << std::endl;
	std::cout << "Press q to quit." << std::endl;
	std::cout << "-------------------------" << std::endl;

	// get input
	char c;
	do {
		c = _getch();
		if (c == 'z')
		{
			BucketAlerts::get_main().Consume(TEST_CATEGORY, TEST_BUCKET, 1.0);
		}
		else if (c == 'x')
		{
			BucketAlerts::get_main().Restore(TEST_CATEGORY, TEST_BUCKET, 1.0);
		}
	} while (c != 'q');

	// stop running
	_is_running = false;

	// wait for all threads
	update_thread.join();

	// success
	return 0;
}