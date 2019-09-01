/*
© 2019, Dark Orb.

The license version - 1.0

This license is hereby grants to any person who obtained a copy of this software the next rights to:
1. Use and do reverse-engineering of compiled version of this software at no cost, without any restrictions, in non-commercial and commercial purposes
2. Use source codes of this software at no cost but with the limitations - source codes available only for non-commercial, academic and / or scientific purposes
3. Copy and distribute without any fee
4. Create a copy of the exists repository with this software for non-commercial,  academic and / or scientific purposes only

This license is require to:
1. Keep the full license text without any changes
2. The license text must be included once in a file called 'License' which placed in the root directory of the software and in all source files of the software

This license is deny to:
1. Change license of the derivative software
2. Use the copyright holder name and name of any contributor of this software for advertising derivative software without legally certified permission
3. Sell this software without an author legally certified permission 

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Compendium.hpp"

using namespace Compendium;

CConfigurator::CConfigurator() noexcept:
  vBuffer( nullptr ) {}

CConfigurator::~CConfigurator() {
  fClearBuffer();

  vUnits.clear();
  vGroups.clear();
}

enErrorCode CConfigurator::fLoadBuffer( const wstring _Path ) {
  if( _Path.empty() ||
      !exists( _Path ) || !directory_entry( _Path ).is_regular_file() )
    return EC_INVALID_PATH;

  if( !fIsCompendiumFile( _Path ) )
    return EC_NOT_COMPENDIUM_FILE;

  fClearBuffer();

  wifstream vFile( _Path, ios::binary );

  vFile.seekg( 0, ios::end );
  streampos vFileLength = vFile.tellg();
  vFile.seekg( 0 );

  if( vFileLength == 0 ) {
    vFile.close();

    return EC_EMPTY_FILE;
  }

  vBuffer = new wchar_t [ static_cast< uint32_t >( vFileLength ) + 1 ];

  vFile.read( vBuffer, vFileLength );

  vFile.close();

  if( wcslen( vBuffer ) == 0 ) {
    fClearBuffer();

    return EC_INVALID_READ;
  }

  return EC_OK;
}

enErrorCode CConfigurator::fLoadBuffer( const wchar_t *_Buffer ) {
  if( _Buffer == nullptr )
    return EC_INVALID_ARGUMENT;

  fClearBuffer();

  size_t vSourceSize = wcslen( _Buffer ) + 1;

  vBuffer = new wchar_t [ vSourceSize ];

  wcscpy_s( vBuffer, vSourceSize, _Buffer );

  if( wcslen( vBuffer ) == 0 ) {
    fClearBuffer();

    return EC_INVALID_READ;
  }

  return EC_OK;
}

enErrorCode CConfigurator::fLoadConfiguration( const wstring _Path ) {
  if( _Path.empty() ||
      !exists( _Path ) || !directory_entry( _Path ).is_regular_file() )
    return EC_INVALID_PATH;

  if( !fIsCompendiumFile( _Path ) )
    return EC_NOT_COMPENDIUM_FILE;

  fClearBuffer();

  wifstream vFile( _Path, ios::binary );

  vFile.seekg( 0, ios::end );
  streampos vFileLength = vFile.tellg();
  vFile.seekg( 0 );

  if( vFileLength == 0 ) {
    vFile.close();

    return EC_EMPTY_FILE;
  }

  vBuffer = new wchar_t [ static_cast< uint32_t >( vFileLength ) + 1 ];

  vFile.read( vBuffer, vFileLength );

  vFile.close();

  if( wcslen( vBuffer ) == 0 ) {
    fClearBuffer();

    return EC_INVALID_READ;
  }

  for( size_t c = 0; c < wcslen( vBuffer ); c++ ) {
    if( vBuffer [ c ] == L'<' && vBuffer [ c + 1 ] == L'-' ) {
      c += 2;
      fSkipComment( c );
    }

    switch( fGetNextKeyword( c ) ) {
      case K_GROUP:
      {
        CGroup *vGetGroup = fParseGroup( c );

        if( vGetGroup != nullptr )
          vGroups.push_back( vGetGroup );
        else
          return EC_INVALID_PARSE;
      }
      break;

      case K_UNIT:
      {
        CUnit *vGetUnit = fParseUnit( c );

        if( vGetUnit != nullptr )
          vUnits.push_back( vGetUnit );
        else
          return EC_INVALID_PARSE;
      }
      break;
    }
  }

  fClearBuffer();

  return EC_OK;
}

enErrorCode CConfigurator::fLoadConfiguration( const wchar_t *_Buffer, const size_t _StartIndex ) {
  if( _Buffer == nullptr )
    return EC_INVALID_ARGUMENT;

  if( _StartIndex >= wcslen( _Buffer ) )
    return EC_INVALID_START_INDEX;

  fClearBuffer();

  size_t vSourceSize = wcslen( _Buffer ) + 1;

  vBuffer = new wchar_t [ vSourceSize ];

  wcscpy_s( vBuffer, vSourceSize, _Buffer );

  if( wcslen( vBuffer ) == 0 ) {
    fClearBuffer();

    return EC_INVALID_READ;
  }

  for( size_t c = _StartIndex; c < wcslen( vBuffer ); c++ ) {
    if( vBuffer [ c ] == L'<' && vBuffer [ c + 1 ] == L'-' ) {
      c += 2;
      fSkipComment( c );
    }

    switch( fGetNextKeyword( c ) ) {
      case K_GROUP:
      {
        CGroup *vGetGroup = fParseGroup( c );

        if( vGetGroup != nullptr )
          vGroups.push_back( vGetGroup );
        else
          return EC_INVALID_PARSE;
      }
      break;

      case K_UNIT:
      {
        CUnit *vGetUnit = fParseUnit( c );

        if( vGetUnit != nullptr )
          vUnits.push_back( vGetUnit );
        else
          return EC_INVALID_PARSE;
      }
      break;
    }
  }

  fClearBuffer();

  return EC_OK;
}

enErrorCode CConfigurator::fSaveConfiguration( const wstring _Path ) {
  if( _Path.empty() || directory_entry( _Path + L".cconf" ).is_directory() )
    return EC_INVALID_PATH;

  wstring vData;

  for( const CUnit *vUnit : vUnits )
    vData += fSerializeUnit( vUnit );

  for( const CGroup *vGroup : vGroups )
    vData += fSerializeGroup( vGroup, 0 );

  wofstream vFile( _Path + L".cconf", ios::binary | ios::trunc );

  vFile.write( vData.c_str(), vData.length() );

  vFile.close();

  vData.clear();

  return EC_OK;
}

enErrorCode CConfigurator::fSaveBuffer( wstring &_Buffer ) {
  wstring vData;

  for( const CUnit *vUnit : vUnits )
    vData += fSerializeUnit( vUnit );

  for( const CGroup *vGroup : vGroups )
    vData += fSerializeGroup( vGroup, 0 );

  _Buffer = vData;

  vData.clear();

  return EC_OK;
}

long CConfigurator::fGetNextKeyword( size_t &_Index ) const noexcept {
  if( vBuffer [ _Index ] == L'g' &&
      vBuffer [ _Index + 1 ] == L'r' &&
      vBuffer [ _Index + 2 ] == L'o' &&
      vBuffer [ _Index + 3 ] == L'u' &&
      vBuffer [ _Index + 4 ] == L'p' ) {
    _Index += 5;

    return K_GROUP;
  }

  if( vBuffer [ _Index ] == L'u' &&
      vBuffer [ _Index + 1 ] == L'n' &&
      vBuffer [ _Index + 2 ] == L'i' &&
      vBuffer [ _Index + 3 ] == L't' ) {
    _Index += 4;

    return K_UNIT;
  }

  return EC_NOT_FOUND_KEYWORD;
}

CGroup *CConfigurator::fParseGroup( size_t &_Index ) {
  if( vBuffer [ _Index ] != L' ' )
    return nullptr;

  wstring vId;
  vector<CGroup *> vInnerGroups;
  vector<CUnit *> vInnerUnits;

  size_t c = _Index + 1;

  for( ; c < wcslen( vBuffer ); c++ ) {
    if( vBuffer [ c ] == L'<' && vBuffer [ c + 1 ] == L'-' ) {
      c += 2;
      fSkipComment( c );
    }

    if( vBuffer [ c ] == L' ' ) {
      ++c;
      break;
    }
    else
      vId += vBuffer [ c ];
  }

  if( vId.empty() )
    return nullptr;

  if( vBuffer [ c ] != L'[' ) {
    vId.clear();

    return nullptr;
  }

  ++c;

  for( ; c < wcslen( vBuffer ); c++ ) {
    if( vBuffer [ c ] == L'<' && vBuffer [ c + 1 ] == L'-' ) {
      c += 2;
      fSkipComment( c );
    }

    if( vBuffer [ c ] == L']' )
      break;

    switch( fGetNextKeyword( c ) ) {
      case K_GROUP:
      {
        CGroup *vGetGroup = fParseGroup( c );

        if( vGetGroup != nullptr )
          vInnerGroups.push_back( vGetGroup );
        else {
          vId.clear();

          vInnerGroups.clear();
          vInnerUnits.clear();

          return nullptr;
        }
      }
      break;

      case K_UNIT:
      {
        CUnit *vGetUnit = fParseUnit( c );

        if( vGetUnit != nullptr )
          vInnerUnits.push_back( vGetUnit );
        else {
          vId.clear();

          vInnerGroups.clear();
          vInnerUnits.clear();

          return nullptr;
        }
      }
      break;
    }
  }

  _Index = c;

  return new CGroup( vId.c_str(), vInnerGroups, vInnerUnits );
}

CUnit *CConfigurator::fParseUnit( size_t &_Index ) {
  if( vBuffer [ _Index ] != L' ' )
    return nullptr;

  wstring vId, vValue;

  size_t c = _Index + 1;

  for( ; c < wcslen( vBuffer ); c++ ) {
    if( vBuffer [ c ] == L':' ) {
      ++c;
      break;
    }
    else
      vId += vBuffer [ c ];
  }

  if( vId.empty() )
    return nullptr;

  for( ; c < wcslen( vBuffer ); c++ ) {
    if( vBuffer [ c ] == L'\n' )
      break;
    else if( vBuffer [ c ] == L'\r' && vBuffer [ c + 1 ] == L'\n' ) {
      ++c;
      break;
    }
    else
      vValue += vBuffer [ c ];
  }

  if( vValue.length() == 0 ) {
    vId.clear();

    return nullptr;
  }

  _Index = c;

  return new CUnit( vId.c_str(), vValue.c_str() );
}

wstring CConfigurator::fSerializeGroup( const CGroup *_Group, const size_t _Level ) const {
  wstring vResult;

  for( size_t c = 0; c < _Level; c++ )
    vResult += L" ";

  vResult += L"group ";
  vResult += _Group->fGetId();
  vResult += L" [\n";

  for( const CUnit *vUnit : _Group->fGetUnits() ) {
    for( size_t c = 0; c < _Level + 1; c++ )
      vResult += L" ";

    vResult += fSerializeUnit( vUnit );
  }

  for( const CGroup *vGroup : _Group->fGetGroups() ) {
    vResult += fSerializeGroup( vGroup, _Level + 1 );
  }

  for( size_t c = 0; c < _Level; c++ )
    vResult += L" ";

  vResult += L"]\n\n";

  return vResult;
}
