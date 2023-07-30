/*
MIT License
Copyright (c) 2019-2023 Sven Lukas

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

#ifndef ESL_OBJECT_PROCEDURE_H_
#define ESL_OBJECT_PROCEDURE_H_

#ifdef ESL_1_6
#include <esl/object/Object.h>
#include <esl/object/Context.h>

namespace esl {
inline namespace v1_6 {
namespace object {

class Procedure : public virtual Object {
public:
	/* this method is blocking. */
	//virtual std::unique_ptr<object::Context> procedureRun(object::Context& context) = 0;
	virtual void procedureRun(Context& context) = 0;

	/* this method is non-blocking. */
	virtual void procedureCancel() { };
};
#else
#include <esl/processing/Procedure.h>

namespace esl {
inline namespace v1_6 {
namespace object {

using Procedure = processing::Procedure;
#endif

} /* namespace object */
} /* inline namespace v1_6 */
} /* namespace esl */

#endif /* ESL_OBJECT_PROCEDURE_H_ */
