/**
 @file			JCFileSystem.cpp
 @brief
 @author			James
 @version		1.0
 @date			2016_5_11
 */

#include "JCFileSystem.h"
#include <stdio.h>
#include "../util/JCCommonMethod.h"
#define ERROR_FILE_C_R_W (-6)
#pragma warning(disable:4996)


#ifdef WIN32
#else
#include <cstring>
#include <sys/stat.h>
#include <cstdlib>
#include <cerrno>
#include <dirent.h>
#include <unistd.h>
#endif



namespace laya
{
    GLOBAL_ONCREATEFILEERROR global_onCreateFileError = NULL;
    bool readFileSync( const char* p_pszFile , JCBuffer& p_buf, int p_nEncode )
    {
        if(!p_pszFile)
            return false;
        FILE* pf = NULL;
        pf=fopen(p_pszFile,"rb");
        if( pf==NULL )
            return false;
        fseek(pf,0,SEEK_END);
        int len = ftell(pf);
        bool bAsText = p_nEncode!=JCBuffer::raw ;
        fseek(pf,0,SEEK_SET);
        p_buf.create(len+(bAsText?1:0));
        int readlen = fread(p_buf.m_pPtr, 1, len, pf);
        if( readlen!=len){
            fclose(pf);
            p_buf.free();
            return false;
        }
        fclose(pf);
        if( bAsText )
            p_buf.m_pPtr[len]=0;
        return true;
    }
    std::string readFileSync1(const char* p_pszFile, const char* p_pszEncode)
    {
        JCBuffer buf;
        if (readFileSync(p_pszFile, buf, JCBuffer::utf8))
        {
            return buf.m_pPtr;
        }
        return "";
    }
    bool writeFileSync1( const char* p_pszFile, char* p_pBuff, int p_nLen, int p_nEncode)
    {
        if (!p_pszFile || strlen(p_pszFile) <= 1)
            return false;
        FILE* pFile = NULL;
        try{
            pFile = fopen( p_pszFile, "wb");
            if( pFile==NULL){
                throw ERROR_FILE_C_R_W;
                return false;
            }
            int ret = fwrite( p_pBuff, 1, p_nLen, pFile );
            if(ret<p_nLen){
                fclose(pFile);
                throw ERROR_FILE_C_R_W;
            }
            fflush(pFile);
            fclose(pFile);
        }
        catch(int e)
        {
            if( e==ERROR_FILE_C_R_W && global_onCreateFileError){
                global_onCreateFileError();
            }
            return false;
        }
        return true;
    }
    bool writeFileSync( const char* p_pszFile, JCBuffer& p_buf, int p_nEncode )
    {
        return writeFileSync1(p_pszFile, p_buf.m_pPtr, p_buf.m_nLen, p_nEncode);
    }
}

#ifndef WIN32
namespace fs
{
#   define FILESYSTEM_REMOVE_DIRECTORY(P)(::rmdir(P)== 0)
#   define FILESYSTEM_DELETE_FILE(P)(::unlink(P)== 0)
#   define FILESYSTEM_ERRNO    errno
#   define FILESYSTEM_SET_CURRENT_DIRECTORY(P)(::chdir(P)== 0)
#   define FILESYSTEM_CREATE_DIRECTORY(P)(::mkdir(P, S_IRWXU|S_IRWXG|S_IRWXO)== 0)
    typedef int err_t;
    typedef path::value_type        value_type;
    typedef path::string_type       string_type;
    typedef string_type::size_type  size_type;
    directory_iterator end_dir_itr;
    const std::error_code ok;
    const char* const separators = "/";
    const char* separator_string = "/";
    const char* preferred_separator_string = "/";
    const char dot = '.';
    const std::error_code not_found_error_code (ENOENT, std::system_category());
    
    path& path::remove_filename()
    {
        m_pathname.erase(m_parent_path_end());
        return *this;
    }
    size_type root_directory_start(const string_type & path, size_type size)
    // return npos if no root_directory found
    {
        
        
        // case "//"
        if (size == 2
            && is_directory_separator(path[0])
            && is_directory_separator(path[1])) return string_type::npos;
        
        
        // case "//net {/}"
        if (size > 3
            && is_directory_separator(path[0])
            && is_directory_separator(path[1])
            && !is_directory_separator(path[2]))
        {
            string_type::size_type pos(path.find_first_of(separators, 2));
            return pos < size ? pos : string_type::npos;
        }
        
        // case "/"
        if (size > 0 && is_directory_separator(path[0])) return 0;
        
        return string_type::npos;
    }
    size_type filename_pos(const string_type & str,size_type end_pos) // end_pos is past-the-end position
    // return 0 if str itself is filename (or empty)
    {
        // case: "//"
        if (end_pos == 2
            && is_directory_separator(str[0])
            && is_directory_separator(str[1])) return 0;
        
        // case: ends in "/"
        if (end_pos && is_directory_separator(str[end_pos-1]))
            return end_pos-1;
        
        // set pos to start of last element
        size_type pos(str.find_last_of(separators, end_pos-1));
        
        
        return (pos == string_type::npos // path itself must be a filename (or empty)
                || (pos == 1 && is_directory_separator(str[0]))) // or net
        ? 0 // so filename is entire string
        : pos + 1; // or starts after delimiter
    }
    const path&  dot_path()
    {
        static const path dot_pth(".");
        return dot_pth;
    }
    
    const path&  dot_dot_path()
    {
        static const path dot_dot("..");
        return dot_dot;
    }
    bool is_root_separator(const string_type & str, size_type pos)
    // pos is position of the separator
    {
        assert(!str.empty() && is_directory_separator(str[pos]) && "precondition violation");
        
        // subsequent logic expects pos to be for leftmost slash of a set
        while (pos > 0 && is_directory_separator(str[pos-1]))
            --pos;
        
        //  "/" [...]
        if (pos == 0)
            return true;
        
        
        //  "//" name "/"
        if (pos < 3 || !is_directory_separator(str[0])
            || !is_directory_separator(str[1]))
            return false;
        
        return str.find_first_of(separators, 2) == pos;
    }
    
    string_type::size_type path::m_parent_path_end() const
    {
        size_type end_pos(filename_pos(m_pathname, m_pathname.size()));
        
        bool filename_was_separator(m_pathname.size()
                                    && is_directory_separator(m_pathname[end_pos]));
        
        // skip separators unless root directory
        size_type root_dir_pos(root_directory_start(m_pathname, end_pos));
        for (;
             end_pos > 0
             && (end_pos-1) != root_dir_pos
             && is_directory_separator(m_pathname[end_pos-1])
             ;
             --end_pos) {}
        
        return (end_pos == 1 && root_dir_pos == 0 && filename_was_separator)
        ? string_type::npos
        : end_pos;
    }
    path path::filename() const
    {
        size_type pos(filename_pos(m_pathname, m_pathname.size()));
        return (m_pathname.size()
                && pos
                && is_directory_separator(m_pathname[pos])
                && !is_root_separator(m_pathname, pos))
        ? dot_path()
        : path(m_pathname.c_str() + pos);
    }
    path& path::operator/=(const path& p)
    {
        if (p.empty())
            return *this;
        if (this == &p)  // self-append
        {
            path rhs(p);
            if (!is_directory_separator(rhs.m_pathname[0]))
                m_append_separator_if_needed();
            m_pathname += rhs.m_pathname;
        }
        else
        {
            if (!is_directory_separator(*p.m_pathname.begin()))
                m_append_separator_if_needed();
            m_pathname += p.m_pathname;
        }
        return *this;
    }
    
    path& path::operator/=(const value_type* ptr)
    {
        if (!*ptr)
            return *this;
        if (ptr >= m_pathname.data()
            && ptr < m_pathname.data() + m_pathname.size())  // overlapping source
        {
            path rhs(ptr);
            if (!is_directory_separator(rhs.m_pathname[0]))
                m_append_separator_if_needed();
            m_pathname += rhs.m_pathname;
        }
        else
        {
            if (!is_directory_separator(*ptr))
                m_append_separator_if_needed();
            m_pathname += ptr;
        }
        return *this;
    }
    path::string_type::size_type path::m_append_separator_if_needed()
    {
        if (!m_pathname.empty() &&
            !is_directory_separator(*(m_pathname.end()-1)))
        {
            string_type::size_type tmp(m_pathname.size());
            m_pathname += preferred_separator;
            return tmp;
        }
        return 0;
    }
    path path::parent_path() const
    {
        size_type end_pos(m_parent_path_end());
        return end_pos == string_type::npos
        ? path()
        //: path(m_pathname.c_str(), m_pathname.c_str() + end_pos);
        :path(m_pathname.substr(0,end_pos));
    }
    bool not_found_error(int errval)
    {
        return errno == ENOENT || errno == ENOTDIR;
    }
    file_status status(const path& p, std::error_code* ec)
    {
        struct stat path_stat;
        if (::stat(p.c_str(), &path_stat)!= 0)
        {
            if (ec != 0)                            // always report errno, even though some
                ec->assign(errno, std::system_category());   // errno values are not status_errors
            
            if (not_found_error(errno))
            {
                return file_status(file_not_found, no_perms);
            }
            //if (ec == 0)
            //    BOOST_FILESYSTEM_THROW(filesystem_error("boost::filesystem::status",
            //                                        p, error_code(errno, system_category())));
            if (ec == 0)
                throw filesystem_error("boost::filesystem::status");
            return file_status(status_error);
        }
        if (ec != 0) ec->clear();;
        if (S_ISDIR(path_stat.st_mode))
            return file_status(directory_file,
                               static_cast<perms>(path_stat.st_mode) & perms_mask);
        if (S_ISREG(path_stat.st_mode))
            return file_status(regular_file,
                               static_cast<perms>(path_stat.st_mode) & perms_mask);
        if (S_ISBLK(path_stat.st_mode))
            return file_status(block_file,
                               static_cast<perms>(path_stat.st_mode) & perms_mask);
        if (S_ISCHR(path_stat.st_mode))
            return file_status(character_file,
                               static_cast<perms>(path_stat.st_mode) & perms_mask);
        if (S_ISFIFO(path_stat.st_mode))
            return file_status(fifo_file,
                               static_cast<perms>(path_stat.st_mode) & perms_mask);
        if (S_ISSOCK(path_stat.st_mode))
            return file_status(socket_file,
                               static_cast<perms>(path_stat.st_mode) & perms_mask);
        return file_status(type_unknown);
        
    }
    bool create_directory(const path& p, std::error_code* ec)
    {
        if (FILESYSTEM_CREATE_DIRECTORY(p.c_str()))
        {
            if (ec != 0)
                ec->clear();
            return true;
        }
        
        //  attempt to create directory failed
        int errval(FILESYSTEM_ERRNO);  // save reason for failure
        //error_code dummy;
        
        if (is_directory(p/*, dummy*/))
        {
            if (ec != 0)
                ec->clear();
            return false;
        }
        
        //  attempt to create directory failed && it doesn't already exist
        if (ec == 0)
            //BOOST_FILESYSTEM_THROW(filesystem_error("boost::filesystem::create_directory",
            //                                        p, error_code(errval, system_category())));
            throw filesystem_error("boost::filesystem::create_directory");
        else
            ec->assign(errval, std::system_category());
        
        return false;
    }
    
    bool create_directories(const path& p, std::error_code* ec)
    {
        if (p.empty())
        {
            if (ec == 0)
                //BOOST_FILESYSTEM_THROW(filesystem_error(
                //                                        "boost::filesystem::create_directories", p,
                //                                        system::errc::make_error_code(system::errc::invalid_argument)));
                throw filesystem_error("boost::filesystem::create_directories");
            else
                ec->assign((int)std::errc::invalid_argument, std::generic_category());
            return false;
        }
        
        if (p.filename_is_dot() || p.filename_is_dot_dot())
            return create_directories(p.parent_path(), ec);
        
        std::error_code local_ec;
        file_status p_status = status(p, &local_ec);
        
        if (p_status.type() == directory_file)
        {
            if (ec != 0)
                ec->clear();
            return false;
        }
        
        path parent = p.parent_path();
        //////////assert(parent != p &&"internal error: p == p.parent_path()");
        if (!parent.empty())
        {
            // determine if the parent exists
            file_status parent_status = status(parent, &local_ec);
            
            // if the parent does not exist, create the parent
            if (parent_status.type() == file_not_found)
            {
                create_directories(parent, &local_ec);
                if (local_ec)
                {
                    if (ec == 0)
                        //BOOST_FILESYSTEM_THROW(filesystem_error(
                        //                                       "boost::filesystem::create_directories", parent, local_ec));
                        throw filesystem_error("boost::filesystem::create_directories");
                    else
                        *ec = local_ec;
                    return false;
                }
            }
        }
        
        // create the directory
        return create_directory(p, ec);
    }
    bool error(err_t error_num, const path& p, std::error_code* ec, const char* message)
    {
        if (!error_num)
        {
            if (ec != 0) ec->clear();
        }
        else
        { //  error
            if (ec == 0)
                //BOOST_FILESYSTEM_THROW(filesystem_error(message,
                //                                        p, error_code(error_num, system_category())));
                throw filesystem_error(message);
            else
                ec->assign(error_num, std::system_category());
        }
        return error_num != 0;
    }
    std::time_t last_write_time(const path& p, std::error_code* ec)
    {
        struct stat path_stat;
        if (error(::stat(p.c_str(), &path_stat)!= 0 ? FILESYSTEM_ERRNO : 0,
                  p, ec, "boost::filesystem::last_write_time"))
            return std::time_t(-1);
        return path_stat.st_mtime;
    }
    
    std::uintmax_t file_size(const path& p, std::error_code* ec)
    {
        struct stat path_stat;
        if (error(::stat(p.c_str(), &path_stat)!= 0 ? FILESYSTEM_ERRNO : 0,
                  p, ec, "boost::filesystem::file_size"))
            return static_cast<std::uintmax_t>(-1);
        if (error(!S_ISREG(path_stat.st_mode) ? EPERM : 0,
                  p, ec, "boost::filesystem::file_size"))
            return static_cast<std::uintmax_t>(-1);
        
        return static_cast<std::uintmax_t>(path_stat.st_size);
    }
    // only called if directory exists
    bool remove_directory(const path& p) // true if succeeds or not found
    {
        return FILESYSTEM_REMOVE_DIRECTORY(p.c_str())
        || not_found_error(FILESYSTEM_ERRNO);  // mitigate possible file system race. See #11166
    }
    
    // only called if file exists
    bool remove_file(const path& p) // true if succeeds or not found
    {
        return FILESYSTEM_DELETE_FILE(p.c_str())
        || not_found_error(FILESYSTEM_ERRNO);  // mitigate possible file system race. See #11166
    }
    
    // called by remove and remove_all_aux
    bool remove_file_or_directory(const path& p, file_type type, std::error_code* ec)
    // return true if file removed, false if not removed
    {
        if (type == file_not_found)
        {
            if (ec != 0) ec->clear();
            return false;
        }
        
        if (type == directory_file)
        {
            if (error(!remove_directory(p) ? FILESYSTEM_ERRNO : 0, p, ec,
                      "boost::filesystem::remove"))
                return false;
        }
        else
        {
            if (error(!remove_file(p) ? FILESYSTEM_ERRNO : 0, p, ec,
                      "boost::filesystem::remove"))
                return false;
        }
        return true;
    }
    namespace detail
    {
        file_status symlink_status(const path& p, std::error_code* ec)
        {
            
            struct stat path_stat;
            if (::lstat(p.c_str(), &path_stat)!= 0)
            {
                if (ec != 0)                            // always report errno, even though some
                    ec->assign(errno, std::system_category());   // errno values are not status_errors
                
                if (errno == ENOENT || errno == ENOTDIR) // these are not errors
                {
                    return file_status(file_not_found, no_perms);
                }
                if (ec == 0)
                    //BOOST_FILESYSTEM_THROW(filesystem_error("boost::filesystem::status",
                    //                                       p, error_code(errno, system_category())));
                    throw filesystem_error("boost::filesystem::status");
                return file_status(status_error);
            }
            if (ec != 0) ec->clear();
            if (S_ISREG(path_stat.st_mode))
                return file_status(regular_file,
                                   static_cast<perms>(path_stat.st_mode) & perms_mask);
            if (S_ISDIR(path_stat.st_mode))
                return file_status(directory_file,
                                   static_cast<perms>(path_stat.st_mode) & perms_mask);
            if (S_ISLNK(path_stat.st_mode))
                return file_status(symlink_file,
                                   static_cast<perms>(path_stat.st_mode) & perms_mask);
            if (S_ISBLK(path_stat.st_mode))
                return file_status(block_file,
                                   static_cast<perms>(path_stat.st_mode) & perms_mask);
            if (S_ISCHR(path_stat.st_mode))
                return file_status(character_file,
                                   static_cast<perms>(path_stat.st_mode) & perms_mask);
            if (S_ISFIFO(path_stat.st_mode))
                return file_status(fifo_file,
                                   static_cast<perms>(path_stat.st_mode) & perms_mask);
            if (S_ISSOCK(path_stat.st_mode))
                return file_status(socket_file,
                                   static_cast<perms>(path_stat.st_mode) & perms_mask);
            return file_status(type_unknown);
            
        }
    }
    inline file_type query_file_type(const path& p, std::error_code* ec)
    {
        return detail::symlink_status(p, ec).type();
    }
    bool remove(const path& p, std::error_code* ec)
    {
        std::error_code tmp_ec;
        file_type type = query_file_type(p, &tmp_ec);
        if (error(type == status_error ? tmp_ec.value() : 0, p, ec,
                  "boost::filesystem::remove"))
            return false;
        
        // Since POSIX remove() is specified to work with either files or directories, in a
        // perfect world it could just be called. But some important real-world operating
        // systems (Windows, Mac OS X, for example) don't implement the POSIX spec. So
        // remove_file_or_directory() is always called to keep it simple.
        return remove_file_or_directory(p, type, ec);
    }
    std::uintmax_t remove_all_aux(const path& p, file_type type,std::error_code* ec)
    {
        std::uintmax_t count = 1;
        if (type == directory_file)  // but not a directory symlink
        {
            directory_iterator itr;
            if (ec != 0)
            {
                itr = directory_iterator(p, *ec);
                if (*ec)
                    return count;
            }
            else
                itr = directory_iterator(p);
            for (; itr != end_dir_itr; ++itr)
            {
                file_type tmp_type = query_file_type(itr->path(), ec);
                if (ec != 0 && *ec)
                    return count;
                count += remove_all_aux(itr->path(), tmp_type, ec);
                if (ec != 0 && *ec)
                    return count;
            }
        }
        remove_file_or_directory(p, type, ec);
        return count;
    }
    std::uintmax_t remove_all(const path& p, std::error_code* ec)
    {
        std::error_code tmp_ec;
        file_type type = query_file_type(p, &tmp_ec);
        if (error(type == status_error ? tmp_ec.value() : 0, p, ec,
                  "boost::filesystem::remove_all"))
            return 0;
        
        return (type != status_error && type != file_not_found) // exists
        ? remove_all_aux(p, type, ec)
        : 0;
    }
    namespace detail
    {
        std::error_code path_max(std::size_t & result)
        // this code is based on Stevens and Rago, Advanced Programming in the
        // UNIX envirnment, 2nd Ed., ISBN 0-201-43307-9, page 49
        {
#   ifdef PATH_MAX
            static std::size_t max = PATH_MAX;
#   else
            static std::size_t max = 0;
#   endif
            if (max == 0)
            {
                errno = 0;
                long tmp = ::pathconf("/", _PC_NAME_MAX);
                if (tmp < 0)
                {
                    if (errno == 0)// indeterminate
                        max = 4096; // guess
                    else return std::error_code(errno, std::system_category());
                }
                else max = static_cast<std::size_t>(tmp + 1); // relative root
            }
            result = max;
            return ok;
        }
        
#if defined(__PGI) && defined(__USE_FILE_OFFSET64)
#define dirent dirent64
#endif
        
        std::error_code dir_itr_first(void *& handle, void *& buffer,const char* dir, std::string& target,file_status &, file_status &)
        {
            if ((handle = ::opendir(dir))== 0)
                return std::error_code(errno, std::system_category());
            target = std::string(".");  // string was static but caused trouble
            // when iteration called from dtor, after
            // static had already been destroyed
            std::size_t path_size (0);  // initialization quiets gcc warning (ticket #3509)
            std::error_code ec = path_max(path_size);
            if (ec)return ec;
            dirent de;
            buffer = std::malloc((sizeof(dirent) - sizeof(de.d_name))
                                 +  path_size + 1); // + 1 for "/0"
            return ok;
        }
        
        // warning: the only dirent member updated is d_name
        inline int readdir_r_simulator(DIR * dirp, struct dirent * entry,struct dirent ** result)// *result set to 0 on end of directory
        {
            errno = 0;
            
#   if !defined(__CYGWIN__)\
            && defined(_POSIX_THREAD_SAFE_FUNCTIONS)\
            && defined(_SC_THREAD_SAFE_FUNCTIONS)\
            && (_POSIX_THREAD_SAFE_FUNCTIONS+0 >= 0)\
            && (!defined(__hpux) || defined(_REENTRANT)) \
            && (!defined(_AIX) || defined(__THREAD_SAFE))
            if (::sysconf(_SC_THREAD_SAFE_FUNCTIONS)>= 0)
            { return ::readdir_r(dirp, entry, result); }
#   endif
            
            struct dirent * p;
            *result = 0;
            if ((p = ::readdir(dirp))== 0)
                return errno;
            std::strcpy(entry->d_name, p->d_name);
            *result = entry;
            return 0;
        }
        
        std::error_code dir_itr_increment(void *& handle, void *& buffer,std::string& target, file_status & sf, file_status & symlink_sf)
        {
            assert(buffer != 0);
            dirent * entry(static_cast<dirent *>(buffer));
            dirent * result;
            int return_code;
            if ((return_code = readdir_r_simulator(static_cast<DIR*>(handle), entry, &result))!= 0)
                return std::error_code(errno, std::system_category());
            if (result == 0)
                return detail::dir_itr_close(handle, buffer);
            target = entry->d_name;
#   ifdef _DIRENT_HAVE_D_TYPE //BOOST_FILESYSTEM_STATUS_CACHE
            if (entry->d_type == DT_UNKNOWN) // filesystem does not supply d_type value
            {
                sf = symlink_sf = file_status(status_error);
            }
            else  // filesystem supplies d_type value
            {
                if (entry->d_type == DT_DIR)
                    sf = symlink_sf = file_status(directory_file);
                else if (entry->d_type == DT_REG)
                    sf = symlink_sf = file_status(regular_file);
                else if (entry->d_type == DT_LNK)
                {
                    sf = file_status(status_error);
                    symlink_sf = file_status(symlink_file);
                }
                else sf = symlink_sf = file_status(status_error);
            }
#   else
            sf = symlink_sf = file_status(status_error);
#    endif
            return ok;
        }
        
        //  dir_itr_close is called both from the ~dir_itr_imp()destructor
        //  and dir_itr_increment()
        std::error_code dir_itr_close( void *& handle, void *& buffer)
        {
            
            std::free(buffer);
            buffer = 0;
            if (handle == 0)return ok;
            DIR * h(static_cast<DIR*>(handle));
            handle = 0;
            return std::error_code(::closedir(h)== 0 ? 0 : errno, std::system_category());
            
        }
        
        void directory_iterator_construct(directory_iterator& it,const path& p, std::error_code* ec)
        {
            if (error(p.empty() ? not_found_error_code.value() : 0, p, ec,
                      "boost::filesystem::directory_iterator::construct"))
                return;
            
            path::string_type filename;
            file_status file_stat, symlink_file_stat;
            std::error_code result = dir_itr_first(it.m_imp->handle,
                                                   
                                                   it.m_imp->buffer,
                                                   
                                                   p.c_str(), filename, file_stat, symlink_file_stat);
            
            if (result)
            {
                it.m_imp.reset();
                error(result.value(), p,
                      ec, "boost::filesystem::directory_iterator::construct");
                return;
            }
            
            if (it.m_imp->handle == 0)
                it.m_imp.reset(); // eof, so make end iterator
            else // not eof
            {
                it.m_imp->dir_entry.assign(p / filename, file_stat, symlink_file_stat);
                if (filename[0] == dot // dot or dot-dot
                    && (filename.size()== 1
                        || (filename[1] == dot
                            && filename.size()== 2)))
                {  it.increment(*ec); }
            }
        }
        
        void directory_iterator_increment(directory_iterator& it,std::error_code* ec)
        {
            assert(it.m_imp.get()&&"attempt to increment end iterator");
            assert(it.m_imp->handle != 0 && "internal program error");
            
            path::string_type filename;
            file_status file_stat, symlink_file_stat;
            std::error_code temp_ec;
            
            for (;;)
            {
                temp_ec = dir_itr_increment(it.m_imp->handle,
                                            
                                            it.m_imp->buffer,
                                            filename, file_stat, symlink_file_stat);
                
                if (temp_ec)  // happens if filesystem is corrupt, such as on a damaged optical disc
                {
                    path error_path(it.m_imp->dir_entry.path().parent_path());  // fix ticket #5900
                    it.m_imp.reset();
                    if (ec == 0)
                        //BOOST_FILESYSTEM_THROW(
                        //                       filesystem_error("boost::filesystem::directory_iterator::operator++",
                        //                                        error_path,
                        //                                        error_code(FILESYSTEM_ERRNO, std::system_category())));
                        throw filesystem_error("boost::filesystem::operator++");
                    ec->assign(FILESYSTEM_ERRNO, std::system_category());
                    return;
                }
                else if (ec != 0) ec->clear();
                
                if (it.m_imp->handle == 0)  // eof, make end
                {
                    it.m_imp.reset();
                    return;
                }
                
                if (!(filename[0] == dot // !(dot or dot-dot)
                      && (filename.size()== 1
                          || (filename[1] == dot
                              && filename.size()== 2))))
                {
                    it.m_imp->dir_entry.replace_filename(
                                                         filename, file_stat, symlink_file_stat);
                    return;
                }
            }
        }
    }  // namespace detail
}
#endif

//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------
