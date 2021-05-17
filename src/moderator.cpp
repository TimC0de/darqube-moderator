#include <thread>
#include <future>
#include <string>

#include "moderator.h"

#include "filter/filter.h"
#include "user/penalty_calculator.h"

namespace darqube::moderator {

	void filterInputMessage(std::string message, std::promise<FilterOutput> filtering_output_promise) {
		filter::Filter messageFilter(message);
		messageFilter.censor();

		filtering_output_promise.set_value(
			FilterOutput{
				messageFilter.getCensoredMessage(),
				messageFilter.getCensoredWordsCount(),
				messageFilter.getTotalWordsCount()
			}
		);
	}

	void processFilteredData(std::string uid, std::future<FilterOutput> filtering_output_future, std::promise<Output> processed_output_promise) {
		user::PenaltyCalculator userPenaltyCalculator(uid);
		filtering_output_future.wait();

		// Receive filter output from message filtering process and calculate penalty for banned messages
		FilterOutput filterOutput = filtering_output_future.get();
		Output output = userPenaltyCalculator.calculateOutput(filterOutput);

		processed_output_promise.set_value(output);
	}

	Output censorMessage(Input input) {
		Output resultOutput;

		// Processing filtering thread
		std::promise<FilterOutput> filtering_output_promise;
		std::future<FilterOutput> filtering_output_future = filtering_output_promise.get_future();
		std::thread filteringThread(filterInputMessage, input.message, std::move(filtering_output_promise));

		// Processing penalty calculating thread
		std::promise<Output> processed_output_promise;
		std::future<Output> processed_output_future = processed_output_promise.get_future();
		std::thread filteredDataProcessingThread(processFilteredData, input.uid, std::move(filtering_output_future), std::move(processed_output_promise));

		// Wait for all threads to finish and receive output object
		processed_output_future.wait();
		resultOutput = processed_output_future.get();
		filteringThread.join();
		filteredDataProcessingThread.join();

		return resultOutput;
	}

}