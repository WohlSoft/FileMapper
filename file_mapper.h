/*
 * FileMapper - a small C++ class which providing a cross-platform read-only file mapping interface
 * Copyright (c) 2016 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following 
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef FILEMAPPER_H
#define FILEMAPPER_H

#include <string>

#if defined(__unix__) || defined(__APPLE__)
#include <sys/types.h>
    #if defined(__APPLE__)
        #define FileManSize off_t
    #elif defined __x86_64__ || !defined __USE_FILE_OFFSET64
        #define FileManSize __off_t
    #else
        #define FileManSize __off64_t
    #endif
#else
    #define FileManSize long
#endif

/*!
 * \brief Provides cross-platform file memory mapping interface
 */
class FileMapper
{
    #ifdef _WIN32
    void* m_File;
    void* m_Map;
    void* m_Address;
    #endif
    //! Full path to opened file
    std::string m_path;
    //! Recent occouped error description
    std::string m_error;

public:
    //! Pointer to mapped file data
    void            *data;

    //! Size of mapped file
    FileManSize  size;

    /*!
     * \brief Constructor
     */
    FileMapper();

    /*!
     * \brief Constructor with pre-opened file
     */
    FileMapper(std::string file);

    /*!
     * \brief Copy Constructor
     */
    FileMapper(const FileMapper &fm);

    /*!
     * \brief Opens file
     * \param path to file
     * \return True if success. False if error occouped
     */
    bool open_file(std::string path);

    /*!
     * \brief Closes opened file
     * \return True if no errors
     */
    bool close_file();

    /*!
     * \brief Destructor
     */

    virtual ~FileMapper();
    /*!
     * \brief Returns recent occouped error info
     * \return recent occouped error info
     */
    std::string error();
};

#endif // FILEMAPPER_H
