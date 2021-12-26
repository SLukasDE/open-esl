/*
MIT License
Copyright (c) 2019-2021 Sven Lukas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ESL_PROCESSING_JOB_RUNNER_H_
#define ESL_PROCESSING_JOB_RUNNER_H_

#include <esl/processing/job/Interface.h>
#include <esl/processing/job/Context.h>
#include <esl/processing/job/Handle.h>
#include <esl/processing/job/Job.h>
#include <esl/module/Implementation.h>

#include <memory>

namespace esl {
namespace processing {
namespace job {

class Runner : public Interface::Runner {
public:
	static module::Implementation& getDefault();

	Runner(const Interface::Settings& settings = getDefault().getSettings(),
			const std::string& implementation = getDefault().getImplementation());

	Handle addJob(std::unique_ptr<Job> job) override;
	void cancel(Handle jobId) override;

private:
	std::unique_ptr<Interface::Runner> runner;
};

} /* namespace job */
} /* namespace processing */
} /* namespace esl */

#endif /* ESL_PROCESSING_JOB_RUNNER_H_ */