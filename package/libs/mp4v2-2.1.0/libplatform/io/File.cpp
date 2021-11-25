#include "libplatform/impl.h"
#include "src/impl.h"

namespace mp4v2 { namespace platform { namespace io {

///////////////////////////////////////////////////////////////////////////////

namespace {
    const File::Size __maxChunkSize = 1024*1024;
}

///////////////////////////////////////////////////////////////////////////////

File::File( std::string name_, Mode mode_, FileProvider* provider_ )
    : _name     ( name_ )
    , _isOpen   ( false )
    , _mode     ( mode_ )
    , _size     ( 0 )
    , _position ( 0 )
    , _provider ( provider_ ? *provider_ : standard() )
    , name      ( _name )
    , isOpen    ( _isOpen )
    , mode      ( _mode )
    , size      ( _size )
    , position  ( _position )
{
}

///////////////////////////////////////////////////////////////////////////////

File::~File()
{
    close();
}

///////////////////////////////////////////////////////////////////////////////

void
File::setMode( Mode mode_ )
{
    _mode = mode_;
}

void
File::setName( const std::string& name_ )
{
    _name = name_;
}

///////////////////////////////////////////////////////////////////////////////

bool
File::open( std::string name_, Mode mode_ )
{
    if( _isOpen )
        return true;

    if( !name_.empty() )
        setName( name_ );
    if( mode_ != MODE_UNDEFINED )
        setMode( mode_ );
	mp4v2::impl::log.printf(MP4_LOG_INFO, "FILE::open\r\n");
    if( _open( _name, _mode ))
        return true;
	
	mp4v2::impl::log.printf(MP4_LOG_INFO, "FILE::open over, file name [%s]\r\n", _name.c_str());

	struct stat buf;
    if (stat(_name.c_str(), &buf) < 0) {
 		_size = 0;
    } else {
		_size = buf.st_size;
	}
	
	mp4v2::impl::log.printf(MP4_LOG_INFO, "FILE::file _size = %ld, buf.st_size = %lld \r\n", _size, buf.st_size);

    _isOpen = true;
    return false;
}

bool
File::seek( Size pos )
{
    if( !_isOpen )
        return true;

    if( _seek( pos ))
        return true;
    _position = pos;
    return false;
}

bool
File::read( void* buffer, Size size, Size& nin, Size maxChunkSize )
{
    nin = 0;

    if( !_isOpen )
        return true;

    if( _read( buffer, size, nin, maxChunkSize ))
        return true;

    _position += nin;
    if( _position > _size )
        _size = _position;

    return false;
}

bool
File::write( const void* buffer, Size size, Size& nout, Size maxChunkSize )
{
    nout = 0;

    if( !_isOpen )
        return true;

    if( _write( buffer, size, nout, maxChunkSize ))
        return true;

    _position += nout;
    if( _position > _size )
        _size = _position;

    return false;
}

bool
File::close()
{
    if( !_isOpen )
        return false;
    if( _close() )
        return true;

    _isOpen = false;
    return false;
}

int64_t File::getSize()
{
   int64_t retSize = 0;
   struct stat buf;
   if (stat(_name.c_str(), &buf)) {
	return 0;
   }
   retSize = buf.st_size;

   _size = retSize;
   
   //printf("FILE::getSize [%d] [%d] [%ld]\r\n",buf.st_size, retSize, _size);
   return 0;
}

///////////////////////////////////////////////////////////////////////////////

CustomFileProvider::CustomFileProvider( const MP4FileProvider& provider )
    : _handle( NULL )
{
    memcpy( &_call, &provider, sizeof(MP4FileProvider) );
}

bool
CustomFileProvider::open( std::string name, Mode mode )
{
    MP4FileMode fm;
    switch( mode ) {
        case MODE_READ:   fm = FILEMODE_READ;   break;
        case MODE_MODIFY: fm = FILEMODE_MODIFY; break;
        case MODE_CREATE: fm = FILEMODE_CREATE; break;

        case MODE_UNDEFINED:
        default:
            fm = FILEMODE_UNDEFINED;
            break;
    }

    _handle = _call.open( name.c_str(), fm );
    return _handle == NULL;
}

bool
CustomFileProvider::seek( Size pos )
{
    return _call.seek( _handle, pos );
}

bool
CustomFileProvider::read( void* buffer, Size size, Size& nin, Size maxChunkSize )
{
    return _call.read( _handle, buffer, size, &nin, maxChunkSize );
}

bool
CustomFileProvider::write( const void* buffer, Size size, Size& nout, Size maxChunkSize )
{
    return _call.write( _handle, buffer, size, &nout, maxChunkSize );
}

bool
CustomFileProvider::close()
{
    return _call.close( _handle );
}

int64_t CustomFileProvider::getSize()
{
   assert( _call.size );
   return _call.size( _handle );
}

///////////////////////////////////////////////////////////////////////////////

}}} // namespace mp4v2::platform::io
