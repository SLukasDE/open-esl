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

#ifndef ESA_OBJECT_PROCESSINGCONTEXT_H_
#define ESA_OBJECT_PROCESSINGCONTEXT_H_

#include <esa/object/Context.h>
#include <esa/object/ProcessingHandler.h>

#include <boost/filesystem/path.hpp>

#include <string>


namespace esa {
inline namespace v1_6 {
namespace object {

class ProcessingContext : public ProcessingHandler, public Context {
public:
	virtual void addData(const std::string& configuration) = 0;
	virtual void addFile(const boost::filesystem::path& filename) = 0;

	virtual int getReturnCode() const = 0;

	virtual void setParentObjectContext(Context* aParentObject) = 0;

	//virtual ProcessingContext& addData(const std::string& configuration) = 0;
	//virtual ProcessingContext& addFile(const boost::filesystem::path& filename) = 0;

	virtual void addAlias(const std::string& destinationId, const std::string& sourceId) = 0;

	inline void addReference(const std::string& sourceId) {
		addAlias("", sourceId);
	}

	/* From Event:
	 * -------------------
	 * virtual void onEvent(const Object& object) = 0;
	 */

	/* From Context:
	 * -------------------
	 * virtual std::set<std::string> getObjectIds() const = 0;
	 */

	/* From Procedure:
	 * ---------------
	 * virtual void procedureRun(Context& objectContext) = 0;
	 */

protected:
	/* From Context:
	 * -------------------
	 * virtual Object* findRawObject(const std::string& id) = 0;
	 * virtual const Object* findRawObject(const std::string& id) const = 0;
	 * virtual void addRawObject(const std::string& id, std::unique_ptr<Object> object) = 0;
	 */
};

} /* namespace object */
} /* inline namespace v1_6 */
} /* namespace esa */

#endif /* ESA_OBJECT_PROCESSINGCONTEXT_H_ */
