#include "libplatform/impl.h"

namespace mp4v2 { namespace platform { namespace io {

///////////////////////////////////////////////////////////////////////////////

class StandardFileProvider : public FileProvider
{
public:
    StandardFileProvider();

    bool open( std::string name, Mode mode );
    bool seek( Size pos );
    bool read( void* buffer, Size size, Size& nin, Size maxChunkSize );
    bool write( const void* buffer, Size size, Size& nout, Size maxChunkSize );
    bool close();

    int64_t getSize();

private:
    bool         _seekg;
    bool         _seekp;
    std::fstream _fstream;
    std::string  _name;
};

StandardFileProvider* file_handle;

///////////////////////////////////////////////////////////////////////////////

StandardFileProvider::StandardFileProvider()
    : _seekg ( false )
    , _seekp ( false )
{
	//printf("standard init over \r\n");
}

bool
StandardFileProvider::open( std::string name, Mode mode )
{
    ios::openmode om = ios::binary;
    switch( mode ) {
        case MODE_UNDEFINED:
        case MODE_READ:
        default:
            om |= ios::in;
            _seekg = true;
            _seekp = false;
            break;

        case MODE_MODIFY:
            om |= ios::in | ios::out;
            _seekg = true;
            _seekp = true;
            break;

        case MODE_CREATE:
            om |= ios::in | ios::out | ios::trunc;
            _seekg = true;
            _seekp = true;
            break;
    }
	
    _fstream.open( name.c_str(), om );
	
    _name = name;
    return _fstream.fail();
}

bool
StandardFileProvider::seek( Size pos )
{
    if( _seekg )
        _fstream.seekg( pos, ios::beg );
    if( _seekp )
        _fstream.seekp( pos, ios::beg );
    return _fstream.fail();
}

bool
StandardFileProvider::read( void* buffer, Size size, Size& nin, Size maxChunkSize )
{
    _fstream.read( (char*)buffer, size );
    if( _fstream.fail() )
        return true;
    nin = _fstream.gcount();
    return false;
}

bool
StandardFileProvider::write( const void* buffer, Size size, Size& nout, Size maxChunkSize )
{
    _fstream.write( (const char*)buffer, size );
    if( _fstream.fail() )
        return true;
    nout = size;
    return false;
}

bool
StandardFileProvider::close()
{
    _fstream.close();
    return _fstream.fail();
}

int64_t StandardFileProvider::getSize()
{
   int64_t retSize = 0;
   FileSystem::getFileSize( _name, retSize );
   return retSize;
}

///////////////////////////////////////////////////////////////////////////////

FileProvider&
FileProvider::standard()
{
	file_handle = new StandardFileProvider();
}

bool
FileProvider::_open( std::string name, Mode mode )
{
	//printf("FILE::open\r\n", __FUNCTION__, __LINE__);
	return file_handle->open( name, mode);
}

bool
FileProvider::_read( void* buffer, int64_t size, int64_t& nin, int64_t maxChunkSize )
{
	return file_handle->read( buffer,size, nin, maxChunkSize);
}

bool
FileProvider::_write( const void* buffer, int64_t size, int64_t& nout, int64_t maxChunkSize )
{
	return file_handle->write( buffer,size, nout, maxChunkSize);
}

bool
FileProvider::_seek( int64_t pos )
{
	return file_handle->seek( pos );
}

bool
FileProvider::_close()
{
	bool result = file_handle->close();
	delete file_handle;
	return result;
}

int64_t
FileProvider::_getSize()
{
	return file_handle->getSize();
}
///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::io
