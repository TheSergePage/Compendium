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

#include <Compendium.hpp>

using namespace Compendium;

#include "Unicoder.hpp"

using namespace Unicoder;

using std::wcscmp;

#include <iostream>

using std::wcout;
using std::endl;

using std::move;

using std::wstring;
using std::to_wstring;

void FInsertAlign( const uint64_t _Level ) {
  for( uint64_t c = 0; c < _Level; c++ )
    wcout << L' ';
}

void FPrintMessage( const u32string &_Text, const bool _NewLine = true, const bool _DoubleNewLine = false, const bool _OnlyNewLine = false ) {
  if( _OnlyNewLine ) {
    wcout << endl;

    return;
  }

  if( _NewLine ) {
    if( _DoubleNewLine ) {
      wcout << UConverter::FConvert<u32string, char32_t, wstring, wchar_t>( _Text ) << endl << endl;
    } else {
      wcout << UConverter::FConvert<u32string, char32_t, wstring, wchar_t>( _Text ) << endl;
    }
  } else {
    wcout << UConverter::FConvert<u32string, char32_t, wstring, wchar_t>( _Text );
  }
}

void FShowUnit( const CUnit *_Unit, const uint64_t _Level ) {
  FInsertAlign( _Level );
  FPrintMessage( U"Unit id - " + _Unit->FGetId() );

  FInsertAlign( _Level );
  FPrintMessage( U"Unit Value - " + _Unit->FGetValue(), true, true );
}

void FShowGroup( const CGroup *_Group, const uint64_t _Level ) {
  FInsertAlign( _Level );
  FPrintMessage( U"Group id - " + _Group->FGetId() );

  FInsertAlign( _Level );
  FPrintMessage( U"-----------" );

  FInsertAlign( _Level );
  FPrintMessage( U"Inner units:", false );

  if( _Group->FGetUnits().size() == 0 )
    FPrintMessage( U" No units" );
  else {
    FPrintMessage( U"", false, false, true );

    for( const CUnit *VUnit : _Group->FGetUnits() )
      FShowUnit( VUnit, _Level + 1 );
  }

  FInsertAlign( _Level );
  FPrintMessage( U"-----------" );

  FInsertAlign( _Level );
  FPrintMessage( U"Inner groups:", false );

  if( _Group->FGetGroups().size() == 0 )
    FPrintMessage( U" No groups" );
  else {
    FPrintMessage( U"", false, false, true );

    for( const CGroup *VGroup : _Group->FGetGroups() )
      FShowGroup( VGroup, _Level + 1 );
  }
}

void FShowHowToUse() {
  FPrintMessage( U"How to use:" );
  FPrintMessage( U"!Command - command[View]" );
  FPrintMessage( U"!LoadPath - path to the File for load", true, true );
}

void FShowConfiguration( const CConfigurator _Configurator ) {
  FPrintMessage( U"Units:", false );

  if( _Configurator.FGetUnits().size() == 0 )
    FPrintMessage( U"No units" );
  else {
    FPrintMessage( U"", false, false, true );

    for( const CUnit *VUnit : _Configurator.FGetUnits() )
      FShowUnit( VUnit, 0 );
  }

  FPrintMessage( U"*************************", true, true );

  FPrintMessage( U"Groups:", false );

  if( _Configurator.FGetGroups().size() == 0 )
    FPrintMessage( U"No groups" );
  else {
    FPrintMessage( U"", false, false, true );

    for( const CGroup *VGroup : _Configurator.FGetGroups() )
      FShowGroup( VGroup, 0 );
  }

  FPrintMessage( U"=========================", true, true );
}

int __cdecl wmain( int argc, wchar_t *argv[], wchar_t *envp[] ) {
  FPrintMessage( U"Compendium Console 3.0" );
  FPrintMessage( U"Compendium Lib 3.0" );

  FPrintMessage( U"---------------------", true, true );

  if( argc == 1 ) {
    FShowHowToUse();
    return 0;
  }

  uint64_t VCommand = 0;

  u32string VLoadPath, VSavePath;
  u32string VTargetGroup, VTargetUnit;

  for( uint64_t c = 1; c < static_cast< uint64_t >( argc ); c++ ) {
    if( wcscmp( argv [ c ], L"!Command" ) == 0 ) {
      if( wcscmp( argv [ c + 1 ], L"View" ) == 0 ) {
        ++c;
        VCommand = 0;
      } else {
        FPrintMessage( U"Invalid argument Value. Argument - Command : " + UConverter::FConvert<wstring, wchar_t, u32string, char32_t>( argv [ c + 1 ] ) );

        FPrintMessage( U"---------------------", true, true );

        FShowHowToUse();

        return -1;
      }
    } else if( wcscmp( argv [ c ], L"!LoadPath" ) == 0 ) {
      VLoadPath = UConverter::FConvert<wstring, wchar_t, u32string, char32_t>( argv [ ++c ] );
    } else {
      FPrintMessage( U"Unknown argument. Argument - " + UConverter::FConvert<wstring, wchar_t, u32string, char32_t>( argv [ c ] ) );

      FPrintMessage( U"---------------------", true, true );

      FShowHowToUse();

      return -1;
    }
  }

  CConfigurator VConfigurator;
  vector<CGroup *> VGroups;

  switch( VCommand ) {
    //View
    case 0:
    {
      if( VLoadPath.empty() ) {
        FPrintMessage( U"Load path is empty" );

        FPrintMessage( U"---------------------", true, true );

        FShowHowToUse();

        return -1;
      }

      ENErrorCodes VLoadResult = VConfigurator.FLoadConfigurationFromFile( VLoadPath.c_str() );

      FPrintMessage( U"Load result - " + UConverter::FConvert<wstring, wchar_t, u32string, char32_t>( to_wstring( static_cast< int64_t >( VLoadResult ) ) ), true, true );

      if( VLoadResult == ENErrorCodes::EC_OK )
        FShowConfiguration( VConfigurator );
    }
    break;
  }

  return 0;
}
