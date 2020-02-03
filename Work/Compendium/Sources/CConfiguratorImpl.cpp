/*
© 2019, Serge Page.

This license is hereby grants to any person who obtained a copy of this product or the product source files the next rights to:

- Use a compiled version of this product at no cost, without any restrictions, in non-commercial and commercial purposes
- Do reverse-engineering of this product in non-commercial purposes only
- Use source codes of this product at no cost but with the limitations - source codes available only for non-commercial, academic and / or scientific purposes
- Copy and distribute without any fee
- Copy of the original repository and / or create own derivative product for non-commercial,  academic and / or scientific purposes only
- Link the product source code with an another product source code which licensed under any of Dark Orb licenses or one of these licenses:
  - MIT License
  - Microsoft Public License
  - Beerware License
  - Academic Free License
  - WTFPL
  - Unlicense
  - Original BSD license
  - Modified BSD License
  - Simplified BSD License
  - Zero Clause BSD
- Link the product source code with an another product source code if between them no any patent collision

This license is require to:

- Keep the full license text without any changes
- The license text must be included once in a file called 'License' which placed in the root directory of the product and in all source files of the product

This license is deny to:

- Change license of the derivative product
- Use the product’s author name and name of any contributor of this product for advertising derivative software without legally certified permission
- Resell this product
- Use the product or the product source code for any purpose which refers to any government of any country

The product is an original source codes and original compiled files which made by the original author and provided only under the grants and restrictions of this license. All damages which can be happen after and while using the product will not be compensate.
*/

#include "Compendium.hpp"

using namespace Compendium;

#include "Unicoder.hpp"

using namespace Unicoder;

CConfigurator::CConfigurator() noexcept:
  VBuffer( U"" ) {}

CConfigurator::~CConfigurator() {
  FClearBuffer();

  VUnits.clear();
  VGroups.clear();
}

ENErrorCodes CConfigurator::FLoadBufferFromFile( const u32string &_Path ) {
  if( _Path.empty() ||
      !exists( _Path ) || !directory_entry( _Path ).is_regular_file() )
    return ENErrorCodes::EC_INVALID_PATH;

  if( !FIsCompendiumFile( _Path ) )
    return ENErrorCodes::EC_NOT_COMPENDIUM_FILE;

  FClearBuffer();

  basic_ifstream<char32_t> VFile( _Path, ios::binary );

  VFile.seekg( 0, ios::end );
  const streampos VFileLength = VFile.tellg();
  VFile.seekg( 0 );

  if( VFileLength == 0 ) {
    VFile.close();

    return ENErrorCodes::EC_EMPTY_FILE;
  }

  char32_t *VTemporaryBuffer = new char32_t [ static_cast< uint32_t >( VFileLength ) + 1 ];
  VTemporaryBuffer [ static_cast< uint32_t >( VFileLength ) ] = U'\0';

  VFile.read( VTemporaryBuffer, VFileLength );

  VFile.close();

  if( char_traits<char32_t>::length( VTemporaryBuffer ) == 0 ) {
    delete[] VTemporaryBuffer;
    VTemporaryBuffer = nullptr;

    return ENErrorCodes::EC_INVALID_READ;
  }

  VBuffer = move( VTemporaryBuffer );

  delete[] VTemporaryBuffer;
  VTemporaryBuffer = nullptr;

  return ENErrorCodes::EC_OK;
}

ENErrorCodes CConfigurator::FLoadBufferFromBuffer( const u32string &_Buffer ) {
  if( _Buffer.empty() )
    return ENErrorCodes::EC_INVALID_ARGUMENT;

  FClearBuffer();

  const uint32_t VSourceSize = static_cast< uint32_t >( _Buffer.length() + 1 );

  char32_t *VTemporaryBuffer = new char32_t [ VSourceSize ];
  VTemporaryBuffer [ VSourceSize ] = U'\0';

  for( uint32_t c = 0; c < VSourceSize - 1; c++ ) {
    VTemporaryBuffer [ c ] = _Buffer [ c ];
  }

  if( char_traits<char32_t>::length( VTemporaryBuffer ) == 0 ) {
    delete[] VTemporaryBuffer;
    VTemporaryBuffer = nullptr;

    return ENErrorCodes::EC_INVALID_READ;
  }

  VBuffer = move( VTemporaryBuffer );

  delete[] VTemporaryBuffer;
  VTemporaryBuffer = nullptr;

  return ENErrorCodes::EC_OK;
}

ENErrorCodes CConfigurator::FLoadConfigurationFromFile( const u32string &_Path ) {
  if( _Path.empty() ||
      !exists( _Path ) || !directory_entry( _Path ).is_regular_file() )
    return ENErrorCodes::EC_INVALID_PATH;

  if( !FIsCompendiumFile( _Path ) )
    return ENErrorCodes::EC_NOT_COMPENDIUM_FILE;

  FClearBuffer();

  basic_ifstream<char32_t> VFile( _Path, ios::binary );

  VFile.seekg( 0, ios::end );
  const streampos VFileLength = VFile.tellg();
  VFile.seekg( 0 );

  if( VFileLength == 0 ) {
    VFile.close();

    return ENErrorCodes::EC_EMPTY_FILE;
  }

  char32_t *VTemporaryBuffer = new char32_t [ static_cast< uint32_t >( VFileLength ) + 1 ];
  VTemporaryBuffer [ static_cast< uint32_t >( VFileLength ) ] = U'\0';

  VFile.read( VTemporaryBuffer, VFileLength );

  VFile.close();

  if( char_traits<char32_t>::length( VTemporaryBuffer ) == 0 ) {
    delete[] VTemporaryBuffer;
    VTemporaryBuffer = nullptr;

    return ENErrorCodes::EC_INVALID_READ;
  }

  VBuffer = move( VTemporaryBuffer );

  delete[] VTemporaryBuffer;
  VTemporaryBuffer = nullptr;

  for( uint32_t c = 0; c < VBuffer.length(); c++ ) {
    if( VBuffer [ c ] == U'<' && VBuffer [ c + 1 ] == U'-' ) {
      c += 2;
      FSkipComment( c );
    }

    switch( FGetNextKeyword( c ) ) {
      case static_cast< int64_t >( ENKeywords::K_GROUP ) :
      {
        CGroup *VGetGroup = FParseGroup( c );

        if( VGetGroup != nullptr )
          VGroups.push_back( VGetGroup );
        else
          return ENErrorCodes::EC_INVALID_PARSE;
      }
      break;

      case static_cast< int64_t >( ENKeywords::K_UNIT ) :
      {
        CUnit *VGetUnit = FParseUnit( c );

        if( VGetUnit != nullptr )
          VUnits.push_back( VGetUnit );
        else
          return ENErrorCodes::EC_INVALID_PARSE;
      }
      break;
    }
  }

  FClearBuffer();

  return ENErrorCodes::EC_OK;
}

ENErrorCodes CConfigurator::FLoadConfigurationFromBuffer( const u32string &_Buffer, const uint32_t _StartIndex ) {
  if( _Buffer.empty() )
    return ENErrorCodes::EC_INVALID_ARGUMENT;

  if( _StartIndex >= _Buffer.length() )
    return ENErrorCodes::EC_INVALID_START_INDEX;

  FClearBuffer();

  const uint32_t VSourceSize = static_cast< uint32_t >( _Buffer.length() + 1 );

  char32_t *VTemporaryBuffer = new char32_t [ VSourceSize ];
  VTemporaryBuffer [ VSourceSize ] = U'\0';

  for( uint32_t c = 0; c < VSourceSize - 1; c++ ) {
    VTemporaryBuffer [ c ] = _Buffer [ c ];
  }

  if( char_traits<char32_t>::length( VTemporaryBuffer ) == 0 ) {
    delete[] VTemporaryBuffer;
    VTemporaryBuffer = nullptr;

    return ENErrorCodes::EC_INVALID_READ;
  }

  VBuffer = VTemporaryBuffer;

  delete[] VTemporaryBuffer;
  VTemporaryBuffer = nullptr;

  for( uint32_t c = _StartIndex; c < VBuffer.length(); c++ ) {
    if( VBuffer [ c ] == U'<' && VBuffer [ c + 1 ] == U'-' ) {
      c += 2;
      FSkipComment( c );
    }

    switch( FGetNextKeyword( c ) ) {
      case static_cast< int64_t >( ENKeywords::K_GROUP ) :
      {
        CGroup *VGetGroup = FParseGroup( c );

        if( VGetGroup != nullptr )
          VGroups.push_back( VGetGroup );
        else
          return ENErrorCodes::EC_INVALID_PARSE;
      }
      break;

      case static_cast< int64_t >( ENKeywords::K_UNIT ) :
      {
        CUnit *VGetUnit = FParseUnit( c );

        if( VGetUnit != nullptr )
          VUnits.push_back( VGetUnit );
        else
          return ENErrorCodes::EC_INVALID_PARSE;
      }
      break;
    }
  }

  FClearBuffer();

  return ENErrorCodes::EC_OK;
}

ENErrorCodes CConfigurator::FSaveConfigurationToFile( const u32string &_Path ) {
  if( _Path.empty() || directory_entry( _Path + U".cconf" ).is_directory() )
    return ENErrorCodes::EC_INVALID_PATH;

  u32string VData;

  for( const CUnit *VUnit : VUnits )
    VData += FSerializeUnit( VUnit );

  for( const CGroup *VGroup : VGroups )
    VData += FSerializeGroup( VGroup, 0 );

  basic_ofstream<char32_t> VFile( _Path + U".cconf", ios::binary | ios::trunc );

  VFile.write( VData.c_str(), VData.length() );

  VFile.close();

  VData.clear();

  return ENErrorCodes::EC_OK;
}

ENErrorCodes CConfigurator::FSaveConfigurationToBuffer( u32string &_Buffer ) {
  u32string VData;

  for( const CUnit *VUnit : VUnits )
    VData += FSerializeUnit( VUnit );

  for( const CGroup *VGroup : VGroups )
    VData += FSerializeGroup( VGroup, 0 );

  _Buffer = move( VData );

  return ENErrorCodes::EC_OK;
}

bool CConfigurator::FIsCompendiumFile( const u32string &_Path ) const noexcept {
  const u32string VGetExtension = path( UConverter::FConvert<u32string, char32_t, wstring, wchar_t>( _Path ) ).extension().u32string();

  if( VGetExtension == VDefaultExtension )
    return true;

  return false;
}

void CConfigurator::FSkipComment( uint32_t &_Index ) noexcept {
  for( uint32_t c = _Index + 1; c < VBuffer.length(); c++ ) {
    if( VBuffer [ c ] == U'-' && VBuffer [ c + 1 ] == U'>' ) {
      _Index = c + 2;
      return;
    }
  }
}

int64_t CConfigurator::FGetNextKeyword( uint32_t &_Index ) const noexcept {
  if( VBuffer [ _Index ] == U'g' &&
      VBuffer [ _Index + 1 ] == U'r' &&
      VBuffer [ _Index + 2 ] == U'o' &&
      VBuffer [ _Index + 3 ] == U'u' &&
      VBuffer [ _Index + 4 ] == U'p' ) {
    _Index += 5;

    return static_cast< int64_t >( ENKeywords::K_GROUP );
  }

  if( VBuffer [ _Index ] == U'u' &&
      VBuffer [ _Index + 1 ] == U'n' &&
      VBuffer [ _Index + 2 ] == U'i' &&
      VBuffer [ _Index + 3 ] == U't' ) {
    _Index += 4;

    return static_cast< int64_t >( ENKeywords::K_UNIT );
  }

  return static_cast< int64_t >( ENErrorCodes::EC_NOT_FOUND_KEYWORD );
}

CGroup *CConfigurator::FParseGroup( uint32_t &_Index ) {
  if( VBuffer [ _Index ] != U' ' )
    return nullptr;

  u32string VId;
  vector<CGroup *> VInnerGroups;
  vector<CUnit *> VInnerUnits;

  uint32_t c = _Index + 1;

  for( ; c < VBuffer.length(); c++ ) {
    if( VBuffer [ c ] == U'<' && VBuffer [ c + 1 ] == U'-' ) {
      c += 2;
      FSkipComment( c );
    }

    if( VBuffer [ c ] == U' ' ) {
      ++c;
      break;
    } else
      VId += VBuffer [ c ];
  }

  if( VId.empty() )
    return nullptr;

  if( VBuffer [ c ] != U'[' ) {
    VId.clear();

    return nullptr;
  }

  ++c;

  for( ; c < VBuffer.length(); c++ ) {
    if( VBuffer [ c ] == U'<' && VBuffer [ c + 1 ] == U'-' ) {
      c += 2;
      FSkipComment( c );
    }

    if( VBuffer [ c ] == U']' )
      break;

    switch( FGetNextKeyword( c ) ) {
      case static_cast< int64_t >( ENKeywords::K_GROUP ) :
      {
        CGroup *VGetGroup = FParseGroup( c );

        if( VGetGroup != nullptr )
          VInnerGroups.push_back( VGetGroup );
        else {
          VId.clear();

          VInnerGroups.clear();
          VInnerUnits.clear();

          return nullptr;
        }
      }
      break;

      case static_cast< int64_t >( ENKeywords::K_UNIT ) :
      {
        CUnit *VGetUnit = FParseUnit( c );

        if( VGetUnit != nullptr )
          VInnerUnits.push_back( VGetUnit );
        else {
          VId.clear();

          VInnerGroups.clear();
          VInnerUnits.clear();

          return nullptr;
        }
      }
      break;
    }
  }

  _Index = c;

  return new CGroup( VId.c_str(), VInnerGroups, VInnerUnits );
}

CUnit *CConfigurator::FParseUnit( uint32_t &_Index ) {
  if( VBuffer [ _Index ] != U' ' )
    return nullptr;

  u32string VId, VValue;

  uint32_t c = _Index + 1;

  for( ; c < VBuffer.length(); c++ ) {
    if( VBuffer [ c ] == U':' ) {
      ++c;
      break;
    } else
      VId += VBuffer [ c ];
  }

  if( VId.empty() )
    return nullptr;

  for( ; c < VBuffer.length(); c++ ) {
    if( VBuffer [ c ] == U'\n' )
      break;
    else if( VBuffer [ c ] == U'\r' && VBuffer [ c + 1 ] == U'\n' ) {
      ++c;
      break;
    } else
      VValue += VBuffer [ c ];
  }

  if( VValue.length() == 0 ) {
    VId.clear();

    return nullptr;
  }

  _Index = c;

  return new CUnit( VId.c_str(), VValue.c_str() );
}

u32string CConfigurator::FSerializeGroup( const CGroup *_Group, const uint32_t _Level ) const {
  u32string VResult;

  for( uint32_t c = 0; c < _Level; c++ )
    VResult += U" ";

  VResult += U"group ";
  VResult += _Group->FGetId();
  VResult += U" [\r\n";

  for( const CUnit *VUnit : _Group->FGetUnits() ) {
    for( uint32_t c = 0; c < _Level + 1; c++ )
      VResult += U" ";

    VResult += FSerializeUnit( VUnit );
  }

  for( const CGroup *VGroup : _Group->FGetGroups() ) {
    VResult += FSerializeGroup( VGroup, _Level + 1 );
  }

  for( uint32_t c = 0; c < _Level; c++ )
    VResult += U" ";

  VResult += U"]\r\n\r\n";

  return VResult;
}

u32string CConfigurator::FSerializeUnit( const CUnit *_Unit ) const {
  return U"unit " + _Unit->FGetId() + U":" + _Unit->FGetValue() + U"\r\n";
}
