/*
MIT License
Copyright (c) 2019-2022 Sven Lukas

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

#ifndef ESL_CRYPTO_PUBLICKEY_H_
#define ESL_CRYPTO_PUBLICKEY_H_

#include <esl/object/Object.h>
//#include <esl/plugin/Plugin.h>

#include <string>

namespace esl {
namespace crypto {

class PublicKey : public object::Object {
public:
	//using Plugin = plugin::Plugin<PublicKey>;

	virtual std::string getX509_DER() const = 0;
	virtual std::string getX509_PEM() const = 0;

	virtual std::string getAlgorithmName() const = 0;
	virtual unsigned int getAlgorithmBits() const = 0;

	virtual std::string encrypt(const std::string& plaintext) const = 0;
	virtual bool verify(const std::string& data, const std::string& signature, const std::string& algorithm = "RS512") const = 0;
};

} /* namespace crypto */
} /* namespace esl */

#endif /* ESL_CRYPTO_PUBLICKEY_H_ */
