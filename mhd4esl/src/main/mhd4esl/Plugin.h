/*
 * This file is part of mhd4esl.
 * Copyright (C) 2019-2023 Sven Lukas
 *
 * Mhd4esl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mhd4esl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with mhd4esl.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef MHD4ESL_PLUGIN_H_
#define MHD4ESL_PLUGIN_H_

#include <esl/plugin/Registry.h>

namespace mhd4esl {
inline namespace v1_6 {

class Plugin final {
public:
	Plugin() = delete;
	static void install(esl::plugin::Registry& registry, const char* data);
};

} /* inline namespace v1_6 */
} /* namespace mhd4esl */

#endif /* MHD4ESL_PLUGIN_H_ */
