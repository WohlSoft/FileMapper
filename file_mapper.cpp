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

#include "file_mapper.h"

#if defined(__unix__) || defined(__APPLE__)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#elif _WIN32
#include <windows.h>
#endif

namespace charsets_utils {
    /*!
     * \brief returns length of UTF8 string line
     * \param Input 8-bit string in UTF8 codepage
     * \return number of characters (not a bytes!)
     */
    size_t utf8len(const char *s);
    int UTF8Str_To_WStr(std::wstring &dest, const std::string  &source);
    int WStr_To_UTF8Str(std::string  &dest, const std::wstring &source);
}


size_t charsets_utils::utf8len(const char *s)
{
    size_t len = 0;
    while(*s)
        len += (*(s++)&0xC0)!=0x80;
    return len;
}


int charsets_utils::UTF8Str_To_WStr(std::wstring &dest, const std::string &source)
{
    #ifdef _WIN32
    dest.resize(utf8len(source.c_str()));
    return MultiByteToWideChar(CP_UTF8, 0, source.c_str(), source.length(), (wchar_t*)dest.c_str(), source.length());
    #else
    (void)dest; (void)source;
    return utf8len(source.c_str());
    #endif
}

int charsets_utils::WStr_To_UTF8Str(std::string &dest, const std::wstring &source)
{
    #ifdef _WIN32
    int dest_len = WideCharToMultiByte(CP_UTF8, 0, source.c_str(), source.length(), (LPSTR)dest.c_str(), 0, NULL, NULL);
    dest.resize(dest_len);
    WideCharToMultiByte(CP_UTF8, 0, source.c_str(), source.length(), (LPSTR)dest.c_str(), dest_len, NULL, NULL);
    return dest_len;
    #else
    (void)dest; (void)source;
    return source.size();
    #endif
}

FileMapper::FileMapper() :
    #ifdef _WIN32
        m_File(NULL),m_Map(NULL),m_Address(NULL),
    #endif
        data(NULL),size(0)
{}

FileMapper::FileMapper(std::string file)
{
    open_file(file);
}

FileMapper::FileMapper(const FileMapper &fm) :
    #ifdef _WIN32
        m_File(NULL),m_Map(NULL),m_Address(NULL),
    #endif
        data(NULL),size(0)
{
    if(fm.data)
    {
        open_file(fm.m_path);
    }
    m_error=fm.m_error;
}

bool FileMapper::open_file(std::string path)
{
#if defined(__unix__) || defined(__APPLE__)
    struct  stat sb;
    int     m_fd;
    m_fd = open(path.c_str(), O_RDONLY);

    if(m_fd == -1 )
    {
        return false;
    }

    if(fstat(m_fd, &sb) == -1)
    {
        m_error="Failed to take state of file "+path;
        return false;
    }

    if(!S_ISREG (sb.st_mode))
    {
        m_error=path+" is not a file";
        return false;
    }

    data = mmap(0, sb.st_size, PROT_READ, MAP_SHARED, m_fd, 0);

    if(data == MAP_FAILED)
    {
        m_error="Failed to map file "+path;
        return false;
    }

    if(close(m_fd) ==-1 )
    {
        m_error="fd is not closed validely "+path;
        return false;
    }
    size = sb.st_size;
#elif _WIN32
    std::wstring wpath;
    charsets_utils::UTF8Str_To_WStr(wpath, path);
    m_File = CreateFileW(wpath.c_str(), GENERIC_READ, 1, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (m_File == INVALID_HANDLE_VALUE)
    {
        m_error="Failed to open file "+path;
        return false;
    }

    size = GetFileSize(m_File, NULL);

    m_Map = CreateFileMappingW(m_File, NULL, PAGE_READONLY, 0, 0, NULL);
    if( m_Map == NULL )
    {
        CloseHandle(m_File);
        m_error="Failed to map file "+path;
        return false;
    }

    m_Address = MapViewOfFile(m_Map, FILE_MAP_READ, 0, 0, size);
    if(m_Address == NULL)
    {
        CloseHandle(m_Map);
        CloseHandle(m_File);
        m_error="Failed to take map address for a file "+path;
        return false;
    }

    data = m_Address;
#endif

    m_path=path;
    return true;
}

bool FileMapper::close_file()
{
    if(data)
    {
        #if defined(__unix__) || defined(__APPLE__)
        if(munmap(data, size)==-1)
        {
            m_error="Fail to unmap";
            return false;
        }
        #elif _WIN32
        if (m_Address != NULL)
        {
            try{ UnmapViewOfFile(m_Address); } catch(void * /*e*/) {}
            m_Address=NULL;
        }

        if (m_Map != NULL)
        {
            try{ CloseHandle(m_Map); } catch(void * /*e*/) {}
            m_Map=NULL;
        }

        if (m_File != INVALID_HANDLE_VALUE)
        {
            try{ CloseHandle(m_File);} catch(void* /*e*/) {}
            m_File = NULL;
        }
        #endif
        m_path.clear();
        data = NULL;
    }
    return true;
}

FileMapper::~FileMapper()
{
    if(data)
    {
        close_file();
    }
}

std::string FileMapper::error()
{
    return m_error;
}
