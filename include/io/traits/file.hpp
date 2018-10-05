/*
 * Copyright 2018 Justas Masiulis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include "../base_file.hpp"

namespace ntw::io::traits {

    template<class Handle, bool Synchronous>
    struct basic_file_traits {
        using handle_type  = Handle;
        using options_type = pipe_options;

        constexpr static auto options = ntw::io::file_options{}.share_all().full_access();

        NT_FN static open(void*&              handle,
                          OBJECT_ATTRIBUTES&  attributes,
                          const options_type& options,
                          unsigned long       disposition);
    };

    template<class Handle>
    using file_traits = basic_file_traits<Handle, true>;

    template<class Handle>
    using async_file_traits = basic_file_traits<Handle, false>;

    template<class Handle, bool Sync>
    NT_FN basic_file_traits<Handle, Sync>::open(void*&              handle,
                                                OBJECT_ATTRIBUTES&  attributes,
                                                const options_type& options,
                                                unsigned long       disposition)
    {
        IO_STATUS_BLOCK status_block;
        return LI_NT(NtCreateFile)(&temp_handle,
                                   options._access | (Sync ? SYNCHRONIZE : 0),
                                   &attributes,
                                   &status_block,
                                   nullptr,
                                   options._attributes ? options._attributes
                                                       : FILE_ATTRIBUTE_NORMAL,
                                   options._share_access,
                                   disposition,
                                   options._options | FILE_NON_DIRECTORY_FILE |
                                       (Sync ? FILE_SYNCHRONOUS_IO_NONALERT : 0),
                                   nullptr,
                                   0);
    }; // namespace ntw::io::traits


} // namespace ntw::io::traits