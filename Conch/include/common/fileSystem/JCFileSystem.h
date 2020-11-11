/**
 @file			JCFileSystem.h
 @brief
 @author			James
 @version		1.0
 @date			2016_5_11
 */
#ifndef __JCFileSystem_H__
#define __JCFileSystem_H__

#include "../buffer/JCBuffer.h"

#ifdef WIN32
#include <filesystem>
namespace  fs = std::experimental::filesystem::v1;
#else
#include <string>
#include <ctime>
#include <cerrno>
#include <cassert>
#include <system_error>
#endif
namespace laya
{
    
    /** @brief 同步读取本地文件
     *  @param[in] 文件名字
     *  @param[out] 返回的buffer
     *  @param[in] Encode是表示文件内容的原本的编码方式。即用这种编码方式去解释内容
     *  @return 是否读取成功
     */
    bool readFileSync(const char* p_pszFile, JCBuffer& p_buf, int p_nEncode = JCBuffer::raw);
    
    
    /** @brief 同步读取本地文件
     *  @param[in] 文件名字
     *  @param[in] Encode是表示文件内容的原本的编码方式。即用这种编码方式去解释内容
     *  @return 返回字符串
     */
    std::string readFileSync1(const char* p_pszFile, const char* p_pszEncode);
    
    /** @brief 同步写本地文件
     *  @param[in] 文件名字
     *  @param[in] 返回的buffer
     *  @param[in] Encode是要把文件保存成什么的编码方式
     *  @return 是否写入成功
     */
    bool writeFileSync(const char* p_pszFile, JCBuffer& p_buf, int p_nEncode = JCBuffer::raw);
    
    /** @brief 写本地文件
     *  @param[in] 文件名
     *  @param[in] 写入的buffer
     *  @param[in] 长度
     *  @param[in] encodetype
     *  @return 是否写入成功
     */
    bool writeFileSync1(const char* p_pszFile, char* p_pBuff, int p_nLen, int p_nEncode = JCBuffer::raw);
    
    typedef void(*GLOBAL_ONCREATEFILEERROR)();
    
    extern GLOBAL_ONCREATEFILEERROR global_onCreateFileError;
}
#ifdef WIN32
#else // __APPLE__ANDROID
namespace fs
{
#define FILESYSTEM_BITMASK(Bitmask)                                            \
    \
    inline Bitmask operator| (Bitmask x , Bitmask y )                       \
    { return static_cast<Bitmask>( static_cast<std::int_least32_t>(x)     \
                                  | static_cast<std::int_least32_t>(y)); }                          \
    \
    inline Bitmask operator& (Bitmask x , Bitmask y )                       \
    { return static_cast<Bitmask>( static_cast<std::int_least32_t>(x)     \
                                  & static_cast<std::int_least32_t>(y)); }                          \
    \
    inline Bitmask operator^ (Bitmask x , Bitmask y )                       \
    { return static_cast<Bitmask>( static_cast<std::int_least32_t>(x)     \
                                  ^ static_cast<std::int_least32_t>(y)); }                          \
    \
    inline Bitmask operator~ (Bitmask x )                                   \
    { return static_cast<Bitmask>(~static_cast<std::int_least32_t>(x)); } \
    \
    inline Bitmask & operator&=(Bitmask & x , Bitmask y)                    \
    { x = x & y ; return x ; }                                              \
    \
    inline Bitmask & operator|=(Bitmask & x , Bitmask y)                    \
    { x = x | y ; return x ; }                                              \
    \
    inline Bitmask & operator^=(Bitmask & x , Bitmask y)                    \
    { x = x ^ y ; return x ; }
    enum file_type
    {
        status_error,
#   ifndef BOOST_FILESYSTEM_NO_DEPRECATED
        status_unknown = status_error,
#   endif
        file_not_found,
        regular_file,
        directory_file,
        // the following may not apply to some operating systems or file systems
        symlink_file,
        block_file,
        character_file,
        fifo_file,
        socket_file,
        reparse_file,  // Windows: FILE_ATTRIBUTE_REPARSE_POINT that is not a symlink
        type_unknown,  // file does exist, but isn't one of the above types or
        // we don't have strong enough permission to find its type
        
        _detail_directory_symlink  // internal use only; never exposed to users
    };
    enum perms
    {
        no_perms = 0,       // file_not_found is no_perms rather than perms_not_known
        
        // POSIX equivalent macros given in comments.
        // Values are from POSIX and are given in octal per the POSIX standard.
        
        // permission bits
        
        owner_read = 0400,  // S_IRUSR, Read permission, owner
        owner_write = 0200, // S_IWUSR, Write permission, owner
        owner_exe = 0100,   // S_IXUSR, Executearch permission, owner
        owner_all = 0700,   // S_IRWXU, Read, write, executearch by owner
        
        group_read = 040,   // S_IRGRP, Read permission, group
        group_write = 020,  // S_IWGRP, Write permission, group
        group_exe = 010,    // S_IXGRP, Executearch permission, group
        group_all = 070,    // S_IRWXG, Read, write, executearch by group
        
        others_read = 04,   // S_IROTH, Read permission, others
        others_write = 02,  // S_IWOTH, Write permission, others
        others_exe = 01,    // S_IXOTH, Executearch permission, others
        others_all = 07,    // S_IRWXO, Read, write, executearch by others
        
        all_all = 0777,     // owner_all|group_all|others_all
        
        // other POSIX bits
        
        set_uid_on_exe = 04000, // S_ISUID, Set-user-ID on execution
        set_gid_on_exe = 02000, // S_ISGID, Set-group-ID on execution
        sticky_bit = 01000, // S_ISVTX,
        // (POSIX XSI) On directories, restricted deletion flag
        // (V7) 'sticky bit': save swapped text even after use
        // (SunOS) On non-directories: don't cache this file
        // (SVID-v4.2) On directories: restricted deletion flag
        // Also see http://en.wikipedia.org/wiki/Sticky_bit
        
        perms_mask = 07777,     // all_all|set_uid_on_exe|set_gid_on_exe|sticky_bit
        
        perms_not_known = 0xFFFF, // present when directory_entry cache not loaded
        
        // options for permissions() function
        
        add_perms = 0x1000,     // adds the given permission bits to the current bits
        remove_perms = 0x2000,  // removes the given permission bits from the current bits;
        // choose add_perms or remove_perms, not both; if neither add_perms
        // nor remove_perms is given, replace the current bits with
        // the given bits.
        
        symlink_perms = 0x4000  // on POSIX, don't resolve symlinks; implied on Windows
    };
    FILESYSTEM_BITMASK(perms)
    class  file_status
    {
    public:
        file_status()
        : m_value(status_error), m_perms(perms_not_known) {}
        explicit file_status(file_type v)
        : m_value(v), m_perms(perms_not_known) {}
        file_status(file_type v, perms prms)
        : m_value(v), m_perms(prms) {}
        
        //  As of October 2015 the interaction between noexcept and =default is so troublesome
        //  for VC++, GCC, and probably other compilers, that =default is not used with noexcept
        //  functions. GCC is not even consistent for the same release on different platforms.
        
        file_status(const file_status& rhs)
        : m_value(rhs.m_value), m_perms(rhs.m_perms) {}
        file_status& operator=(const file_status& rhs)
        {
            m_value = rhs.m_value;
            m_perms = rhs.m_perms;
            return *this;
        }
        
        // observers
        file_type  type() const { return m_value; }
        perms      permissions() const { return m_perms; }
        
        // modifiers
        void       type(file_type v) { m_value = v; }
        void       permissions(perms prms) { m_perms = prms; }
        
        bool operator==(const file_status& rhs) const
        {
            return type() == rhs.type() &&
            permissions() == rhs.permissions();
        }
        bool operator!=(const file_status& rhs) const
        {
            return !(*this == rhs);
        }
        
    private:
        file_type   m_value;
        enum perms  m_perms;
    };
    class  filesystem_error
    {
    public:
        filesystem_error(const std::string & what_arg)
        {
        }
        const char* what()
        {
            return m_what.c_str();
        }
    private:
        mutable std::string  m_what;
    };
    
    class path
    {
    public:
        typedef char                           value_type;
        static const value_type      separator = '/';
        static const value_type      preferred_separator = '/';
        static const value_type      dot = '.';
        typedef std::basic_string<value_type> string_type;
        
        path() {}
        path(const path& p) : m_pathname(p.m_pathname) {}
        path(const value_type* s) : m_pathname(s) {}
        path(value_type* s) : m_pathname(s) {}
        path(const string_type& s) : m_pathname(s) {}
        path(string_type& s) : m_pathname(s) {}
        
        path& remove_filename();
        path filename() const;
        void clear() { m_pathname.clear(); }
        const value_type*   c_str() const { return m_pathname.c_str(); }
        const std::string&  generic_string() const { return m_pathname; }
        
        path& operator=(const path& p)
        {
            m_pathname = p.m_pathname;
            return *this;
        }
        
        //  value_type overloads
        
        path& operator=(const value_type* ptr)  // required in case ptr overlaps *this
        {
            m_pathname = ptr; return *this;
        }
        path& operator=(value_type* ptr)  // required in case ptr overlaps *this
        {
            m_pathname = ptr; return *this;
        }
        path& operator=(const string_type& s) { m_pathname = s; return *this; }
        path& operator=(string_type& s) { m_pathname = s; return *this; }
        
        //  value_type overloads. Same rationale as for constructors above
        path& operator+=(const path& p) { m_pathname += p.m_pathname; return *this; }
        path& operator+=(const value_type* ptr) { m_pathname += ptr; return *this; }
        path& operator+=(value_type* ptr) { m_pathname += ptr; return *this; }
        path& operator+=(const string_type& s) { m_pathname += s; return *this; }
        path& operator+=(string_type& s) { m_pathname += s; return *this; }
        path& operator+=(value_type c) { m_pathname += c; return *this; }
        
        path& operator/=(const path& p);
        
        path& operator/=(const value_type* ptr);
        path& operator/=(value_type* ptr)
        {
            return this->operator/=(const_cast<const value_type*>(ptr));
        }
        path& operator/=(const string_type& s) { return this->operator/=(path(s)); }
        path& operator/=(string_type& s) { return this->operator/=(path(s)); }
        
        bool empty() const { return m_pathname.empty(); }
        string_type::size_type m_append_separator_if_needed();
        
        bool filename_is_dot() const;
        bool filename_is_dot_dot() const;
        string_type::size_type size() const { return m_pathname.size(); }
        path  parent_path() const;
    private:
        string_type m_pathname;
        string_type::size_type m_parent_path_end() const;
        
    };
    /*inline bool operator==(const path& lhs, const path& rhs)              {return lhs.compare(rhs) == 0;}
     inline bool operator==(const path& lhs, const path::string_type& rhs) {return lhs.compare(rhs) == 0;}
     inline bool operator==(const path::string_type& lhs, const path& rhs) {return rhs.compare(lhs) == 0;}
     inline bool operator==(const path& lhs, const path::value_type* rhs)  {return lhs.compare(rhs) == 0;}
     inline bool operator==(const path::value_type* lhs, const path& rhs)  {return rhs.compare(lhs) == 0;}
     
     inline bool operator!=(const path& lhs, const path& rhs)              {return lhs.compare(rhs) != 0;}
     inline bool operator!=(const path& lhs, const path::string_type& rhs) {return lhs.compare(rhs) != 0;}
     inline bool operator!=(const path::string_type& lhs, const path& rhs) {return rhs.compare(lhs) != 0;}
     inline bool operator!=(const path& lhs, const path::value_type* rhs)  {return lhs.compare(rhs) != 0;}
     inline bool operator!=(const path::value_type* lhs, const path& rhs)  {return rhs.compare(lhs) != 0;}
     
     // TODO: why do == and != have additional overloads, but the others don't?
     
     inline bool operator<(const path& lhs, const path& rhs)  {return lhs.compare(rhs) < 0;}
     inline bool operator<=(const path& lhs, const path& rhs) {return !(rhs < lhs);}
     inline bool operator> (const path& lhs, const path& rhs) {return rhs < lhs;}
     inline bool operator>=(const path& lhs, const path& rhs) {return !(lhs < rhs);}*/
    namespace detail
    {
        
        inline bool is_element_separator(path::value_type c)
        {
            return c == path::separator;
        }
    }
    inline path operator/(const path& lhs, const path& rhs) { return path(lhs) /= rhs; }
    inline bool is_directory_separator(path::value_type c)
    {
        return c == path::separator;
    }
    
    file_status status(const path&p, std::error_code* ec = 0);
    
    inline bool exists(file_status f)
    {
        return f.type() != status_error
        && f.type() != file_not_found;
    }
    inline bool exists(const path& p) { return exists(status(p)); }
    
    inline bool is_regular_file(file_status f)
    {
        return f.type() == regular_file;
    }
    inline bool is_directory(file_status f)
    {
        return f.type() == directory_file;
    }
    inline bool is_directory(const path& p) { return is_directory(status(p)); }
    inline bool is_regular_file(const path& p) { return is_regular_file(status(p)); }
    bool create_directories(const path& p, std::error_code* ec = 0);
    bool remove(const path& p, std::error_code* ec = 0);
    std::time_t last_write_time(const path& p, std::error_code* ec = 0);
    std::uintmax_t file_size(const path& p, std::error_code* ec = 0);
    std::uintmax_t remove_all(const path& p, std::error_code* ec = 0);
    inline bool path::filename_is_dot() const
    {
        // implicit dot is tricky, so actually call filename(); see path::filename() example
        // in reference.html
        path p(filename());
        return p.size() == 1 && *p.c_str() == dot;
    }
    
    inline bool path::filename_is_dot_dot() const
    {
        return size() >= 2 && m_pathname[size() - 1] == dot && m_pathname[size() - 2] == dot
        && (m_pathname.size() == 2 || detail::is_element_separator(m_pathname[size() - 3]));
        // use detail::is_element_separator() rather than detail::is_directory_separator
        // to deal with "c:.." edge case on Windows when ':' acts as a separator
    }
    //--------------------------------------------------------------------------------------//
    //                                                                                      //
    //                                 directory_entry                                      //
    //                                                                                      //
    //--------------------------------------------------------------------------------------//
    
    //  GCC has a problem with a member function named path within a namespace or
    //  sub-namespace that also has a class named path. The workaround is to always
    //  fully qualify the name path when it refers to the class name.
    
    class  directory_entry
    {
    public:
        typedef fs::path::value_type value_type;   // enables class path ctor taking directory_entry
        
        directory_entry() {}
        explicit directory_entry(const fs::path& p)
        : m_path(p), m_status(file_status()), m_symlink_status(file_status())
        {}
        directory_entry(const fs::path& p,
                        file_status st, file_status symlink_st = file_status())
        : m_path(p), m_status(st), m_symlink_status(symlink_st) {}
        
        directory_entry(const directory_entry& rhs)
        : m_path(rhs.m_path), m_status(rhs.m_status), m_symlink_status(rhs.m_symlink_status) {}
        
        directory_entry& operator=(const directory_entry& rhs)
        {
            m_path = rhs.m_path;
            m_status = rhs.m_status;
            m_symlink_status = rhs.m_symlink_status;
            return *this;
        }
        //  As of October 2015 the interaction between noexcept and =default is so troublesome
        //  for VC++, GCC, and probably other compilers, that =default is not used with noexcept
        //  functions. GCC is not even consistent for the same release on different platforms.
        
        /*#if !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)
         directory_entry(directory_entry&& rhs)
         {
         m_path = std::move(rhs.m_path);
         m_status = std::move(rhs.m_status);
         m_symlink_status = std::move(rhs.m_symlink_status);
         }
         directory_entry& operator=(directory_entry&& rhs)
         {
         m_path = std::move(rhs.m_path);
         m_status = std::move(rhs.m_status);
         m_symlink_status = std::move(rhs.m_symlink_status);
         return *this;
         }
         #endif*/
        
        void assign(const fs::path& p,file_status st = file_status(), file_status symlink_st = file_status())
        {
            m_path = p; m_status = st; m_symlink_status = symlink_st;
        }
        
        void replace_filename(const fs::path& p,file_status st = file_status(), file_status symlink_st = file_status())
        {
            m_path.remove_filename();
            m_path /= p;
            m_status = st;
            m_symlink_status = symlink_st;
        }
        
# ifndef BOOST_FILESYSTEM_NO_DEPRECATED
        void replace_leaf(const fs::path& p,file_status st, file_status symlink_st)
        {
            replace_filename(p, st, symlink_st);
        }
# endif
        
        const fs::path&  path() const { return m_path; }
        operator const fs::path&() const { return m_path; }
        file_status   status() const { return m_get_status(); }
        file_status   status(std::error_code& ec) const
        {
            return m_get_status(&ec);
        }
        file_status   symlink_status() const { return m_get_symlink_status(); }
        file_status   symlink_status(std::error_code& ec) const
        {
            return m_get_symlink_status(&ec);
        }
        
        /*bool operator==(const directory_entry& rhs) const  {return m_path == rhs.m_path; }
         bool operator!=(const directory_entry& rhs) const  {return m_path != rhs.m_path;}
         bool operator< (const directory_entry& rhs) const  {return m_path < rhs.m_path;}
         bool operator<=(const directory_entry& rhs) const  {return m_path <= rhs.m_path;}
         bool operator> (const directory_entry& rhs) const  {return m_path > rhs.m_path;}
         bool operator>=(const directory_entry& rhs) const  {return m_path >= rhs.m_path;}*/
        
    private:
        fs::path   m_path;
        mutable file_status       m_status;           // stat()-like
        mutable file_status       m_symlink_status;   // lstat()-like
        
        file_status m_get_status(std::error_code* ec = 0) const;
        file_status m_get_symlink_status(std::error_code* ec = 0) const;
    };
    // directory_entry
    //--------------------------------------------------------------------------------------//
    //                                                                                      //
    //                            directory_iterator helpers                                //
    //                                                                                      //
    //--------------------------------------------------------------------------------------//
    class directory_iterator;
    
    namespace detail
    {
        
        std::error_code dir_itr_close(void *& handle, void *& buffer);
        
        struct dir_itr_imp
        {
            directory_entry  dir_entry;
            void*            handle;
            void*            buffer;  // see dir_itr_increment implementation
            dir_itr_imp() : handle(0), buffer(0)
            {}
            
            ~dir_itr_imp() // never throws
            {
                dir_itr_close(handle, buffer);
            }
        };
        
        // see path::iterator: comment below
        void directory_iterator_construct(directory_iterator& it,
                                          const path& p, std::error_code* ec);
        void directory_iterator_increment(directory_iterator& it,
                                          std::error_code* ec);
        
    }  // namespace detail
    //--------------------------------------------------------------------------------------//
    //                                                                                      //
    //                                directory_iterator                                    //
    //                                                                                      //
    //--------------------------------------------------------------------------------------//
    
    class directory_iterator
    {
    public:
        
        directory_iterator() {}  // creates the "end" iterator
        
        // iterator_facade derived classes don't seem to like implementations in
        // separate translation unit dll's, so forward to detail functions
        explicit directory_iterator(const path& p)
        : m_imp(new detail::dir_itr_imp)
        {
            detail::directory_iterator_construct(*this, p, 0);
        }
        directory_iterator(const path& p, std::error_code& ec)
        : m_imp(new detail::dir_itr_imp)
        {
            detail::directory_iterator_construct(*this, p, &ec);
        }
        
        ~directory_iterator() {}
        directory_iterator& increment(std::error_code& ec)
        {
            detail::directory_iterator_increment(*this, &ec);
            return *this;
        }
        const directory_entry& operator*() const
        {
            return this->dereference();
        }
        const directory_entry * operator->() const
        {
            return std::addressof(this->dereference());
        }
        
        directory_iterator & operator++()
        {
            this->increment();
            return *this;
        }
        
        directory_iterator operator++(int)
        {
            directory_iterator temp = *this;
            this->operator++();
            return temp;
        }
        
        bool operator==(directory_iterator const& r)
        {
            return this->equal(r);
        }
        bool operator!=(directory_iterator const& r)
        {
            return !this->equal(r);
        }
    private:
        friend struct detail::dir_itr_imp;
        friend  void detail::directory_iterator_construct(directory_iterator& it,
                                                          const path& p, std::error_code* ec);
        friend  void detail::directory_iterator_increment(directory_iterator& it,
                                                          std::error_code* ec);
        
        // shared_ptr provides the shallow-copy semantics required for single pass iterators
        // (i.e. InputIterators). The end iterator is indicated by !m_imp || !m_imp->handle
        std::shared_ptr< detail::dir_itr_imp >  m_imp;
        
        //friend class boost::iterator_core_access;
        
        const directory_entry& dereference() const
        {
            assert(m_imp.get() && "attempt to dereference end iterator");
            return m_imp->dir_entry;
        }
        
        void increment() { detail::directory_iterator_increment(*this, 0); }
        
        bool equal(const directory_iterator& rhs) const
        {
            return m_imp == rhs.m_imp
            || (!m_imp && rhs.m_imp && !rhs.m_imp->handle)
            || (!rhs.m_imp && m_imp && !m_imp->handle);
        }
        
    };  // directory_iterator
}
#endif
//------------------------------------------------------------------------------


#endif //__JCFileSystem_H__

//-----------------------------END FILE--------------------------------
